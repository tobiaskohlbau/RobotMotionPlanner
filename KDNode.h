#ifndef KDNODE_H_
#define KDNODE_H_

#include <cstdint>

template<uint16_t dim, class T>
class KDNode
{
public:
    KDNode(Vec<dim, float> vec, T node);

    std::shared_ptr<KDNode<dim, T>> left;
    std::shared_ptr<KDNode<dim, T>> right;
    Vec<dim, float> vec;
    T node;
    uint16_t axis;
    float value;
private:

};

template<uint16_t dim, class T>
KDNode<dim, T>::KDNode(Vec<dim, float> vec, T node) {
    this->vec = vec;
    this->node = node;
    axis = 0;
    value = 0;
}

#endif /* KDNODE_H_ */
