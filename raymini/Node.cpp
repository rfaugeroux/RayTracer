#include "Node.h"

Node::Node()
{
}

Node::~Node(){

}

const BoundingBox & Node::getBoundingBox() const{
    return bbox;
}
