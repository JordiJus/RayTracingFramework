#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H


// To start, let this be the interface of a point light source
// Then, make this an abstract class from which we can derive:
//   - omnidirectional uniform point light sources
//   - area light sources

class LightSource
{
public:
    LightSource() {}; 


    virtual Vector3D getIntensity() const = 0;
    virtual Vector3D sampleLightPosition() const = 0;

    virtual double getArea() const = 0;
    virtual Vector3D getNormal() const = 0;


};

#endif 
