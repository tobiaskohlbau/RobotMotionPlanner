#ifndef EDGE_H_
#define EDGE_H_

#include <core/Base.h>

namespace rmpl {

class Node;

/*!
* \brief   Class Edge contains the two nodes of the Edge and different parameters
* \author  Sascha Kaden
* \date    2016-05-25
*/
class Edge : public Base
{
public:
    Edge();
    Edge(const std::shared_ptr<Node> &source, const std::shared_ptr<Node> &target);

    float getLength();

    void setSource(const std::shared_ptr<Node> &source);
    void setTarget (const std::shared_ptr<Node> &target);

    std::shared_ptr<Node> getSource();
    std::shared_ptr<Node> getTarget();

private:
    std::shared_ptr<Node> m_source;
    std::shared_ptr<Node> m_target;
    float m_length;
};

} /* namespace rmpl */

#endif /* EDGE_H_ */
