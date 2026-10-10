#pragma once
#include "Engine/world.hpp"

class GPlanetWorld : public IWorld
{
private:
	std::vector<GDrawable> _drawableObjects;

public:
	const std::vector<GDrawable>& GetDrawableObjects() const override
	{
		return _drawableObjects;
	}

	void Clear() override
	{
		_drawableObjects.clear();
	}

	void Add(GDrawable&& object);
};