// *********************************************************
// Material Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <vector>

#include "Vec3D.h"

// Ce modèle suppose une couleur spéculaire blanche (1.0, 1.0, 1.0)

class Material {
public:
    inline Material () : diffuse (0.8f), specular (0.2f), shininess(2.f), color (0.5f, 0.5f, 0.5f)  {}
    inline Material (float diffuse, float specular, float shininess, float gloss, const Vec3Df & color)
        : diffuse (diffuse), specular (specular), shininess(shininess), gloss(gloss), color (color) {}
    virtual ~Material () {}

    inline float getDiffuse () const { return diffuse; }
    inline float getSpecular () const { return specular; }
    inline float getShininess () const { return shininess; }
    inline float getGloss () const { return gloss; }
    inline Vec3Df getColor () const { return color; }

    inline void setDiffuse (float d) { diffuse = d; }
    inline void setSpecular (float s) { specular = s; }
    inline void setShininess (float s) { shininess = s; }
    inline void setGloss (float g) { gloss = g; }
    inline void setColor (const Vec3Df & c) { color = c; }
    
private:
    float diffuse;
    float specular;
    float shininess;
    float gloss;
    Vec3Df color;
};


#endif // MATERIAL_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
