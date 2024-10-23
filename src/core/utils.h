#ifndef UTILS_H
#define UTILS_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "ray.h"
#include "../shapes/shape.h"


#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

class Utils
{
public:
    Utils();

    static bool getClosestIntersection(const Ray &cameraRay, const std::vector<Shape*> &objectsList, Intersection &its);
    static bool hasIntersection(const Ray &ray, const std::vector<Shape*> &objectsList);
    static Vector3D scalarToRGB(double scalar);
    static double degreesToRadians(double degrees);

    static bool isTotalInternalReflection(const Vector3D& n, const Vector3D& wi, const double mu_t);

    static Vector3D computeReflectionDirection(const Vector3D &Direction, const Vector3D &normal);



    static void printProgress(double percentage) {
        int val = (int)(percentage * 100);
        int lpad = (int)(percentage * PBWIDTH);
        int rpad = PBWIDTH - lpad;
        printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
        fflush(stdout);
    };

    static double clamp(double num, double max, double min);

};

#endif // UTILS_H
