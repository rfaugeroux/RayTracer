#ifndef NODE_H
#define NODE_H

#include "Triangle.h"
#include "BoundingBox.h"
#include <vector>

class Node
{
public:
    Node();

    virtual ~Node();

    const BoundingBox & getBoundingBox() const;
    virtual const Node & getLeftChild() const = 0;
    virtual const Node & getRightChild() const = 0;
    virtual const std::vector<Triangle> & getTriangles() const = 0;

    virtual bool isLeaf() const =0;
    virtual unsigned int depth() const =0;
    virtual void print(int depth) const = 0;

protected:
    BoundingBox bbox;
};

#endif // NODE_H
