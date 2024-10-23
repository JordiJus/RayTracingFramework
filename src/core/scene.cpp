#include "scene.h"
#include "../lightsources/arealightsource.h"

Scene::Scene()
{
	objectsList = new std::vector<Shape*>;
	LightSourceList = new std::vector<LightSource*>;

}

void Scene::AddObject(Shape* new_object)
{
	objectsList->push_back(new_object);
	if (new_object->getMaterial().isEmissive())
		LightSourceList->push_back(new AreaLightSource(dynamic_cast<Square*>(new_object)));

}	

void Scene::AddPointLight(PointLightSource* new_pointLight)
{
	LightSourceList->push_back(new_pointLight);
}

