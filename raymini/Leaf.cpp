#include "Leaf.h"

#include <iostream>

using namespace std;

Leaf::Leaf()
{   
}

Leaf::Leaf(const BoundingBox & _bbox, const std::vector<Triangle> &_triangles){
    bbox = _bbox;
    triangles = _triangles;
}

Leaf::Leaf(const vector<Vertex>& vertices, const vector<Triangle>& _triangles){

    triangles = _triangles;
    bbox = BoundingBox(vertices, _triangles);
}

Leaf::~Leaf(){
}

const Node & Leaf::getLeftChild() const{
    return Leaf();
}

const Node & Leaf::getRightChild() const{
    return Leaf();
}

const std::vector<Triangle> & Leaf::getTriangles() const{
    return triangles;
}

bool Leaf::isLeaf() const{
    return true;
}

unsigned int Leaf::depth() const{
    return 1;
}

void Leaf::print(int depth) const{
    cout << "Leaf: Depth = " << depth << endl;
    cout << "Bounding box: " << endl;
    cout << bbox.getHeight() << endl;
    cout << bbox.getLength() << endl;
    cout << bbox.getWidth() << endl;
    cout << "Number of triangles: " << triangles.size() << endl << endl;
}
