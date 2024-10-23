#ifndef EMISSIVEMATERIAL
#define EMISSIVEMATERIAL

#include "material.h"

class Emissive : public Material
{
public:
    Emissive();
    Emissive(Vector3D Ke_, Vector3D rho_d_);

    Vector3D getReflectance(const Vector3D& n, const Vector3D& wo,
        const Vector3D& wi) const ;

    bool hasSpecular() const { return false; }
    bool hasTransmission() const { return false; }
    bool hasDiffuseOrGlossy() const { return true; }
    bool isEmissive() const { return true; }

    double getIndexOfRefraction() const;
    Vector3D getEmissiveRadiance() const;
    Vector3D getDiffuseReflectance() const;

private:
    Vector3D Ke;    Vector3D rho_d;
};


#endif // MATERIAL