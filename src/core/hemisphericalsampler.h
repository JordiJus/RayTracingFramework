#ifndef HEMISPHERICALSAMPLER_H
#define HEMISPHERICALSAMPLER_H

#include "../core/vector3d.h"

using namespace std;


class HemisphericalSampler
{
public:
    HemisphericalSampler();
    Vector3D getSample(const Vector3D &normal) const;
    //Vector3D getSample_OMP(const Vector3D &normal, const double rand_numbers[], int idx, int n_spp) const;
};

#endif // HEMISPHERICALSAMPLER_H
