#pragma once
#include "UI/imgui_ui.hpp"
#include "Engine/application.hpp"
#include "Engine/event_listener.hpp"
#include "World/planet_world.hpp"
#include <map>

class GPlanetApplication : public IGApplication
{
public:
	std::map<GEnums::EMouse, GEnums::EAction> mouseStates;
	std::map<GEnums::EKey, GEnums::EAction> keyStates;
	std::shared_ptr<RenderScope> Scope;
	
	GPlanetWorld world;
	GEventListener engineListener;
	std::shared_ptr<GActor> TestActor;

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