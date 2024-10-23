#include "arealightsource.h"
#include <algorithm>
#include <cstdlib> 
#include <ctime>   

AreaLightSource::AreaLightSource(Square* areaLightsource_) :
    myAreaLightsource(areaLightsource_)
{ }



Vector3D AreaLightSource::getIntensity() const
{
    return myAreaLightsource->getMaterial().getEmissiveRadiance();
}


Vector3D AreaLightSource::sampleLightPosition()   const
{
    return this->myAreaLightsource->corner;
}

Vector3D AreaLightSource::generateRandomPoint() const {

    double rand1 = (double)std::rand() / RAND_MAX;
    double rand2 = (double)std::rand() / RAND_MAX;

    Vector3D p1 = this->myAreaLightsource->v1 + this->myAreaLightsource->corner;
    Vector3D p2 = this->myAreaLightsource->v2 + this->myAreaLightsource->corner;
    Vector3D corner = this->myAreaLightsource->corner;
    Vector3D otherCorner = this->myAreaLightsource->corner + this->myAreaLightsource->v1 + this->myAreaLightsource->v2;

    Vector3D points[4] = { p1, p2, corner, otherCorner };

    double maxX = p1.x;
    double minX = p1.x;

    double maxZ = p1.z;
    double minZ = p1.z;

    for (int i = 0; i < 4; i++) {
        if (maxX < points[i].x) {
            maxX = points[i].x;
        }
        if (minX > points[i].x) {
            minX = points[i].x;
        }
        if (maxZ < points[i].z) {
            maxZ = points[i].z;
        }
        if (minZ > points[i].z) {
            minZ = points[i].z;
        }
    }



    double random_numberX = minX + rand1 * (maxX - minX);
    double random_numberZ = minZ + rand2  * (maxZ - minZ);



    return Vector3D(random_numberX, 3.0, random_numberZ);
    //return Vector3D(0.0, 3.0, 4.0);
}
