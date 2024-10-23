#include "hemisdirectlightshader.h"
#include "../core/hemisphericalsampler.h"
#include "../core/Utils.h"

HemisphericalDirectLightShader::HemisphericalDirectLightShader():
	Shader(Vector3D(0.0))
{}

HemisphericalDirectLightShader::HemisphericalDirectLightShader(double _numSamples, double _Ia) :
    Shader(Vector3D(0.0)), numSamples(_numSamples), Ia(_Ia)
{}

Vector3D HemisphericalDirectLightShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
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
            HemisphericalSampler hs = HemisphericalSampler();
            
            Vector3D sampleColor(0.0);

            Vector3D Li(0.0);
            Vector3D Fr(0.0);
            Vector3D wjn(0.0);

            Vector3D hemidir(0.0);
            Ray rayLi;
            double prob = 0.0;
                

            for (int j = 0; j < numSamples; j++) {
                hemidir = hs.getSample(its.normal).normalized();

                   
                rayLi = Ray(its.itsPoint, hemidir, r.depth + 1);
                if (Utils::getClosestIntersection(rayLi, objList, its2)) {
                    Li = its2.shape->getMaterial().getEmissiveRadiance();
                 
                }
                else {
                    Li = Vector3D(0.0);
                }

                Fr = its.shape->getMaterial().getReflectance(its.normal, hemidir, r.d);


                wjn = dot(hemidir, its.normal);
                    
                prob = 1.0 / (2.0 * 3.14);
                //prob = 1.0;
                    
                sampleColor += (Li * Fr * wjn) / prob;
                //std::cout << sampleColor;
            }
            Ldir = sampleColor / numSamples;

                
           
            
            Lindir = its.shape->getMaterial().getDiffuseReflectance() * Ia;

            color = Lemitted + Ldir + Lindir;


        }

    }
    return Vector3D(Utils::clamp(color.x, 1, 0), Utils::clamp(color.y, 1, 0), Utils::clamp(color.z, 1, 0));
}
