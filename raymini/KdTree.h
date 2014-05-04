#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include "BoundingBox.h"
#include "Mesh.h"
#include "Node.h"


class KdTree
{
public:
    KdTree();
    KdTree(const Mesh &mesh, unsigned int maxNumberTriangles=15);
    KdTree(const KdTree & kdTree);

    KdTree & operator= (const KdTree & kdTree);

    ~KdTree();

    const Node & getRoot() const;
    void print() const;


protected:
    Node* root;
};

#endif // KDTREE_H
