// *********************************************************
// Ray Tracer Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2012 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <iostream>
#include <vector>
#include <QImage>

#include "Vec3D.h"
#include "Material.h"
#include "Scene.h"
#include "Object.h"
#include "Light.h"
#include "Ray.h"

class RayTracer {
public:
    static RayTracer * getInstance ();
    static void destroyInstance ();

    inline const Vec3Df & getBackgroundColor () const { return backgroundColor;}
    inline void setBackgroundColor (const Vec3Df & c) { backgroundColor = c; }
    
    QImage render (const Vec3Df & camPos,
                   const Vec3Df & viewDirection,
                   const Vec3Df & upVector,
                   const Vec3Df & rightVector,
                   float fieldOfView,
                   float aspectRatio,
                   unsigned int screenWidth,
                   unsigned int screenHeight, int AA_MODE, bool WITH_SHADOWS, int LIGHT_SAMPLING);

    float phongBRDF (Vec3Df w0, Vec3Df wi, Vec3Df n, const Material& mat) const;
    Vec3Df computeRayColor (const Vec3Df &direction,
                            const Vec3Df &camPos,
                            const Scene *scene,
                            bool WITH_SHADOWS, int light_sampling) const;
    bool findClosestIntersection (const Vec3Df &direction,
                                  const Vec3Df &camPos,
                                  const std::vector<Object> &objects,
                                  Vec3Df &intersection_bary_coord,
                                  Triangle & intersected_triangle,
                                  unsigned int & intersected_object_idx) const;
    float computeVisibility (const Vec3Df &point, const Light & light,
                             int sampling_density, const std::vector<Object> & objects) const;
    float computeAmbientOcclusion (const Vec3Df & intersection_point, const Vec3Df & normal,
                                             const vector<Object> & objects, float radius, int AO_sampling) const;

protected:
    inline RayTracer () {}
    inline virtual ~RayTracer () {}
    
private:
    Vec3Df backgroundColor;
};


#endif // RAYTRACER_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
