#include "intersectionshader.h"
#include "../core/utils.h"

IntersectionShader::IntersectionShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

IntersectionShader::IntersectionShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D IntersectionShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<LightSource*> &lsList) const
{
    //(FILL..)
    /*
    for (int i = 0; i < objList.size(); i++) {
        if (objList[i]->rayIntersectP(r)) {
            return hitColor;
        }
    }
    return bgColor;
    */

    if (Utils::hasIntersection(r, objList)) {
        return hitColor;
    }
    return bgColor;
}
