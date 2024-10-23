#include "purepathtracingshader.h"
#include "../core/hemisphericalsampler.h"
#include "../core/Utils.h"


PurePathTracingShader::PurePathTracingShader() :
    Shader(Vector3D(0.0))
{}

PurePathTracingShader::PurePathTracingShader(double _numSamples, double _maximumDepth) :
    Shader(Vector3D(0.0)), numSamples(_numSamples), maximumDepth(_maximumDepth)
{}

Vector3D PurePathTracingShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
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

        } else if(its.shape->getMaterial().isEmissive() == true) {
            return its.shape->getMaterial().getEmissiveRadiance();
        }

        else {
            Vector3D Ldir(0.0);

            

            Intersection its2;
            HemisphericalSampler hs = HemisphericalSampler();
            Vector3D Li;
            Vector3D Ldir_sample(0.0);

            Vector3D Fr(0.0);
            Vector3D wjn(0.0);

            Vector3D hemidir(0.0); // Dirección del sample hemisferio
            Ray rayLi; // Rayo del hemisferio

            for (int j = 0; j < numSamples; j++) {
                hemidir = hs.getSample(its.normal).normalized();

                rayLi = Ray(its.itsPoint, hemidir, r.depth + 1);
                

                Ldir_sample += (computeRadiance(rayLi, objList, maximumDepth) * its.shape->getMaterial().getReflectance(its.normal, hemidir, r.d) * dot(its.normal, hemidir)) / (1 / (2 * 3.14));

                

            }
            color = Ldir_sample / numSamples;

        }

    }
    return Vector3D(Utils::clamp(color.x, 1, 0), Utils::clamp(color.y, 1, 0), Utils::clamp(color.z, 1, 0));
}


Vector3D PurePathTracingShader::computeRadiance(const Ray& r, const std::vector<Shape*>& objList, int maxDepth) const {
    Intersection its;
    if (Utils::getClosestIntersection(r, objList, its)) {
        
        Vector3D Lo = its.shape->getMaterial().getEmissiveRadiance();

        if (its.shape->getMaterial().isEmissive()) {
            return Lo;
        }
        else if (its.shape->getMaterial().hasSpecular()) {
            Vector3D wr = Utils::computeReflectionDirection(-r.d, its.normal);
            Ray rayVis = Ray(its.itsPoint, wr.normalized(), r.depth + 1);
            Lo += computeRadiance(rayVis, objList, maxDepth);
        }
        else if (its.shape->getMaterial().hasTransmission() == true) {

            Vector3D wt;
            Vector3D normal = its.normal.normalized();
            Vector3D wi = r.d;
            double ratio = 0.7;



            if (!Utils::isTotalInternalReflection(normal, wi, its.shape->getMaterial().getIndexOfRefraction())) {
                Vector3D wt = its.shape->getMaterial().getReflectance(normal, Vector3D(0.0), wi);
                Ray rayTra = Ray(its.itsPoint, wt.normalized(), r.depth + 1);
                Lo += computeRadiance(rayTra, objList, maxDepth);
            }

            else {
                //Vector3D wr = its.shape->getMaterial().getReflectance(its.normal, Vector3D(0.0), -r.d);
                if (dot(wi, normal) > 0) {
                    ratio = 1 / ratio;
                    normal = -normal;
                }
                Vector3D wr = Utils::computeReflectionDirection(wi, normal);
                Ray rayVis = Ray(its.itsPoint, wr.normalized(), r.depth + 1);
                Lo += computeRadiance(rayVis, objList, maxDepth);
            }

        }
        else {
            HemisphericalSampler hs = HemisphericalSampler();
            Vector3D wi = hs.getSample(its.normal).normalized();

            if (r.depth < maxDepth) {
                Ray newR = Ray(its.itsPoint, wi, r.depth + 1);
                Lo += (computeRadiance(newR, objList, maxDepth) * its.shape->getMaterial().getReflectance(its.normal, wi, -newR.d) * dot(its.normal, wi)) / (1 / (2 * 3.14));
            }
        }
        return Lo;
    }
    
    else {
        return Vector3D(0.0, 0.0, 0.0);
    }
    
    
}