#ifndef MATERIAL
#define MATERIAL

#include "../core/vector3d.h"

class Material
{
public:
    Material();

    virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
                                    const Vector3D &wi) const = 0; //Return Phong BRDF of Phong Materials and Emissive Diffuse

    virtual double getIndexOfRefraction() const; // Return Refraction ratio of Transmissive Materials
    virtual Vector3D getEmissiveRadiance() const; //Return Emissive Radiance of Emissive Materials
    virtual Vector3D getDiffuseReflectance() const; //Return Difusse Coefficient of Phong Materials

    //Functions to check the material of the object
    //TODO
    virtual bool hasSpecular() const = 0;
    virtual bool hasTransmission() const = 0;
    virtual bool hasDiffuseOrGlossy() const = 0;
    virtual bool isEmissive() const = 0;


    
};


#endif // MATERIAL
