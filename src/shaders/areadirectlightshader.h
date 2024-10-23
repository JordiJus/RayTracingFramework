#include "shader.h"
class AreaDirectLightShader : public Shader
{
public:
	AreaDirectLightShader();
	AreaDirectLightShader(double _numSamples, double _Ia);
	virtual Vector3D computeColor(const Ray& r,
		const std::vector<Shape*>& objList,
		const std::vector<LightSource*>& lsList) const;

	double numSamples;
	double Ia;
};
