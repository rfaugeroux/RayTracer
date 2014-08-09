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
#include <omp.h>

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

QImage RayTracer::render (const Vec3Df & camPos, const Vec3Df & direction,
                          const Vec3Df & upVector, const Vec3Df & rightVector,
                          float fieldOfView, float aspectRatio,
                          unsigned int screenWidth, unsigned int screenHeight,
                          int BRDF_MODE, int AA_MODE,
                          bool WITH_SHADOWS, int LIGHT_SAMPLING) {
    QImage image (QSize (screenWidth, screenHeight), QImage::Format_RGB888);
    Scene * scene = Scene::getInstance ();
    QProgressDialog progressDialog ("Raytracing...", "Cancel", 0, 100);
    progressDialog.show ();

    DirectionGenerator dirGen(direction, upVector, rightVector, fieldOfView, aspectRatio, screenWidth, screenHeight);

    //Iterating through every screen's pixels
    //#pragma omp parallel for
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
                                                   scene,
                                                   BRDF_MODE,
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

float RayTracer::phongBRDF(const Vec3Df & w0, const Vec3Df & wi, const Vec3Df & n, const Material& mat) const{

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

float RayTracer::CookTorranceBRDF(const Vec3Df & w0, const Vec3Df & wi, const Vec3Df & n, const Material& mat) const{

    Vec3Df half_vector = w0 + wi;
    half_vector.normalize();
    float NdotL        = max(0.f, Vec3Df::dotProduct( n, w0 ));
    float NdotH        = max(0.f,  Vec3Df::dotProduct( n, half_vector ));
    float NdotV        = max(0.f, Vec3Df::dotProduct( n, wi ));
    float VdotH        = max(0.f, Vec3Df::dotProduct( wi, half_vector ));
    float roughness_square  = 2.f / ( mat.getShininess() + 2.f);


    // Computation of the geometric term
    float geo_num1   = 2.0f * NdotH;
    float geo1 = (geo_num1 * NdotV ) / VdotH;
    float geo2 = (geo_num1 * NdotL ) / VdotH;
    float geo  = min( 1.0f, min(geo1, geo2) );

    // Computation of the roughness term  (microfacet Beckmann's distribution)
    float roughness;
    float r1 = 1.0f / ( 4.0f * roughness_square * pow( NdotH, 4 ) );
    float r2 = (NdotH * NdotH - 1.0f) / (roughness_square * NdotH * NdotH);
    roughness = r1 * exp( r2 );


    // Computation of the Fresnel term
    float refractionCoeff = 0.1f;
    float fresnel = pow( 1.0f - VdotH, 5.0f );
    fresnel *= ( 1.0f - refractionCoeff);
    fresnel += refractionCoeff;

    // Computation of the specular term
    float specular = (fresnel * geo * roughness ) / (M_PI * NdotV * NdotL);

    float coeff = max(0.0f, NdotL) * (mat.getSpecular() * specular + mat.getDiffuse());

    return coeff;
}

Vec3Df RayTracer::computeRayColor(const Vec3Df & direction, const Vec3Df & camPos, const Scene * scene,
                                  int BRDF_MODE, bool WITH_SHADOWS, int light_sampling) const {

    const vector<Light> & lights = scene->getLights();
    const vector<Object> & objects = scene->getObjects();


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

    // If there is gloss, compute reflection
    if(gloss > 0.f){
        Vec3Df reflected_w0 = 2*Vec3Df::dotProduct(w0, n)*n - w0;
        reflectedColor = computeRayColor(reflected_w0, intersection_point, scene, BRDF_MODE, WITH_SHADOWS, light_sampling);
    }

    // If it is not pure reflection, compute Direct Ligthing
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

            float f;
            switch (BRDF_MODE) {
            case 0:
                f = phongBRDF(w0, wi, n, intersected_object.getMaterial());
                break;
            case 1:
                f = CookTorranceBRDF(w0, wi, n, intersected_object.getMaterial());
                break;
            default:
                f = 1;
                break;
            }

            baseColor += visibility * f * light.getColor()* intersected_object.getMaterial().getColor()* 255.f;
        }

        baseColor /= lights.size();
    }

    // Compute Ambient Occlusion

    //float ambient_occlusion = computeAmbientOcclusion(intersection_point, n, objects,
    //                                                  0.05f*scene->getBoundingBox().getRadius(),  20);

    //Ex-BRDF
    //c = 255.f * ((intersectionPoint - minBb) / rangeBb);

    return Vec3Df::interpolate(baseColor, reflectedColor, gloss);
    //return ambient_occlusion * Vec3Df(240.f, 240.f, 240.f);

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


float RayTracer::computeAmbientOcclusion (const Vec3Df & intersection_point, const Vec3Df & normal,
                                          const vector<Object> & objects, float radius, int AO_sampling) const {

    Vec3Df u, v;
    normal.getTwoOrthogonals(u, v);
    Vec3Df direction;
    Ray ray;

    float ambient_occlusion = (float) AO_sampling;
    for (int i = 0; i < AO_sampling; ++i) {
        float theta = 0.5f * M_PI * (rand() % 100000)/100000.f;
        float phi = 2.f * M_PI * (rand() % 100000)/100000.f;
        direction = cos(theta) * normal + sin(theta) * (cos(phi)*u + sin(phi)*v);
        ray.setDirection(direction);
        for (unsigned int i = 0; i < objects.size(); ++i) {
            ray.setOrigin(intersection_point - objects[i].getTrans());
            if ( ray.intersect(objects[i], radius)) {
                ambient_occlusion--;
                break;
            }
        }
    }
    return ambient_occlusion/AO_sampling;
}



