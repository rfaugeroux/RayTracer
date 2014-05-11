#ifndef LEAF_H
#define LEAF_H

#include "Node.h"
#include "Triangle.h"
#include "Vertex.h"
#include "BoundingBox.h"
#include <vector>

class Leaf: public Node
{
public:
    Leaf();
    Leaf(const BoundingBox &_bbox, const std::vector<Triangle>& _triangles);
    Leaf(const std::vector<Vertex>& vertices, const std::vector<Triangle>& _triangles);

    virtual ~Leaf();

    const Node & getLeftChild() const;
    const Node & getRightChild() const;
    const std::vector<Triangle> & getTriangles() const;

    virtual bool isLeaf() const;
    virtual unsigned int depth() const;
    virtual void print(int depth) const;

private:

    std::vector<Triangle> triangles;

};

#endif // LEAF_H
