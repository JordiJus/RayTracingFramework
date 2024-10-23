#include "phong.h"

#define _USE_MATH_DEFINES
#include <cmath>

Phong::Phong()
{ }

Phong::Phong(Vector3D Kd_, Vector3D Ks_, float alpha_):
rho_d(Kd_), Ks(Ks_), alpha(alpha_){}


Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo,
    const Vector3D& wi) const {

    Vector3D wr = (n * (2 * dot(n, wi))) - wi;
    Vector3D fr = (this->rho_d / 3.14) + (this->Ks * pow(dot(wo, wr), this->alpha)) * ((2 * 3.14) / (this->alpha + 1));


    //Vector3D wr = (n * (2 * dot(n, wi))) - wi;
    //Vector3D fr = (this->rho_d / 3.14) + (this->Ks * pow(dot(wo, wr), this->alpha));

    return fr;

};

double Phong::getIndexOfRefraction() const
{
    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
              << std::endl;

    return -1;
}


Vector3D Phong::getEmissiveRadiance() const
{
    return Vector3D(0.0);
}


Vector3D Phong::getDiffuseReflectance() const
{
    return rho_d;
}

