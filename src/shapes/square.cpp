#include "square.h"


Square::Square(const Vector3D pos_, const Vector3D& v1_, const Vector3D& v2_, const Vector3D& normal_, Material *material_)
    : Shape(Matrix4x4(), material_), corner(pos_), v1(v1_), v2(v2_), normal(normal_)
{ 
    Vector3D n = cross(v1_,v2);
    w = n / dot(n, n);
}

// Return the normal in world coordinates

Vector3D Square::getNormalWorld(const Vector3D &pt_world) const
{    
    return normal;
}

// Chapter 3 PBRT, page 117
bool Square::rayIntersect(const Ray &ray, Intersection &its) const
{
    //return false;  
     // Compute the denominator of the tHit formula
    double denominator = dot(ray.d, normal);

    // Test for parallel ray/plane
    if (std::abs(denominator) < Epsilon)
        return false;

    // Effectivelly compute the intersection distance
    double tHit = dot((corner - ray.o), normal) / denominator;

    // Is tHit outside the bounds of the ray segment we want to test intersecion?
    if (tHit < ray.minT || tHit > ray.maxT)
        return false;

    // Compute ray/plane the intersection point
    Vector3D p = ray.o + (ray.d * tHit);  

    
    //Check if the point is inside the square
    Vector3D planar_hitpt_vector = p - corner;

    double alpha = dot(w, cross(planar_hitpt_vector, v2));
    double beta = dot(w, cross(v1, planar_hitpt_vector));

    if ( !(alpha >0.0 && alpha <1.0) || !(beta > 0.0 && beta < 1.0))
        return false;

    // Update intersection info
    its.itsPoint = p;
    its.normal = normal;
    its.shape = this;

    // Update the ray maxT
    ray.maxT = tHit;

    return true;
}

// Chapter 3 PBRT, page 117
bool Square::rayIntersectP(const Ray &ray) const
{
    //return false;  
 // Compute the denominator of the tHit formula
    double denominator = dot(ray.d, normal);

    // Test for parallel ray/plane
    if (std::abs(denominator) < Epsilon)
        return false;

    // Effectivelly compute the intersection distance
    double tHit = dot((corner - ray.o), normal) / denominator;

    // Is tHit outside the bounds of the ray segment we want to test intersecion?
    if (tHit < ray.minT || tHit > ray.maxT)
        return false;

    // Compute ray/plane the intersection point
    Vector3D p = ray.o + (ray.d * tHit);

    //Check if the point is insde the square
    Vector3D planar_hitpt_vector = p - corner;
    double alpha = dot(w, cross(planar_hitpt_vector, v2));
    double beta = dot(w, cross(v1, planar_hitpt_vector));

    if (!(alpha > 0.0 && alpha < 1.0) || !(beta > 0.0 && beta < 1.0))
        return false;

    // Update the ray maxT
    ray.maxT = tHit;




    return true;
}

std::string Square::toString() const
{
    std::stringstream s;
    s << "[ " << std::endl
      << " Center (World) = " << objectToWorld.transformPoint(Vector3D(0,0,0)) << ", Radius = " << corner.x << std::endl
      << "]" << std::endl;

    return s.str();
}

std::ostream& operator<<(std::ostream &out, const Square&s)
{
    out << s.toString();
    return out;
}
