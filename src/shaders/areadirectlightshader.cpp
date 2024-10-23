#include "areadirectlightshader.h"
#include "../core/Utils.h"

AreaDirectLightShader::AreaDirectLightShader():
	Shader(Vector3D(0.0))
{}

AreaDirectLightShader::AreaDirectLightShader(double _numSamples, double _Ia) :
    Shader(Vector3D(0.0)), numSamples(_numSamples), Ia(_Ia)
{}

Vector3D AreaDirectLightShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const {

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
                if (dot(wi, normal) > 0) {
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
            Vector3D Lemitted(0.0);
            Vector3D Ldir(0.0);
            Vector3D Lindir(0.0);

            if (its.shape->getMaterial().isEmissive() == true) {
                Lemitted = its.shape->getMaterial().getEmissiveRadiance();
            }

            Intersection its2;
            Vector3D Li;
            Vector3D sampleColor(0.0);

            Vector3D Le;
            Vector3D Fr(0.0);
            Vector3D wjn(0.0);
            Vector3D wj(0.0);

            Vector3D y(0.0);
            double V = 0.0;
            double G = 0.0;

            double Gup = 0.0;

            Vector3D hemidir(0.0);
            Ray rayLi;
            
            
            for (int i = 0; i < lsList.size(); i++) {
                if (lsList[i]->getArea() == 0.0) {
                    // Se tiene que tener en cuenta¿
                    continue;
                }
                AreaLightSource* areaLight = (AreaLightSource*) lsList[i];
                
                for (int j = 0; j < numSamples; j++) {
                    y = areaLight->generateRandomPoint();
                    
                    wj = (y - its.itsPoint).normalized();
                    Vector3D wjdir = y - its.itsPoint;

                    Le = areaLight->getIntensity();

                    Fr = its.shape->getMaterial().getReflectance(its.normal, wj, r.d);

                    rayLi = Ray(its.itsPoint, wj, r.depth + 1, 0.0001, wjdir.length());
                    if (Utils::getClosestIntersection(rayLi, objList, its2)) {
                        
                        V = 0.0;
                    }
                    else {
                        V = 1.0;
                    }

                    Gup = dot(its.normal, wj) * dot(-wj, areaLight->getNormal());

                    G = Gup / pow(wjdir.length(), 2);
                    

                    double prob = 1.0 / (areaLight->getArea());
                    //prob = 1.0;

                    sampleColor += (Le * Fr * G * V) / prob;
                }
            }
            
            Ldir = sampleColor / numSamples;

            Lindir = its.shape->getMaterial().getDiffuseReflectance() * Ia;

            color = Lemitted + Ldir + Lindir;


        }

    }
    return Vector3D(Utils::clamp(color.x, 1, 0), Utils::clamp(color.y, 1, 0), Utils::clamp(color.z, 1, 0));
}