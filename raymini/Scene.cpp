// *********************************************************
// Scene Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#include "Scene.h"

#include <cmath>

using namespace std;

static Scene * instance = NULL;

Scene * Scene::getInstance () {
    if (instance == NULL)
        instance = new Scene ();
    return instance;
}

void Scene::destroyInstance () {
    if (instance != NULL) {
        delete instance;
        instance = NULL;
    }
}

Scene::Scene () {
    //buildDefaultScene ();
    buildOriginalScene1();
    updateBoundingBox ();
}

Scene::~Scene () {
}

void Scene::updateBoundingBox () {
    if (objects.empty ())
        bbox = BoundingBox ();
    else {
        bbox = objects[0].getBoundingBox ();
        for (unsigned int i = 1; i < objects.size (); i++)
            bbox.extendTo (objects[i].getBoundingBox ());
    }
}

// Changer ce code pour creer des scenes originales
void Scene::buildDefaultScene () {

    Mesh groundMesh;
    groundMesh.loadOFF ("models/ground.off");
    Material groundMat;
    Object ground (groundMesh, groundMat);
    objects.push_back (ground);

    Mesh ramMesh;
    ramMesh.loadOFF ("models/ram.off");
    Material ramMat (1.f, 1.f, 2.f, 0.f, Vec3Df (1.f, .6f, .2f));
    Object ram (ramMesh, ramMat);
    ram.setTrans (Vec3Df (0.4f, 0.6f, 0.f));
    objects.push_back (ram);

    Mesh mirrorMesh;
    mirrorMesh.loadOFF("models/mirror.off");
    Material mirrorMat(1.f, 1.f, 2.f, 0.7f, Vec3Df(0.2f, 1.f, 1.f));
    Object mirror(mirrorMesh, mirrorMat);
    mirror.setTrans(Vec3Df(0.f, 0.f, 0.f));
    objects.push_back(mirror);
    //    for (int i = 0; i < 9; ++i) {
    //        Mesh ramMesh;
    //        ramMesh.loadOFF ("models/ram.off");
    //        Object ram (ramMesh, ramMat);
    //        ram.setTrans (Vec3Df (-2 + 2.f * (i%3), 2*i/3.f, 0.f));
    //        objects.push_back (ram);
    //    }
    Mesh rhinoMesh;
    rhinoMesh.loadOFF ("models/rhino.off");
    Material rhinoMat (1.0f, 0.2f, 1.f, 0.f, Vec3Df (0.6f, 0.6f, 0.7f));
    Object rhino (rhinoMesh, rhinoMat);
    rhino.setTrans (Vec3Df (-1.f, -1.f, 0.4f));
    objects.push_back (rhino);
    //    Mesh gargMesh;
    //    gargMesh.loadOFF ("models/gargoyle.off");
    //    Material gargMat (0.7f, 0.4f, 2.f, 0.f, Vec3Df (0.5f, 0.8f, 0.5f));
    //    Object garg (gargMesh, gargMat);
    //    garg.setTrans (Vec3Df (-1.f, 1.0f, 0.1f));
    //    objects.push_back (garg);

    //    Mesh monkeyMesh;
    //    monkeyMesh.loadOFF("models/monkey.off");
    //    Material monkeyMat (1.0f, 0.2f, 2.f, 0.f, Vec3Df (0.6f, 0.6f, 0.7f));
    //    Object monkey(monkeyMesh, monkeyMat);
    //    monkey.setTrans (Vec3Df (0.f, 0.f, 0.f));
    //    objects.push_back(monkey);

    //    Mesh sphereMesh;
    //    sphereMesh.loadOFF("models/sphere.off");
    //    sphereMesh.scale(0.5f);
    //    Material sphereMat (1.0f, 0.2f, 2.f, 1.f, Vec3Df (0.6f, 0.6f, 0.7f));
    //    Object sphere(sphereMesh, sphereMat);
    //    sphere.setTrans (Vec3Df (0.8f, 0.6f, 0.f));
    //    objects.push_back (sphere);

    //    Mesh triangleMesh;
    //    triangleMesh.loadOFF("models/triangles.off");
    //    Material triangleMat;
    //    Object triangle(triangleMesh, triangleMat);
    //    triangle.setTrans (Vec3Df (0.f, 0.f, 0.f));
    //    objects.push_back (triangle);


    Light l0 (Vec3Df (2.0f, -2.0f, 10.0f), Vec3Df(-1.f, 0.f, 0.f), 1.f, Vec3Df (1.0f, 1.0f, 1.0f), 1.0f);
    lights.push_back (l0);

    //    Light l1 (Vec3Df (-10.0f, 10.0f, -10.0f), Vec3Df(-1.f, 0.f, 0.f), 1.0f, Vec3Df (1.0f, 1.0f, 1.0f), 1.0f);
    //    lights.push_back (l1);
}

