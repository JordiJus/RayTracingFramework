#ifndef AREALIGHTSOURCE_H
#define AREALIGHTSOURCE_H

#include "../shapes/square.h"
#include "lightsource.h"


class AreaLightSource : public LightSource
{
public:
    AreaLightSource() = delete;
    
    AreaLightSource(Square* areaLightsource);


    Vector3D getIntensity() const;        
    Vector3D sampleLightPosition() const ;
    Vector3D generateRandomPoint() const;

    double getArea() const {
        Vector3D square_dim = myAreaLightsource->v1 + myAreaLightsource->v2;
        if (square_dim.x <= Epsilon) {
            return square_dim.y * square_dim.z;
        }
        if (square_dim.y <= Epsilon) {
            return square_dim.x * square_dim.z;
        }
        if (square_dim.z <= Epsilon) {
            return square_dim.x * square_dim.y;
        }
    }

    Vector3D getNormal() const {
        return myAreaLightsource->normal;
    };

private:
    Square* myAreaLightsource;
};

#endif 
