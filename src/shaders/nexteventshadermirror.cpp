#include "nexteventshadermirror.h"
#include "../core/utils.h"
#include "../core/hemisphericalsampler.h"

NextEventShaderMirror::NextEventShaderMirror() :
    Shader(Vector3D(0.0))
{}

NextEventShaderMirror::NextEventShaderMirror(double _numSamples, double _maximumDepth) :
    Shader(Vector3D(0.0)), numSamples(_numSamples), maximumDepth(_maximumDepth)
{}

Vector3D NextEventShaderMirror::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
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
                rayTra.rayType = TRANSMISSIVE;
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
                rayVis.rayType = REFLECTIVE;
                color += computeColor(rayVis, objList, lsList);
                return color;
            }

        }

        else if (its.shape->getMaterial().hasSpecular() == true) {
            Vector3D wr = Utils::computeReflectionDirection(-r.d, its.normal);
            Ray rayVis = Ray(its.itsPoint, wr.normalized(), r.depth + 1);
            rayVis.rayType = REFLECTIVE;
            color += computeColor(rayVis, objList, lsList);

        } else if (its.shape->getMaterial().isEmissive() == true) {
            color = its.shape->getMaterial().getEmissiveRadiance();
        }

        else {
            Vector3D Lemitted(0.0);
            Vector3D Lrad(0.0);

            if (its.shape->getMaterial().isEmissive() == true) {
                Lemitted = its.shape->getMaterial().getEmissiveRadiance();
            }


            Intersection its2;
            HemisphericalSampler hs = HemisphericalSampler();
            Vector3D Li;
            Vector3D Lrad_sample(0.0);

            Vector3D Fr(0.0);
            Vector3D wjn(0.0);

            Vector3D hemidir(0.0); // Dirección del sample hemisferio
            Ray rayLi; // Rayo del hemisferio

           

            
            for (int j = 0; j < numSamples; j++) {

                Lrad_sample += reflectedRadiance(its, r, objList, lsList, maximumDepth);

            }
            
            Lrad = Lrad_sample / numSamples;

            color = Lemitted + Lrad;


        }

    }
    return Vector3D(Utils::clamp(color.x, 1, 0), Utils::clamp(color.y, 1, 0), Utils::clamp(color.z, 1, 0));
}

Vector3D NextEventShaderMirror::reflectedRadiance(const Intersection its, const Ray r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList, int maxDepth) const
{
    Vector3D Ldir(0.0);
    Vector3D Lindir(0.0);
   
    Ldir = directRadiance(its, r, objList, lsList);
    Lindir = indirectRadiance(its, r, objList, lsList, maxDepth);
    //Lindir = its.shape->getMaterial().getDiffuseReflectance() * 0.1;

    return Ldir + Lindir;
}

