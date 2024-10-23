#include "transmission.h"
#include <iostream>

Transmissive::Transmissive()

{ }

Transmissive::Transmissive(double mu_t_) :
    mu_t(mu_t_) {}

Vector3D Transmissive::getReflectance(const Vector3D& n, const Vector3D& wo,
    const Vector3D& wi) const {

    double ratio = mu_t;
    Vector3D normal = n;

    if (dot(wi, normal) > 0) {
        ratio = 1 / ratio;
        normal = -n;
    }


    //Vector3D a1 = -sqrt(1 - ratio * ratio * (1 - (pow(dot(-wi, normal), 2)))) + ratio * dot(-wi,normal);
    //Vector3D a2 = (-wi) * ratio;
    //Vector3D wt = normal * a1 - a2;

    //double intReflec = sqrt(1 - pow(ratio, 2) * (1 - pow(dot(normal, -wi), 2)));
    //Vector3D wt = (-wi * (ratio)) + normal * (ratio * (dot(normal, -wi)) - intReflec);
    

    double intReflec = sqrt(1 - pow(ratio, 2) * (1 - pow(dot(normal, -wi), 2)));
    Vector3D wt = - (-wi * (ratio)) + normal * (ratio * (dot(normal, -wi)) - intReflec);

    return wt;
};

double Transmissive::getIndexOfRefraction() const
{
    return mu_t;
}


Vector3D Transmissive::getEmissiveRadiance() const
{
    return Vector3D(0.0);
}


Vector3D Transmissive::getDiffuseReflectance() const
{
    return rho_d;
}
