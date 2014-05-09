// *********************************************************
// Ray Tracer Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2012 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#include "RayTracer.h"
#include "Ray.h"
#include "Scene.h"
#include "KdTree.h"
#include "Triangle.h"
#include "DirectionGenerator.h"
#include <QProgressDialog>
#include <vector>
#include <math.h>

#include <iostream>
using namespace std;

static RayTracer * instance = NULL;

RayTracer * RayTracer::getInstance () {
    if (instance == NULL)
        instance = new RayTracer ();
    return instance;
}

void RayTracer::destroyInstance () {
    if (instance != NULL) {
        delete instance;
        instance = NULL;
    }
}

inline int clamp (float f, int inf, int sup) {
    int v = static_cast<int> (f);
    return (v < inf ? inf : (v > sup ? sup : v));
}

QImage RayTracer::render (const Vec3Df & camPos,
                          const Vec3Df & direction,
                          const Vec3Df & upVector,
                          const Vec3Df & rightVector,
                          float fieldOfView,
                          float aspectRatio,
                          unsigned int screenWidth,
                          unsigned int screenHeight,
                          int AA_MODE,
                          bool WITH_SHADOWS,
                          int LIGHT_SAMPLING) {
    QImage image (QSize (screenWidth, screenHeight), QImage::Format_RGB888);
    Scene * scene = Scene::getInstance ();
    QProgressDialog progressDialog ("Raytracing...", "Cancel", 0, 100);
    progressDialog.show ();

    DirectionGenerator dirGen(direction, upVector, rightVector, fieldOfView, aspectRatio, screenWidth, screenHeight);

    //Iterating through every screen's pixels
    for (unsigned int i = 0; i < screenWidth; i++) {
        progressDialog.setValue ((100*i)/screenWidth);
        for (unsigned int j = 0; j < screenHeight; j++) {

            //Generation of the directions of the ray to be traced for this pixel
            vector<Vec3Df> dirs;
            dirGen.generateMultipleDirs((float) i, (float) j, AA_MODE, dirs);

            vector<Vec3Df> pixelColors;

            //Loop over every ray
            for (unsigned int d_i = 0; d_i < dirs.size(); ++d_i) {
                Vec3Df ray_color = computeRayColor(dirs[d_i],
                                                   camPos,
                                                   scene->getObjects(),
                                                   scene->getLights(),
                                                   WITH_SHADOWS,
                                                   LIGHT_SAMPLING);
                pixelColors.push_back(ray_color);
            }

            //Computation of the final color: the mean of the colors returned by each ray
            Vec3Df c;
            for (unsigned int c_i = 0; c_i < pixelColors.size(); ++c_i) {
                c += pixelColors[c_i];
            }
            c /= (float) pixelColors.size();

            //Setting the color of the pixel
            image.setPixel (i, j, qRgb (clamp (c[0], 0, 255), clamp (c[1], 0, 255), clamp (c[2], 0, 255)));
        }
    }
    progressDialog.setValue (100);

    return image;
}

float RayTracer::phongBRDF(Vec3Df w0, Vec3Df wi, Vec3Df n, const Material& mat) const{

    Vec3Df r = 2*Vec3Df::dotProduct(wi, n)*n - wi;

    float diffuse = mat.getDiffuse()*Vec3Df::dotProduct(n, wi);
    float spec = Vec3Df::dotProduct(r, w0);
    if (spec < 0.f) spec = 0;

    float specular = mat.getSpecular()*pow( spec, mat.getShininess());
    float coeff = 0;
    if (diffuse >= 0) coeff+= diffuse;
    if (specular >= 0) coeff += specular;

    return coeff;
}

