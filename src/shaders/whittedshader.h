#ifndef WHITTEDSHADER_H
#define WHITTEDSHADER_H

#include "shader.h"

class WhittedShader : public Shader
{
public:
    WhittedShader();
    WhittedShader(Vector3D bgColor_);

    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<LightSource*>& lsList) const;

    Vector3D hitColor;
};

#endif // INTERSECTIONSHADER_H
