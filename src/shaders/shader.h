#ifndef SHADER_H
#define SHADER_H

#include <vector>

#include "../core/ray.h"
#include "../lightsources/pointlightsource.h"
#include "../lightsources/arealightsource.h"
#include "../shapes/shape.h"

class Shader
{
public:
    Shader();
    Shader(Vector3D bgColor_);

    virtual Vector3D computeColor(const Ray &r,
                             const std::vector<Shape*> &objList,
                             const std::vector<LightSource*> &lsList) const = 0;

    Vector3D bgColor;
};

#endif // SHADER_H
