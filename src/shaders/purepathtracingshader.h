#include "shader.h"

class PurePathTracingShader : public Shader
{
	public:
		PurePathTracingShader();
		PurePathTracingShader(double _numSamples, double _maximumDepth);
		virtual Vector3D computeColor(const Ray& r,
			const std::vector<Shape*>& objList,
			const std::vector<LightSource*>& lsList) const;

		Vector3D computeRadiance(const Ray& r, const std::vector<Shape*>& objList, int maxDepth) const;

		double numSamples;
		double maximumDepth;
};