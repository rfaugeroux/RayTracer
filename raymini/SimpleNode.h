#ifndef SIMPLENODE_H
#define SIMPLENODE_H

#include "Node.h"
#include "BoundingBox.h"
#include "Vertex.h"
#include "Triangle.h"
#include "Vec3D.h"
#include <vector>

using namespace std;

class SimpleNode : public Node
{
public:
    SimpleNode();
    SimpleNode(const BoundingBox &_bbox, const Node & _leftTree, const Node & _rightTree);
    SimpleNode(const vector<Vertex>& vertices, const vector<Triangle>& triangles,
         unsigned int maxNumberTriangles);
    SimpleNode(const Node & node);

    virtual ~SimpleNode();

    virtual const Node & getLeftChild() const;
    virtual const Node & getRightChild() const;
    virtual const vector<Triangle> & getTriangles() const;

    virtual bool isLeaf() const;
    virtual void print(int depth) const;

private:
    Node* leftTree;
    Node* rightTree;

};

#endif // SIMPLENODE_H
