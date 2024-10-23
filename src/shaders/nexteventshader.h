#pragma once
#include "shader.h"
class NextEventShader : public Shader
{
	public:
		NextEventShader();
		NextEventShader(double _numSamples, double _maximumDepth);

		virtual Vector3D computeColor(const Ray& r,
			const std::vector<Shape*>& objList,
			const std::vector<LightSource*>& lsList) const;

		Vector3D reflectedRadiance(const Intersection its, const Ray r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList, int maxDepth) const;

		Vector3D directRadiance(const Intersection its, const Ray r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const;
		Vector3D indirectRadiance(const Intersection its, const Ray r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList, int maxDepth) const;

		double numSamples;
		double maximumDepth;
};


