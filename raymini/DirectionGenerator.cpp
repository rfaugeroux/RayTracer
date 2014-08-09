#include "DirectionGenerator.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

DirectionGenerator::DirectionGenerator()
{
}

DirectionGenerator::DirectionGenerator (const Vec3Df &_direction,
                                        const Vec3Df &_upVector,
                                        const Vec3Df &_rightVector,
                                        float fieldOfView,
                                        float aspectRatio,
                                        unsigned int screenWidth,
                                        unsigned int screenHeight) {


    float tanX = tan (fieldOfView)*aspectRatio;
    float tanY = tan (fieldOfView);
    stepX = tanX / screenWidth;
    offsetX = -tanX / 2.f;
    stepY = tanY / screenHeight;
    offsetY = -tanY / 2.f;

    direction = _direction;
    rightVector = _rightVector;
    upVector = _upVector;

}

Vec3Df DirectionGenerator::generateDir(float x, float y){

    Vec3Df vX = (x*stepX + offsetX) * rightVector;
    Vec3Df vY = (y*stepY + offsetY) * upVector;

    Vec3Df dir = direction + vX + vY;
    dir.normalize();
    return dir;
}

void DirectionGenerator::generateMultipleDirs(float p_x, float p_y, int MODE, vector<Vec3Df> &dirs){

    dirs.clear();

    switch (MODE) {
    // No anti aliasing
    case 0:
        dirs.push_back(generateDir(p_x, p_y));
        break;
    // 2x2 grid
    case 1:
        dirs.push_back(generateDir(p_x - 0.25f, p_y - 0.25f));
        dirs.push_back(generateDir(p_x - 0.25f, p_y + 0.25f));
        dirs.push_back(generateDir(p_x + 0.25f, p_y - 0.25f));
        dirs.push_back(generateDir(p_x + 0.25f, p_y + 0.25f));
        break;
    // 3x3 grid
    case 2:
        for (int i = -1; i < 2; ++i) {
            for (int j = -1; j < 2; ++j) {
                dirs.push_back(generateDir(p_x + i * 0.5f, p_y + j * 0.5f));
            }
        }
        break;
    // Rotated grid (2x2 grid with rotation of angle arctan(1/2))
    case 3:
    {
        float r = sqrt(5.f/32.f);
        dirs.push_back(generateDir(p_x + r * cos( M_PI / 4.f + atan (1.f / 2.f) ), p_y + r * sin(M_PI / 4.f + atan (1.f / 2.f) ) ));
        dirs.push_back(generateDir(p_x + r * cos(3 * M_PI / 4.f + atan(1.f / 2.f)), p_y + r * sin(3 * M_PI / 4.f + atan (1.f / 2.f ) )));
        dirs.push_back(generateDir(p_x + r * cos(5 * M_PI / 4.f + atan(1.f / 2.f)), p_y + r * sin(5 * M_PI/ 4.f + atan ( 1.f / 2.f) ) ));
        dirs.push_back(generateDir(p_x + r * cos(7 * M_PI/ 4.f + atan(1.f / 2.f) ), p_y + r * sin(7 * M_PI / 4.f + atan (1.f / 2.f) ) ));
        break;
    }
    // Jitter in a 5x5 grid
    case 4:
    {
        srand (static_cast <unsigned> (time(0)));
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/0.2f));
                float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/0.2f));
                dirs.push_back(generateDir(p_x -0.5f + i * 0.2f + r1, p_y - 0.5f + j * 0.2f + r2));
            }
        }
    }
        break;
    default:
        dirs.push_back(generateDir(p_x, p_y));
        break;
    }

}

void DirectionGenerator::print() const {

    cout << "Direction : " << direction << endl;
    cout << "Right vector : " << rightVector << endl;
    cout << "Up vector : " << upVector << endl;
    cout << "Step X : " << stepX << endl;
    cout << "Step Y : " << stepY << endl;
    cout << "Offset X : " << offsetX << endl;
    cout << "Offset Y : " << offsetY << endl;
}
