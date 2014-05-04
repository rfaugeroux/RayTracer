#ifndef DIRECTIONGENERATOR_H
#define DIRECTIONGENERATOR_H

#include "Vec3D.h"
#include <vector>
#include <math.h>


// A class to compute the directions of the ray used by the ray tracer
class DirectionGenerator
{
public:
    DirectionGenerator();

    DirectionGenerator(const Vec3Df & _direction,
                       const Vec3Df & _upVector,
                       const Vec3Df & _rightVector,
                       float fieldOfView,
                       float aspectRatio,
                       unsigned int screenWidth,
                       unsigned int screenHeight);

    Vec3Df generateDir(float x, float y);
    void generateMultipleDirs(float p_x, float p_y, int MODE, std::vector<Vec3Df> & dirs);

    void print() const;

private:

    Vec3Df direction;
    Vec3Df rightVector;
    Vec3Df upVector;
    float stepX;
    float stepY;
    float offsetX;
    float offsetY;

};

#endif // DIRECTIONGENERATOR_H
