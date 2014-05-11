#include "SimpleNode.h"
#include "Leaf.h"

#include <algorithm>
#include <iostream>

using namespace std;

SimpleNode::SimpleNode()
{
}

SimpleNode::SimpleNode(const BoundingBox & _bbox, const Node &_leftTree, const Node &_rightTree){

    bbox = _bbox;

    //Copying left child
    if (_leftTree.isLeaf()){
        leftTree = new Leaf(_leftTree.getBoundingBox(), _leftTree.getTriangles());
    }
    else{
        leftTree = new SimpleNode(_leftTree.getBoundingBox(),
                                  _leftTree.getLeftChild(), _leftTree.getRightChild());
    }

    //Copying right child
    if (_rightTree.isLeaf()){
        rightTree = new Leaf(_rightTree.getBoundingBox(), _rightTree.getTriangles());
    }
    else{
        rightTree = new SimpleNode(_rightTree.getBoundingBox(),
                                  _rightTree.getLeftChild(), _rightTree.getRightChild());
    }

}

SimpleNode::SimpleNode(const vector<Vertex> & vertices, const vector<Triangle> & triangles,
                       unsigned int maxNumberTriangles){

    bbox = BoundingBox(vertices, triangles);

    int d = bbox.getMaxDir();
    //cout << "Width: " << bbox.getWidth() << endl;
    //cout << "Height: " << bbox.getHeight() << endl;
    //cout << "Length: " << bbox.getLength() << endl;
    //cout << "Dim: " << d << endl;

    //Computation of the barycenters of the triangles, projected on the dimension to be cut
    vector<float> projBarycenters;
    for (unsigned int i = 0; i < triangles.size(); ++i) {
        Triangle t = triangles[i];
        float projBary = vertices[t.getVertex(0)].getPos()[d];
        projBary += vertices[t.getVertex(1)].getPos()[d];
        projBary += vertices[t.getVertex(2)].getPos()[d];
        projBary /= 3.f;
        projBarycenters.push_back(projBary);
    }

    //Find the median value where to cut
    int middle = projBarycenters.size()/2;

    std::nth_element(projBarycenters.begin(), projBarycenters.begin() + middle, projBarycenters.end());
    float cut = projBarycenters[middle];

    vector<Triangle> leftTriangles;
    vector<Triangle> rightTriangles;

    bool left = false;

    for (unsigned int i = 0; i < projBarycenters.size(); ++i) {

        if (projBarycenters[i] < cut ) {
            leftTriangles.push_back(triangles[i]);
        }
        else if (projBarycenters[i] > cut ){
            rightTriangles.push_back(triangles[i]);
        }
        // When equal to cut, put alternatively left of right
        else {
            if(left){
                leftTriangles.push_back(triangles[i]);
            }
            else {
                rightTriangles.push_back(triangles[i]);
            }
            left = !left;
        }
    }

    if (leftTriangles.size() > maxNumberTriangles) {
        leftTree = new SimpleNode(vertices, leftTriangles, maxNumberTriangles);
    }
    else {
        leftTree = new Leaf(vertices, leftTriangles);
    }

    if (rightTriangles.size() > maxNumberTriangles) {
        rightTree = new SimpleNode(vertices, rightTriangles, maxNumberTriangles);
    }
    else {
        rightTree = new Leaf(vertices, rightTriangles);
    }
}

SimpleNode::~SimpleNode(){
    delete leftTree;
    delete rightTree;
}

const Node & SimpleNode::getLeftChild() const{
    return *leftTree;
}

const Node & SimpleNode::getRightChild() const{
    return *rightTree;
}

const vector<Triangle> & SimpleNode::getTriangles() const{
    return vector<Triangle>();
}



bool SimpleNode::isLeaf() const{
    return false;
}

unsigned int SimpleNode::depth() const {
    unsigned int leftDepth = 1 + leftTree->depth();
    unsigned int rightDepth = 1 + rightTree->depth();

    return leftDepth>rightDepth ? leftDepth:rightDepth;
}

void SimpleNode::print(int depth) const{
    cout << "SimpleNode: depth = " << depth << endl;
    cout << "Bounding box: " << bbox.getHeight()*bbox.getLength()*bbox.getWidth() << endl << endl;
    leftTree->print(depth + 1);
    rightTree->print(depth + 1);
}
