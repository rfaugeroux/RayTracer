#include "KdTree.h"
#include "Leaf.h"
#include "SimpleNode.h"

#include <iostream>

using namespace std;

KdTree::KdTree()
{
}

KdTree::KdTree(const Mesh& mesh, unsigned int maxNumberTriangles){

    //If there is only one triangle in the object, or if maxDepth is specified lower than 1
    if(mesh.getTriangles().size() <= maxNumberTriangles){

        root = new Leaf(mesh.getVertices(), mesh.getTriangles());

    }
    else{

        root = new SimpleNode(mesh.getVertices(), mesh.getTriangles(), maxNumberTriangles);

    }
}

//Copy constructor
KdTree::KdTree(const KdTree & kdTree){

    const Node & node_to_cp = kdTree.getRoot();
    if(node_to_cp.isLeaf()){
        root = new Leaf(node_to_cp.getBoundingBox(), node_to_cp.getTriangles());
    }
    else {
        root = new SimpleNode(node_to_cp.getBoundingBox(), node_to_cp.getLeftChild(), node_to_cp.getRightChild());
    }
}

//Assignment operator: same code than the copy constructor
KdTree & KdTree::operator=(const KdTree & kdTree){

    const Node & node_to_cp = kdTree.getRoot();
    if(node_to_cp.isLeaf()){
        root = new Leaf(node_to_cp.getBoundingBox(), node_to_cp.getTriangles());
    }
    else {
        root = new SimpleNode(node_to_cp.getBoundingBox(), node_to_cp.getLeftChild(), node_to_cp.getRightChild());
    }
    return *this;
}

KdTree::~KdTree(){
    delete root;
}

const Node & KdTree::getRoot() const{
    return *root;
}

unsigned int KdTree::depth () const {
    return root->depth();
}

void KdTree::print() const{
    cout << "KdTree" << endl;
    cout << "------\n" << endl;
    root->print(0);
}
