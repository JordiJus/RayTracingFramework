#ifndef DIRECTLIGHTSHADER_H
#define DIRECTLIGHTSHADER_H

#include "shader.h"

class HemisphericalDirectLightShader : public Shader
{
	public:
		HemisphericalDirectLightShader();
		HemisphericalDirectLightShader(double _numSamples, double _Ia);
		virtual Vector3D computeColor(const Ray& r,
			const std::vector<Shape*>& objList,
			const std::vector<LightSource*>& lsList) const;

		double numSamples;
		double Ia;
};



#endif
/*
*/