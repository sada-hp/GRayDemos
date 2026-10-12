#include "planet_world.hpp"

void GPlanetWorld::GetDrawableObjects(std::vector<std::pair<const GDrawable*, const EllipsoidMatrix*>>& objects) const
{
	auto view = _registry.view<GDrawable, EllipsoidMatrix>();

	for (const auto& [ent, drawable, matrix] : view.each())
		objects.emplace_back(&drawable, &matrix);
}

std::shared_ptr<GActor> GPlanetWorld::NewActor()
{
	return std::make_shared<GActor>(_registry);
}