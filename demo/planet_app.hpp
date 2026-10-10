#pragma once
#include "Engine/application.hpp"
#include "Engine/event_listener.hpp"
#include "planet_world.hpp"
#include <map>
#include "chunks.hpp"
#include "Factories/VkMeshFactory.hpp"
#include "Materials/mesh_material.hpp"
#include "Factories/VkImageFactory.hpp"

class GPlanetApplication : public IGApplication
{
private:

public:
	std::map<GEnums::EMouse, GEnums::EAction> mouseStates;
	std::map<GEnums::EKey, GEnums::EAction> keyStates;
	std::shared_ptr<RenderScope> Scope;

	GEventListener engineListener;
	GPlanetWorld world;
	ChunkAtlass atlass;

protected:
	void _mouseMove(GEvents::MousePosition Event, void* Data);
	void _mousePress(GEvents::MousePress Event, void* Data);
	void _keyPress(GEvents::KeyPress Event, void* Data);

protected:
	void _updateCamera(float Delta);
	void Update(float Delta) override;

public:
	GPlanetApplication();
	virtual ~GPlanetApplication();

	const IWorld& GetWorld() const override
	{
		return world;
	}
};