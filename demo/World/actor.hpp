#pragma once
#include "Math/PlanetoidMatrix.hpp"
#include <entt/entt.hpp>
#include "glm/glm.hpp"

class GActor
{
private:
	entt::registry& _registry;
	entt::entity _id;

public:
	GActor(entt::registry& registry);
	~GActor();

	template<typename T, typename ...Args>
	inline void AddComponent(Args&&... args)
	{
		_registry.emplace_or_replace<T>(_id, args...);
	}

	template<typename T>
	inline void DeleteComponent()
	{
		_registry.erase<T>(_id);
	}

	template<typename T>
	inline T& GetComponent()
	{
		return _registry.get<T>(_id);
	}

	EllipsoidMatrix& GetTransform();

	void SetPosition(const glm::dvec3& offset);
	void SetPosition(double x, double y, double z);

	void Translate(const glm::dvec3& offset);
	void Translate(double x, double y, double z);

	void Rotate(double P, double Y, double R);
};