void Scene::buildOriginalScene1 () {


    //Creation of the box
    Mesh groundMesh;
    groundMesh.loadOFF ("models/ground.off");
    groundMesh.scale(Vec3Df(1, 2, 1));
    Material groundMat (1.f, 1.f, 2.f, 0.2f, Vec3Df(0.6f, 0.7f, 0.7f) );
    Object ground (groundMesh, groundMat);
    objects.push_back (ground);
    Mesh leftWallMesh;
    leftWallMesh.loadOFF ("models/groundX.off");
    leftWallMesh.scale(Vec3Df(1, 2, 1.f));
    Material leftWallMat(1.f, 1.f, 2.f, 0.f, Vec3Df(67/255.f, 240/255.f, 100/255.f));
    Object leftWall (leftWallMesh, leftWallMat);
    leftWall.setTrans(Vec3Df(-2.f, 0.f, 1.f));
    objects.push_back (leftWall);
    Mesh bottomWallMesh;
    bottomWallMesh.loadOFF ("models/groundY.off");
    bottomWallMesh.scale(Vec3Df(1, 1, 1.f));
    Material bottomWallMat(1.f, 1.f, 2.f, 0.f, Vec3Df(254/255.f, 110/255.f, 87/255.f));
    Object bottomWall (bottomWallMesh, bottomWallMat);
    bottomWall.setTrans(Vec3Df(0.f, 4.f, 1.f));
    objects.push_back (bottomWall);


    // First statue
    Mesh box1Mesh;
    box1Mesh.loadOFF ("models/box.off");
    box1Mesh.scale(Vec3Df(0.9f, 0.9f, 0.6f));
    Material box1Mat(1.f, 1.f, 2.f, 0.f, Vec3Df(242/255.f, 209/255.f, 116/255.f));
    Object box1 (box1Mesh, box1Mat);
    box1.setTrans(Vec3Df(-1.3f, -2.6f, 0.f));
    objects.push_back (box1);

    Mesh manMesh;
    manMesh.loadOFF ("models/soldier.off");
    manMesh.scale(Vec3Df(2.f, 2.f, 2.f));
    manMesh.rotate(0);
    manMesh.rotate(0);
    manMesh.rotate(0);
    manMesh.rotate(2);
    manMesh.rotate(2);
    manMesh.rotate(2);
    Material manMat(1.f, 1.f, 2.f, 0.f, Vec3Df(0.2f, 0.2f, 0.2f));
    Object man (manMesh, manMat);
    man.setTrans(Vec3Df(-1.3f, -2.6f, 0.52f));
    objects.push_back (man);


    // Second statue
    Mesh box2Mesh;
    box2Mesh.loadOFF ("models/box.off");
    box2Mesh.scale(Vec3Df(0.7f, 0.7f, 1.2f));
    Material box2Mat(1.f, 1.f, 2.f, 0.f, Vec3Df(242/255.f, 209/255.f, 116/255.f));
    Object box2 (box2Mesh, box2Mat);
    box2.setTrans(Vec3Df(-1.3f, 0.f, 0.f));
    objects.push_back (box2);

    Mesh vaseMesh;
    vaseMesh.loadOFF ("models/vase.off");
    vaseMesh.rotate(0);
    vaseMesh.rotate(0);
    vaseMesh.rotate(0);
    vaseMesh.scale(0.7f);
    Material vaseMat(1.f, 1.f, 2.f, 0.f, Vec3Df(20/255.f, 24/255.f, 100/255.f));
    Object vase (vaseMesh, vaseMat);
    vase.setTrans(Vec3Df(-1.3f, 0.7f, 1.2f));
    objects.push_back (vase);

    // Third statue
    Mesh box3Mesh;
    box3Mesh.loadOFF ("models/box.off");
    box3Mesh.scale(Vec3Df(0.7f, 0.7f, 1.f));
    Material box3Mat(1.f, 1.f, 2.f, 0.f, Vec3Df(242/255.f, 209/255.f, 116/255.f));
    Object box3 (box3Mesh, box3Mat);
    box3.setTrans(Vec3Df(-1.3f, 2.6f, 0.f));
    objects.push_back (box3);

    Mesh monkeyMesh;
    monkeyMesh.loadOFF ("models/monkey.off");
    monkeyMesh.rotate(0);
    monkeyMesh.rotate(0);
    monkeyMesh.rotate(0);
    monkeyMesh.rotate(2);
    monkeyMesh.rotate(2);
    monkeyMesh.rotate(2);
    monkeyMesh.scale(0.3f);
    Material monkeyMat(1.f, 1.f, 2.f, 0.f, Vec3Df(94/255.f, 56/255.f, 13/255.f));
    Object monkey (monkeyMesh, monkeyMat);
    monkey.setTrans(Vec3Df(-1.f, 3.f, 1.3f));
    objects.push_back (monkey);

    //Lights
    Light l0 (Vec3Df (12.0f, 0.f, 5.0f), Vec3Df(-1.f, 0.f, 0.f), 1.f, Vec3Df (1.0f, 1.0f, 1.0f), 1.0f);
    lights.push_back (l0);

    Light l1 (Vec3Df (0.f, 5.f, 5.0f), Vec3Df(-1.f, 0.f, 0.f), 1.f, Vec3Df (1.0f, 1.0f, 1.0f), 1.0f);
    lights.push_back (l1);

    Light l2 (Vec3Df (2.f, -8.0f, 5.0f), Vec3Df(-1.f, 0.f, 0.f), 1.f, Vec3Df (1.0f, 1.0f, 1.0f), 1.0f);
    lights.push_back (l2);


}

