// *********************************************************
// Ray Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#include "Ray.h"

using namespace std;

static const unsigned int NUMDIM = 3, RIGHT = 0, LEFT = 1, MIDDLE = 2;

bool Ray::intersect (const BoundingBox & bbox, Vec3Df & intersectionPoint, float distMax) const {
    const Vec3Df & minBb = bbox.getMin ();
    const Vec3Df & maxBb = bbox.getMax ();
    bool inside = true;
    unsigned int  quadrant[NUMDIM];
    register unsigned int i;
    unsigned int whichPlane;
    Vec3Df maxT;
    Vec3Df candidatePlane;
    
    for (i=0; i<NUMDIM; i++)
        if (origin[i] < minBb[i]) {
            quadrant[i] = LEFT;
            candidatePlane[i] = minBb[i];
            inside = false;
        } else if (origin[i] > maxBb[i]) {
            quadrant[i] = RIGHT;
            candidatePlane[i] = maxBb[i];
            inside = false;
        } else	{
            quadrant[i] = MIDDLE;
        }

    if (inside)	{
        intersectionPoint = origin;
        return (true);
    }

    for (i = 0; i < NUMDIM; i++)
        if (quadrant[i] != MIDDLE && direction[i] !=0.)
            maxT[i] = (candidatePlane[i]-origin[i]) / direction[i];
        else
            maxT[i] = -1.;

    whichPlane = 0;
    for (i = 1; i < NUMDIM; i++)
        if (maxT[whichPlane] < maxT[i])
            whichPlane = i;

    if (maxT[whichPlane] < 0.) return (false);
    for (i = 0; i < NUMDIM; i++)
        if (whichPlane != i) {
            intersectionPoint[i] = origin[i] + maxT[whichPlane] *direction[i];
            if (intersectionPoint[i] < minBb[i] || intersectionPoint[i] > maxBb[i])
                return (false);
        } else {
            intersectionPoint[i] = candidatePlane[i];
        }
    if (distMax>0 && Vec3Df::distance(intersectionPoint, origin) > distMax) {
        return false;
    }
    return true;
}

bool Ray::intersect (const Vec3Df &p0, const Vec3Df &p1, const Vec3Df &p2,
                     Vec3Df & intersectionBarCoord, float & t,
                     float distMax, float distMin) const {

    Vec3Df e0 = p1 - p0;
    Vec3Df e1 = p2 - p0;
    Vec3Df n = Vec3Df::crossProduct(e0,e1);
    n.normalize();
    Vec3Df q = Vec3Df::crossProduct(direction,e1);
    float a = Vec3Df::dotProduct(e0, q);

    if (Vec3Df::dotProduct(n, direction) >= 0 ||  abs(a) <  0.000001) {
        return false;
    }

    Vec3Df s = (origin-p0)/a;
    Vec3Df r = Vec3Df::crossProduct(s, e0);

    float b1 = Vec3Df::dotProduct(s, q);
    float b2 = Vec3Df::dotProduct(r, direction);
    float b0 = 1 - b1 - b2;

    if (b0 < 0.f || b1 < 0.f || b2 < 0.f) {
        return false;
    }

    t = Vec3Df::dotProduct(e1, r);

    if (distMax > 0 && t > distMax) {
        return false;
    }

    if (t >= distMin) {
        intersectionBarCoord[0] = b0;
        intersectionBarCoord[1] = b1;
        intersectionBarCoord[2] = b2;

        return true;
    }

    return false;

}

bool Ray::intersect (const Node & node, const vector<Vertex> & vertices, float distMax, float distMin) {

    Vec3Df point;
    if(!intersect(node.getBoundingBox(), point, distMax)) return false;

    if ( !node.isLeaf()) {
        const Node & leftChild = node.getLeftChild();
        const Node & rightChild = node.getRightChild();
        return intersect(leftChild, vertices, distMax, distMin) || intersect(rightChild, vertices, distMax, distMin);
    }

    const vector<Triangle> & triangles = node.getTriangles();

    for (unsigned int i = 0; i < triangles.size(); ++i) {
        const Vec3Df & p0 = vertices.at(triangles[i].getVertex(0)).getPos();
        const Vec3Df & p1 = vertices.at(triangles[i].getVertex(1)).getPos();
        const Vec3Df & p2 = vertices.at(triangles[i].getVertex(2)).getPos();

        float t;

        if(intersect(p0, p1, p2, point, t, distMax, distMin)) return true;
    }

    return false;

}

bool Ray::intersect(const Object & object, float distMax, float distMin){

    const Node & root = object.getKdTree().getRoot();

    return intersect (root, object.getMesh().getVertices(), distMax, distMin);
}

bool Ray::intersect(const Node &node, const vector<Vertex> & vertices, vector<Triangle> & intersected_triangles) {

    Vec3Df point;
    if (!intersect(node.getBoundingBox(), point)) return false;

    if ( !node.isLeaf()) {
        const Node & leftChild = node.getLeftChild();
        const Node & rightChild = node.getRightChild();
        bool intersect_left = intersect(leftChild, vertices, intersected_triangles);
        bool intersect_right = intersect(rightChild, vertices, intersected_triangles);

        return intersect_left || intersect_right;
    }

    const vector<Triangle> & triangles = node.getTriangles();

    for (unsigned int i = 0; i < triangles.size(); ++i) {
        const Vec3Df & p0 = vertices.at(triangles[i].getVertex(0)).getPos();
        const Vec3Df & p1 = vertices.at(triangles[i].getVertex(1)).getPos();
        const Vec3Df & p2 = vertices.at(triangles[i].getVertex(2)).getPos();

        float t;

        if(intersect(p0, p1, p2, point, t)) intersected_triangles.push_back(triangles[i]);
    }

    return !triangles.empty();
}

bool Ray::intersect(const Object & object, vector<Triangle> & intersected_triangles) {
    const Node & root = object.getKdTree().getRoot();
    return intersect(root, object.getMesh().getVertices(), intersected_triangles);
}
