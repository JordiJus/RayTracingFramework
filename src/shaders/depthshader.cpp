#include "depthshader.h"

#include "../core/utils.h"

DepthShader::DepthShader() :
    color(Vector3D(1, 0, 0))
{ }

DepthShader::DepthShader(Vector3D hitColor_, double maxDist_, Vector3D bgColor_) :
    Shader(bgColor_), maxDist(maxDist_), color(hitColor_)
{ }

Vector3D DepthShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<LightSource*> &lsList) const
{
    /*
    for (int i = 0; i < objList.size(); i++) {
        if (objList[i]->rayIntersectP(r)) {
            // std::cout << r.depth;
            double ci = fmax((1 - ((double)r.depth) / maxDist), 0);
            
            
            return Vector3D(ci,color.y,color.z);
        }
    }
    return bgColor;
    */

    Intersection its;

    if (Utils::getClosestIntersection(r, objList, its)) {
        
        double ci = fmax((1 - ((double)(r.o - its.itsPoint).length()) / maxDist), 0);
        return Vector3D(ci, 1.f, ci);
    }
    return bgColor;
}