void Scene::buildOriginalScene2 () {

    Mesh groundMesh;
    groundMesh.loadOFF ("models/ground.off");
    groundMesh.scale(Vec3Df(20.f, 25.f, 1.f));
    Material groundMat;
    Object ground (groundMesh, groundMat);
    ground.setTrans(Vec3Df(0.f, -15.f, 0.f));
    objects.push_back (ground);

    Mesh killerooMesh;
    killerooMesh.loadOFF ("models/killeroo.off");
    killerooMesh.scale(0.3f);
    killerooMesh.rotate(0);
    killerooMesh.rotate(0);
    killerooMesh.rotate(0);
    killerooMesh.rotate(2);
    killerooMesh.rotate(2);
    Material killerooMat (1.f, 1.f, 2.f, 0.f, Vec3Df (0.1f, .6f, 0.5f));
    Object killeroo (killerooMesh, killerooMat);
    killeroo.setTrans (Vec3Df (0., 29.f, 4.f));
    objects.push_back (killeroo);


    float stepX = 2.2f;
    float stepY = 2.2f;
    float offsetX = -2.8f;
    float offsetY = 3.f;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            Mesh ramMesh;
            ramMesh.loadOFF ("models/ram.off");
            Material ramMat (1.f, 1.f, 2.f, 0.f, Vec3Df (1.f, .6f, .2f));
            Object ram (ramMesh, ramMat);
            float randX = (rand()%100)/100.f - 0.5f;
            float randY = (rand()%100)/100.f - 0.5f;
            ram.setTrans (Vec3Df (offsetX + i*stepX + randX, offsetY + j*stepY + randY, 0.f));
            objects.push_back (ram);
        }
    }

    Vec3Df lightPos(Vec3Df (2.0f, 25.0f, 15.0f));
    Vec3Df dir = -lightPos;
    dir.normalize();

    Light l0 (lightPos , dir, 2.f, Vec3Df (1.0f, 1.0f, 1.0f), 1.0f);
    lights.push_back (l0);

}

void Scene::buildOriginalScene3 () {

}

void Scene::rotateLight(float theta) {
    const Vec3Df & pos = lights[0].getPos();
    Vec3Df rotated_pos(cos(theta)*pos[0] + sin(theta)*pos[2],
            pos[1],
            cos(theta)*pos[2] - sin(theta)*pos[0] );
    lights[0].setPos(rotated_pos);
    /*
    for (int i = 1; i < 10; ++i) {
        objects.at(i).setTrans(objects.at(i).getTrans() + Vec3Df(0.f, -0.2f, 0.f));
    }
    */
}

void Scene::animateScene2(float speed) {
    objects.at(1).setTrans(objects.at(1).getTrans() + Vec3Df(0.f, -1.1f*speed, 0.f));
    for (int i = 1; i < 11; ++i) {
        float randX = speed*((rand()%100)/1000.f - 0.05f);
        float randY = speed*(rand()%100)/1000.f - 0.05f;
        objects.at(i).setTrans(objects.at(i).getTrans() + Vec3Df(randX, randY-speed, 0.f));
    }
}
