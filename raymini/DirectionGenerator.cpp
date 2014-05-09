#include "DirectionGenerator.h"
#include <iostream>

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
    // Pentagon
    case 3:
        break;
    // Jitter
    case 4:

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
