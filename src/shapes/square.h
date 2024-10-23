#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>
#include <string>

#include "shape.h"
#include "../core/eqsolver.h"

class Square : public Shape
{
public:
    Square() = delete;
    Square(const Vector3D pos_, const Vector3D & v1_, const Vector3D& v2_, const Vector3D& normal_,Material *material_);

    Vector3D getNormalWorld(const Vector3D &pt_world) const;

    bool rayIntersect(const Ray &ray, Intersection &its) const;
    bool rayIntersectP(const Ray &ray) const;
    std::string toString() const;


    Vector3D normal;
    Vector3D corner;
    Vector3D v1;
    Vector3D v2;

    Vector3D    w;//constant for a given quadrilateral

};

std::ostream& operator<<(std::ostream &out, const Square &s);

#endif // SPHERE_H
