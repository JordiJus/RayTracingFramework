#ifndef MIRRORMATERIAL
#define MIRRORMATERIAL

#include "material.h"

class Mirror : public Material
{
public:
    Mirror();
    Mirror(Vector3D Ke_, Vector3D rho_d_);

    Vector3D getReflectance(const Vector3D& n, const Vector3D& wo,
        const Vector3D& wi) const;

    bool hasSpecular() const { return true; }
    bool hasTransmission() const { return false; }
    bool hasDiffuseOrGlossy() const { return true; }
    bool isEmissive() const { return false; }

    double getIndexOfRefraction() const;
    Vector3D getEmissiveRadiance() const;
    Vector3D getDiffuseReflectance() const;

private:
    Vector3D Ke;    Vector3D rho_d;
};


#endif // MATERIAL
