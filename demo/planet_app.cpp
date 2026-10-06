#include "pch.hpp"
#include "planet_app.hpp"
#include "Factories/VkMeshFactory.hpp"
#include "Materials/mesh_material.hpp"

GPlanetApplication::GPlanetApplication()
	: IGApplication(SWindowParameters{ 1024, 720, "Procedural planet demo" })
{
	engineListener.SetUserPointer(this);
	Window.SetUpEvents(engineListener);

	engineListener.SubscribeKeyPressEvent(this, &GPlanetApplication::_keyPress);
	engineListener.SubscribeMouseMoveEvent(this, &GPlanetApplication::_mouseMove);
	engineListener.SubscribeMousePressEvent(this, &GPlanetApplication::_mousePress);

	GDrawable testCube{};
	testCube.Mesh = GVkMeshFactory::Plane(Window.GetRenderer().GetScope(), 150, 150);
	testCube.Material = GMeshMaterial::Create(Window.GetRenderer().GetScope());
	testCube.WorldMatrix.Translate(Camera.GetWorldMatrix().GetForward() * 100.0);

	world.Add(std::move(testCube));
}

GPlanetApplication::~GPlanetApplication()
{
}

void GPlanetApplication::_mouseMove(GEvents::MousePosition Event, void* Data)
{
	if (mouseStates[GEnums::EMouse::Left] == GEnums::EAction::Press)
	{
		const double Mult = 0.005;
		Camera.GetWorldMatrix().Rotate(Mult * Event.delta_y, Mult * -Event.delta_x, 0.0);
	}
}

void GPlanetApplication::_mousePress(GEvents::MousePress Event, void* Data)
{
	mouseStates[Event.key] = Event.action;
}

void GPlanetApplication::_keyPress(GEvents::KeyPress Event, void* Data)
{
	keyStates[Event.key] = Event.action;
}

void GPlanetApplication::_updateCamera(float Delta)
{
	glm::vec3 translation(0.0);
	const float speed = 5000.0;

	if (keyStates[GEnums::EKey::W] != GEnums::EAction::Release)
		translation.z += speed * Delta;

	if (keyStates[GEnums::EKey::S] != GEnums::EAction::Release)
		translation.z -= speed * Delta;

	if (keyStates[GEnums::EKey::A] != GEnums::EAction::Release)
		translation.x += speed * Delta;

	if (keyStates[GEnums::EKey::D] != GEnums::EAction::Release)
		translation.x -= speed * Delta;

	if (keyStates[GEnums::EKey::PageUp] != GEnums::EAction::Release)
		translation.y += speed * Delta;

	if (keyStates[GEnums::EKey::PageDown] != GEnums::EAction::Release)
		translation.y -= speed * Delta;

	if (translation.x != 0.0 || translation.y != 0.0 || translation.z != 0.0)
		Camera.GetWorldMatrix().Translate(translation);
}

void GPlanetApplication::Update(float Delta)
{
	Window.SetTitle(("Procedural planet demo " + std::format("{:.1f}", 1.0 / Delta)).c_str());
	_updateCamera(Delta);
}