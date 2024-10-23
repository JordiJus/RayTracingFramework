#include "utils.h"

Utils::Utils()
{ }

double Utils::degreesToRadians(double degrees)
{
    return degrees * M_PI / 180.0;
}



bool Utils::hasIntersection(const Ray& cameraRay, const std::vector<Shape*>& objectsList) //or Shadow Ray
{

    // For each object on the scene...
    for(size_t objIndex = 0; objIndex < objectsList.size(); objIndex ++)
    {
          // Get the current object
          const Shape *obj = objectsList.at(objIndex);
          if (obj->rayIntersectP(cameraRay))
              return true;
    }   

    return false;
}



bool Utils::getClosestIntersection(const Ray& cameraRay, const std::vector<Shape*>& objectsList, Intersection& its) //or Closest Hit Ray
{
    //std::cout << "Need to implement the function Utils::getClosestIntersection() in the file utils.cpp" << std::endl;

    bool hasIntersection = false;

    for (size_t objIndex = 0; objIndex < objectsList.size(); objIndex++)
    {
        // Get the current object
        const Shape* obj = objectsList.at(objIndex);
        if (obj->rayIntersect(cameraRay, its))
            hasIntersection =  true;
    }
    return hasIntersection;
}

double interpolate(double val, double y0, double x0, double y1, double x1 )
{
    return (val-x0)*(y1-y0)/(x1-x0) + y0;
}

double getRed(double value)
{
    if (value > 0.5)
        return interpolate( value, 0.0, 0.5, 1.0, 1.0 );
    else
        return 0.0;
}

double getGreen(double value)
{
    if (value < 0.25)
        return 0.0;
    else if (value < 0.5)
        return interpolate(value, 0.0, 0.25, 1, 0.5);
    else if (value < 0.75)
        return interpolate(value, 1, 0.5, 0, 0.75);
    else
        return 0;
}

double getBlue(double value)
{
    if (value < 0.5)
        return interpolate(value, 1.0, 0.0, 0.0, 0.5);
    else
        return 0;
}


Vector3D Utils::scalarToRGB(double scalar)
{
    return Vector3D( getRed(scalar),
                getGreen(scalar),
                getBlue(scalar) );
}

Vector3D Utils::computeReflectionDirection(const Vector3D &Direction, const Vector3D &normal)
{
    Vector3D wr = normal * (dot(Direction, normal) * 2) - Direction;

    return wr;
}

bool Utils::isTotalInternalReflection(const Vector3D& n, const Vector3D& wi, const double mu_t) {

    double ratio = mu_t;
    Vector3D normal = n;

    if (dot(wi, n) > 0) {
        ratio = 1 / mu_t;
        normal = -n;
    }

    double intReflec = 1 - pow(ratio, 2) * (1 - pow(dot(normal, -wi), 2));
    if (intReflec < 0) {
        return true;
    }
    else {
        return false;
    }
}

double Utils::clamp(double num, double max, double min) {
    if (num < min) {
        return min;
    }
    else if (num > max) {
        return max;
    }
    else {
        return num;
    }
}
