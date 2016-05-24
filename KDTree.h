#ifndef KDTREE_H_
#define KDTREE_H_

#include <cstdint>

#include "KDNode.h"
#include "Node.h"

using std::shared_ptr;

template<class T>
class KDTree
{
public:
    void addNode(const Vec<float> &vec, T node);
    T searchNearestNeighbor(const Vec<float> &vec);
    std::vector<T> searchRange(const Vec<float> &vec, const float &range);

private:
    shared_ptr<KDNode<T>> insert(shared_ptr<KDNode<T>> insertNode, shared_ptr<KDNode<T>> currentNode, unsigned int depth);
    void NNS(const Vec<float> &point, shared_ptr<KDNode<T>> node, shared_ptr<KDNode<T>> &refNode, float &bestDist);
    void RS(const Vec<float> &point, shared_ptr<KDNode<T>> node, std::vector<shared_ptr<KDNode<T>>> &refNodes,
        const float &sqRange, const Vec<float> &maxBoundary, const Vec<float> &minBoundary);
    //variables
    shared_ptr<KDNode<T>> m_root;
};

template<class T>
void KDTree<T>::addNode(const Vec<float> &vec, T node) {
    shared_ptr<KDNode<T>> shrKDNode(new KDNode<T>(vec, node));
    if (m_root == NULL) {
        m_root = shrKDNode;
        return;
    }
    insert(shrKDNode, m_root, 0);
}

template<class T>
shared_ptr<KDNode<T>> KDTree<T>::insert(shared_ptr<KDNode<T>> insertNode, shared_ptr<KDNode<T>> currentNode, unsigned int cd) {
    if (currentNode == NULL) {              // node at leaf doesn't exist and will be added
        currentNode = insertNode;
        currentNode->axis = cd;
        currentNode->value = currentNode->vec[cd];
    }
    else if (insertNode == currentNode) {   // error! duplicate
        return currentNode;
    }
    else if (insertNode->vec[cd] < currentNode->vec[cd]) {
        currentNode->left = insert(insertNode, currentNode->left, (cd+1) % insertNode->getDim());
    }
    else {
        currentNode->right = insert(insertNode, currentNode->right, (cd+1) % insertNode->getDim());
    }
    return currentNode;
}

template<class T>
T KDTree<T>::searchNearestNeighbor(const Vec<float> &vec) {
    if (m_root == NULL)
        return NULL;

    shared_ptr<KDNode<T>> node;
    float bestDist = std::numeric_limits<float>::max();
    NNS(vec, m_root, node, bestDist);
    return node->node;
}

template<class T>
std::vector<T> KDTree<T>::searchRange(const Vec<float> &vec, const float &range) {
    std::vector<T> nodes;
    if (m_root == NULL)
        return nodes;

    std::vector<shared_ptr<KDNode<T>>> kdNodes;
    float sqRange = range * range;
    Vec<float> maxBoundary = vec + range;
    Vec<float> minBoundary = vec - range;
    RS(vec, m_root, kdNodes, sqRange, maxBoundary, minBoundary);

    for (int i = 0; i < kdNodes.size(); ++i)
        nodes.push_back(kdNodes[i]->node);
    return nodes;
}

template<class T>
void KDTree<T>::NNS(const Vec<float> &vec, shared_ptr<KDNode<T>> node, shared_ptr<KDNode<T>> &refNode, float &bestDist) {
    if (node->left == NULL && node->right == NULL) {
        float dist = vec.getSqDist(node->vec);
        if (dist < bestDist) {
            bestDist = dist;
            refNode = node;
        }
    }
    else {
        if (vec[node->axis] <= node->value) {
            if (vec[node->axis] - bestDist <= node->value && node->left != NULL)
                NNS(vec, node->left, refNode, bestDist);
            if (vec[node->axis] + bestDist > node->value && node->right != NULL)
                NNS(vec, node->right, refNode, bestDist);
        }
        else {
            if (vec[node->axis] + bestDist > node->value && node->right != NULL)
                NNS(vec, node->right, refNode, bestDist);
            if (vec[node->axis] - bestDist <= node->value && node->left != NULL)
                NNS(vec, node->left, refNode, bestDist);
        }
    }
}

template<class T>
void KDTree<T>::RS(const Vec<float> &vec, shared_ptr<KDNode<T>> node, std::vector<shared_ptr<KDNode<T>>> &refNodes,
    const float &sqRange, const Vec<float> &maxBoundary, const Vec<float> &minBoundary) {
    if (node == NULL) {
        return;
    }
    if (vec.getSqDist(node->vec) < sqRange) {
        refNodes.push_back(node);
    }
    if (vec[node->axis] < maxBoundary[node->axis] && vec[node->axis] < minBoundary[node->axis]) {
        RS(vec, node->left, refNodes, sqRange, maxBoundary, minBoundary);
    }
    else if (vec[node->axis] > maxBoundary[node->axis] && vec[node->axis] > minBoundary[node->axis]) {
        RS(vec, node->right, refNodes, sqRange, maxBoundary, minBoundary);
    }
    else {
        RS(vec, node->left, refNodes, sqRange, maxBoundary, minBoundary);
        RS(vec, node->right, refNodes, sqRange, maxBoundary, minBoundary);
    }
}

#endif /* KDTREE_H_ */