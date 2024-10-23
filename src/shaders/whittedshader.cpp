#include "whittedshader.h"
#include "../core/utils.h"
#include "../core/ray.h"
#include "../materials/transmission.h"

WhittedShader::WhittedShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

WhittedShader::WhittedShader(Vector3D bgColor_) :
    Shader(bgColor_)
{ }

Vector3D WhittedShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    Intersection its;
    double Vx = 1;
    Vector3D color = (0.0, 0.0, 0.0);

    if (Utils::getClosestIntersection(r, objList, its)) {
        Vector3D Lo = Vector3D(0.0);

        if (its.shape->getMaterial().hasTransmission() == true) {
            
            Vector3D wt;
            Vector3D normal = its.normal.normalized();
            Vector3D wi = r.d;
            double ratio = 0.7;


            
            if (!Utils::isTotalInternalReflection(normal, wi, its.shape->getMaterial().getIndexOfRefraction())) {
                Vector3D wt = its.shape->getMaterial().getReflectance(normal, Vector3D(0.0), wi);
                Ray rayTra = Ray(its.itsPoint, wt.normalized(), r.depth + 1);
                color += computeColor(rayTra, objList, lsList);
                return color;
            }

            else {
                //Vector3D wr = its.shape->getMaterial().getReflectance(its.normal, Vector3D(0.0), -r.d);
                if (dot(wi,normal) > 0) {
                    ratio = 1 / ratio;
                    normal = -normal;
                }
                Vector3D wr = Utils::computeReflectionDirection(wi, normal);
                Ray rayVis = Ray(its.itsPoint, wr.normalized(), r.depth + 1);
                color += computeColor(rayVis, objList, lsList);
                return color;
            }
            
        }

        else if (its.shape->getMaterial().hasSpecular() == true) {
            Vector3D wr = Utils::computeReflectionDirection(-r.d, its.normal);
            Ray rayVis = Ray(its.itsPoint, wr.normalized(), r.depth + 1);
            color += computeColor(rayVis, objList, lsList);

        }
        else {
            for (int i = 0; i < lsList.size(); i++) {
                Vector3D Li = lsList[i]->getIntensity();
                Vector3D wis = lsList[i]->sampleLightPosition() - its.itsPoint;
                Vector3D fr = its.shape->getMaterial().getReflectance(its.normal, wis.normalized(), r.d);
                Vector3D win = dot(wis.normalized(), its.normal);

                Ray rayVis = Ray(its.itsPoint, wis.normalized(), 0, 0.0001, wis.length());
                Intersection itsLs;
                if (Utils::getClosestIntersection(rayVis, objList, itsLs)) {
                    Vx = 0;
                }
                else {
                    Vx = 1;
                }


                Lo += Li * fr * win * Vx;

            }

                
            Vector3D ambientL = its.shape->getMaterial().getDiffuseReflectance() * 0.2;

            color += Lo + ambientL;


            
        }   
        
    }
    return Vector3D(Utils::clamp(color.x, 1, 0), Utils::clamp(color.y, 1, 0), Utils::clamp(color.z, 1, 0));
}
