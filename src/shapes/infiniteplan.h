#ifndef INFINITEPLAN_H
#define INFINITEPLAN_H

#include "shape.h"

class InfinitePlan : public Shape
{
public:
    InfinitePlan() = delete;
    InfinitePlan(const Vector3D &p0_, const Vector3D &normal_,
                 Material *mat_);

    // Get the normal at a surface point in world coordinates
    Vector3D getNormalWorld() const;

    // Ray/plan intersection methods
    bool rayIntersect(const Ray &ray, Intersection &its) const;
    bool rayIntersectP(const Ray &rayWorld) const;


    // Convert triangle to String
    std::string toString() const;

private:
    // A point belonging to the plan, and the normal at the plan
    /* All values are in world coordinates */
    Vector3D p0World;
    Vector3D nWorld;
};

std::ostream &operator<<(std::ostream &out, const InfinitePlan &t);

#endif // INFINITEPLAN_H
