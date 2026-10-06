#include "planet_world.hpp"

void GPlanetWorld::Add(GDrawable&& object)
{
	_drawableObjects.push_back(std::move(object));
}