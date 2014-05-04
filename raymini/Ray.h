// *********************************************************
// Ray Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#ifndef RAY_H
#define RAY_H

#include <iostream>
#include <vector>

#include "Vec3D.h"
#include "BoundingBox.h"
#include "Triangle.h"
#include "Object.h"
#include "Node.h"

using namespace std;

class Ray {
public:
    inline Ray () {}
    inline Ray (const Vec3Df & origin, const Vec3Df & direction)
        : origin (origin), direction (direction) {}
    inline virtual ~Ray () {}

    inline const Vec3Df & getOrigin () const { return origin; }
    inline Vec3Df & getOrigin () { return origin; }
    inline const Vec3Df & getDirection () const { return direction; }
    inline Vec3Df & getDirection () { return direction; }

    bool intersect (const BoundingBox & bbox, Vec3Df & intersectionPoint, float distMax=-1) const;
    bool intersect (const Vec3Df & p0, const Vec3Df & p1, const Vec3Df & p2,
                    Vec3Df &intersectionBarCoord, float &t, float distMax=-1, float distMin=0.00001f) const;
    bool intersect (const Node & node, const vector<Vertex> & vertices, float distMax, float distMin);
    bool intersect (const Object & object, float distMax=-1, float distMin=0.00001f);
    bool intersect (const Node & node, const vector<Vertex> & vertices, vector<Triangle> & intersected_triangles);
    bool intersect (const Object & object, vector<Triangle> & intersected_triangles);

private:
    Vec3Df origin;
    Vec3Df direction;
};


#endif // RAY_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
