#pragma once
#include "UI/imgui_ui.hpp"
#include "Engine/application.hpp"
#include "Engine/event_listener.hpp"
#include "World/planet_world.hpp"
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

	std::shared_ptr<IMaterial> _debugMaterial;

	GEventListener engineListener;
	GPlanetWorld world;
	ChunkAtlass atlass;

	std::shared_ptr<GActor> _debugCube;
	float camera_speed = 1000.f;

protected:
	void _wheelScroll(GEvents::ScrollDelta Event, void* Data);
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