#pragma once
#include "Engine/world.hpp"
#include <entt/entt.hpp>
#include "actor.hpp"

class GPlanetWorld : public IWorld
{
	mutable std::vector<GDrawable> _drawableObjects;
	entt::registry _registry;

public:
	void GetDrawableObjects(std::vector<std::pair<const GDrawable*, const EllipsoidMatrix*>>& objects) const override;
	std::shared_ptr<GActor> NewActor();
};