Vector3D NextEventShaderMirror::directRadiance(const Intersection its, const Ray r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{


    Intersection its2;
    Ray rayLi;

    Vector3D V(0.0);
    Vector3D G(0.0);
    Vector3D Gup(0.0);

    Vector3D Le(0.0);
    Vector3D y(0.0);
    double pdf = 0;
    Vector3D wj(0.0);
    Vector3D Ldir(0.0);
    Vector3D Fr(0.0);
    AreaLightSource* areaLight;
    Vector3D wjdir(0.0);
    if (r.rayType != DIFFUSE) {
        if (its.shape->getMaterial().isEmissive()) {
            return its.shape->getMaterial().getEmissiveRadiance();
        }
    }
    
    for (int i = 0; i < lsList.size(); i++) {
        if (lsList[i]->getArea() == 0.0) {
            // Se tiene que tener en cuenta¿
            continue;
        }
        areaLight = (AreaLightSource*)lsList[i];


        y = areaLight->generateRandomPoint();

        wj = (y - its.itsPoint).normalized();
        wjdir = y - its.itsPoint;

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

        Ldir += (Le * Fr * G * V) / prob;

    }
    return Ldir;
}

Vector3D NextEventShaderMirror::indirectRadiance(const Intersection its, const Ray r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList, int maxDepth) const
{
    Intersection its2;
    Vector3D Lo(0.0);
    if (r.rayType != DIFFUSE) {
        Lo += its.shape->getMaterial().getEmissiveRadiance();
    }

    if (its.shape->getMaterial().hasSpecular()) {
        Vector3D wr = Utils::computeReflectionDirection(r.d, its.normal);
        Ray rayVis = Ray(its.itsPoint, wr.normalized(), r.depth + 1);
        rayVis.rayType = REFLECTIVE;
        if (Utils::getClosestIntersection(rayVis, objList, its2)) {
            Lo += indirectRadiance(its2, rayVis, objList, lsList, maxDepth);
        }
    } else if (its.shape->getMaterial().hasTransmission() == true) {

        Vector3D wt;
        Vector3D normal = its.normal.normalized();
        Vector3D wi = r.d;
        double ratio = 0.7;



        if (!Utils::isTotalInternalReflection(normal, wi, its.shape->getMaterial().getIndexOfRefraction())) {
            Vector3D wt = its.shape->getMaterial().getReflectance(normal, Vector3D(0.0), wi);
            Ray rayTra = Ray(its.itsPoint, wt.normalized(), r.depth + 1);
            rayTra.rayType = TRANSMISSIVE;
            if (Utils::getClosestIntersection(rayTra, objList, its2)) {
                Lo += indirectRadiance(its2, rayTra, objList, lsList, maxDepth);
            }
        }

        else {
            //Vector3D wr = its.shape->getMaterial().getReflectance(its.normal, Vector3D(0.0), -r.d);
            if (dot(wi, normal) > 0) {
                ratio = 1 / ratio;
                normal = -normal;
            }
            Vector3D wr = Utils::computeReflectionDirection(wi, normal);
            Ray rayVis = Ray(its.itsPoint, wr.normalized(), r.depth + 1);
            rayVis.rayType = REFLECTIVE;
            if (Utils::getClosestIntersection(rayVis, objList, its2)) {
                Lo += indirectRadiance(its2, rayVis, objList, lsList, maxDepth);
            }
        }

    }
    else {
        HemisphericalSampler hs = HemisphericalSampler();
        Vector3D wi = hs.getSample(its.normal).normalized();

        if (r.depth < maxDepth) {
            Ray newR = Ray(its.itsPoint, wi, r.depth + 1);
            if (Utils::getClosestIntersection(newR, objList, its2)) {
                Lo += (reflectedRadiance(its2, newR, objList, lsList, maxDepth) * its.shape->getMaterial().getReflectance(its.normal, wi, -r.d) * dot(its.normal, wi)) / (1 / (2 * 3.14));
            }

        }

    }

   

    return Lo;
    

    
}
/*
Vector3D NextEventShader::reflectedRadiance(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList, int maxDepth) const {
    Vector3D Ldir(0.0);
    Vector3D Lindir(0.0);

    Ldir = directRadiance(r, objList, lsList);
    Lindir = indirectRadiance(r, objList, lsList, maxDepth);

    return Ldir + Lindir;
}

Vector3D NextEventShader::directRadiance(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    Intersection its;
    Intersection its2;

    Vector3D Lemitted(0.0);
    Vector3D y(0.0);
    Vector3D wj(0.0);
    Vector3D Le(0.0);
    Vector3D Fr(0.0);
    Ray rayLi;
    Vector3D V(0.0);
    Vector3D Gup(0.0);
    Vector3D G(0.0);
    Vector3D sampleColor(0.0);
    
    if (Utils::getClosestIntersection(r, objList, its)) {
        for (int i = 0; i < lsList.size(); i++) {
            if (lsList[i]->getArea() == 0.0) {
                // Se tiene que tener en cuenta¿
                continue;
            }
            AreaLightSource* areaLight = (AreaLightSource*)lsList[i];

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

    
    return sampleColor;
}

Vector3D NextEventShader::indirectRadiance(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList, int maxDepth) const
{
    Intersection its;
    if (Utils::getClosestIntersection(r, objList, its)) {

        Vector3D Lo = its.shape->getMaterial().getEmissiveRadiance();

        if (its.shape->getMaterial().isEmissive()) {
            return Lo;
        }
        else if (its.shape->getMaterial().hasSpecular()) {
            Vector3D wr = Utils::computeReflectionDirection(-r.d, its.normal);
            Ray rayVis = Ray(its.itsPoint, wr.normalized(), r.depth + 1);
            Lo = reflectedRadiance(rayVis, objList, lsList, maxDepth);
        }
        else if (its.shape->getMaterial().hasTransmission() == true) {

            Vector3D wt;
            Vector3D normal = its.normal.normalized();
            Vector3D wi = r.d;
            double ratio = 0.7;



            if (!Utils::isTotalInternalReflection(normal, wi, its.shape->getMaterial().getIndexOfRefraction())) {
                Vector3D wt = its.shape->getMaterial().getReflectance(normal, Vector3D(0.0), wi);
                Ray rayTra = Ray(its.itsPoint, wt.normalized(), r.depth + 1);
                Lo = reflectedRadiance(rayTra, objList, lsList, maxDepth);
            }

            else {
                //Vector3D wr = its.shape->getMaterial().getReflectance(its.normal, Vector3D(0.0), -r.d);
                if (dot(wi, normal) > 0) {
                    ratio = 1 / ratio;
                    normal = -normal;
                }
                Vector3D wr = Utils::computeReflectionDirection(wi, normal);
                Ray rayVis = Ray(its.itsPoint, wr.normalized(), r.depth + 1);
                Lo = reflectedRadiance(rayVis, objList, lsList, maxDepth);
            }
                
        }
        else {
            HemisphericalSampler hs = HemisphericalSampler();
            Vector3D wi = hs.getSample(its.normal).normalized();

            if (r.depth < maxDepth) {
                Ray newR = Ray(its.itsPoint, wi, r.depth + 1);
                Lo = (reflectedRadiance(newR, objList, lsList, maxDepth) * its.shape->getMaterial().getReflectance(its.normal, wi, -newR.d) * dot(its.normal, wi)) / (1 / (2 * 3.14));
            }
        }
        return Lo;
    }

    else {
        return Vector3D(0.0, 0.0, 0.0);
    }
}
*/