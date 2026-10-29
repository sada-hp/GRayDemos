#include "actor.hpp"
#include "Math/PlanetoidMatrix.hpp"

GActor::GActor(entt::registry& registry)
	: _registry(registry)
{
	_id = _registry.create();
	_registry.emplace<EllipsoidMatrix>(_id);
}

GActor::~GActor()
{
	_registry.destroy(_id);
}

EllipsoidMatrix& GActor::GetTransform()
{
	return GetComponent<EllipsoidMatrix>();
}

void GActor::SetPosition(const glm::dvec3& offset)
{
	GetTransform().SetPosition(offset);
}

void GActor::SetPosition(double x, double y, double z)
{
	GetTransform().SetPosition({ x, y, z });
}

void GActor::Rotate(double P, double Y, double R)
{
	GetTransform().Rotate(P, Y, R);
}

void GActor::Translate(const glm::dvec3& offset)
{
	GetTransform().Translate(offset);
}

void GActor::Translate(double x, double y, double z)
{
	GetTransform().Translate({x, y, z});
}