Vec3Df RayTracer::computeRayColor(const Vec3Df & direction,
                                  const Vec3Df & camPos,
                                  const vector<Object> & objects,
                                  const vector<Light> & lights,
                                  bool WITH_SHADOWS,
                                  int light_sampling) const {


    unsigned int intersected_object_idx;
    Triangle intersected_triangle;
    Vec3Df intersection_bary_coord;

    bool intersection_found = findClosestIntersection (direction,
                                                       camPos,
                                                       objects,
                                                       intersection_bary_coord,
                                                       intersected_triangle,
                                                       intersected_object_idx);

    if (!intersection_found) return backgroundColor;

    const Object & intersected_object = objects.at(intersected_object_idx);
    Vec3Df ray_color(0.f, 0.f, 0.f);
    const vector<Vertex> & vertices = intersected_object.getMesh().getVertices();
    Vec3Df intersection_point = intersected_object.getTrans();
    intersection_point += intersection_bary_coord[0] * vertices.at(intersected_triangle.getVertex(0)).getPos();
    intersection_point += intersection_bary_coord[1] * vertices.at(intersected_triangle.getVertex(1)).getPos();
    intersection_point += intersection_bary_coord[2] * vertices.at(intersected_triangle.getVertex(2)).getPos();

    // Normal interpolation
    Vec3Df n = intersection_bary_coord[0]*vertices.at(intersected_triangle.getVertex(0)).getNormal();
    n += intersection_bary_coord[1]*vertices.at(intersected_triangle.getVertex(1)).getNormal();
    n += intersection_bary_coord[2]*vertices.at(intersected_triangle.getVertex(2)).getNormal();
    n.normalize();

    Vec3Df w0 = -direction;

    float gloss = intersected_object.getMaterial().getGloss();

    Vec3Df reflectedColor(0.f, 0.f, 0.f);
    Vec3Df baseColor(0.f, 0.f, 0.f);

    if(gloss > 0.f){
        Vec3Df reflected_w0 = 2*Vec3Df::dotProduct(w0, n)*n - w0;
        reflectedColor = computeRayColor(reflected_w0, intersection_point, objects, lights, WITH_SHADOWS, light_sampling);
    }

    if(gloss < 1.f) {

        // Loop over every light
        for (unsigned int l_i = 0; l_i < lights.size(); ++l_i) {
            const Light & light = lights[l_i];

            Vec3Df wi = light.getPos() - intersection_point;
            wi.normalize();


            float visibility = 1.f;

            if (WITH_SHADOWS){
                visibility = computeVisibility(intersection_point, light, light_sampling, objects);
            }

            if (visibility == 0) continue;

            float f = phongBRDF(w0, wi, n, intersected_object.getMaterial());

            baseColor += visibility * f * light.getColor()* intersected_object.getMaterial().getColor()* 255.f;
        }

        baseColor /= lights.size();
    }

    //Ex-BRDF
    //c = 255.f * ((intersectionPoint - minBb) / rangeBb);

    return Vec3Df::interpolate(baseColor, reflectedColor, gloss);
}



bool RayTracer::findClosestIntersection(const Vec3Df & direction,
                                        const Vec3Df & camPos,
                                        const vector<Object> & objects,
                                        Vec3Df & intersection_bary_coord,
                                        Triangle & intersected_triangle,
                                        unsigned int & intersected_object_idx) const {

    bool intersectionFound = false;
    float smallestIntersectionDistance = 1000000.f;

    //Loop over every objects in the scene
    for (unsigned int i = 0; i < objects.size (); i++) {
        const Object & o = objects.at(i);

        Ray ray(camPos-o.getTrans(), direction);

        //Parcours Triangles Kd-Tree
        vector<Triangle>  triangles;
        if (!ray.intersect(o, triangles)) continue;


        const vector<Vertex> & vertices = o.getMesh().getVertices();
        for(unsigned int j = 0; j < triangles.size(); ++j) {

            const Vec3Df& p0 = vertices.at(triangles[j].getVertex(0)).getPos();
            const Vec3Df& p1 = vertices.at(triangles[j].getVertex(1)).getPos();
            const Vec3Df& p2 = vertices.at(triangles[j].getVertex(2)).getPos();
            Vec3Df intersection_point;
            float intersectionDistance = -1;

            if (ray.intersect (p0, p1, p2, intersection_point, intersectionDistance)) {

                intersectionFound = true;

                if (intersectionDistance < smallestIntersectionDistance && intersectionDistance > 0) {

                    smallestIntersectionDistance = intersectionDistance;

                    intersected_object_idx = i;
                    intersected_triangle = triangles[j];
                    intersection_bary_coord = intersection_point;
                }
            }
        }
    }
    return intersectionFound;
}

float RayTracer::computeVisibility (const Vec3Df & point, const Light & light,
                                    int sampling_density, const vector<Object> & objects) const {

    float visibility = (float) sampling_density*sampling_density;
    float step = light.getRadius()/sampling_density;

    Vec3Df u, v;
    light.getNormal().getTwoOrthogonals(u, v);
    for (int i = 0; i < sampling_density; ++i) {
        for (int j = 0; j < sampling_density; ++j) {

            Vec3Df direction;

            if(sampling_density > 1) {
                float x = step * (i + (rand() % 1000000)/1000000.f);
                float y = step * (j + (rand() % 1000000)/1000000.f);
                Vec3Df light_sample_point = light.getPos() + x*u + y*v;
                direction = light_sample_point - point;
            }
            else {
                direction = light.getPos() - point;
            }
            direction.normalize();

            for (unsigned int j = 0; j < objects.size(); ++j) {
                const Object & o = objects.at(j);
                Ray ray(point - o.getTrans(), direction);
                if (ray.intersect(o)) {
                    visibility--;
                    break;
                }
            }
        }
    }
    return visibility/(sampling_density*sampling_density);
}

/*
void RayTracer::computeAO (const vector<Object> & objects, const Vertex & vertex, int AO_sampling) {

    for (int i = 0; i < AO_sampling; ++i) {
        float theta = 0.5f * M_PI * (rand() % 100000)/100000.f;
        float phi = 2.f * M_PI * (rand() % 100000)/100000.f;

    }

}
*/


