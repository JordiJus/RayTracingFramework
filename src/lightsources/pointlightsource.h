#ifndef POINTLIGHTSOURCE_H
#define POINTLIGHTSOURCE_H

#include "../core/vector3d.h"
#include "lightsource.h"


class PointLightSource : public LightSource
{
public:
    PointLightSource() = delete;
    PointLightSource(Vector3D pos_, Vector3D intensity_) :
        pos(pos_), intensity(intensity_)
    { }



    Vector3D getIntensity() const { return intensity; };
    Vector3D sampleLightPosition() const { return pos; };

    ////A point light emits light uniformly in all directions
    //Its Area is zero and have no Normal
    double getArea() const { return 0.0; };              
    Vector3D getNormal() const { return Vector3D(0.0); };

private:
    Vector3D pos;
    Vector3D intensity; // (unity: watts/sr)
};

#endif // POINTLIGHTSOURCE_H
