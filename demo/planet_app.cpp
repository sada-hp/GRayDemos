#include "planet_app.hpp"
#include "UI/imgui_ui.hpp"

#define IsNotReleased(key) keyStates[key] != EAction::Release
#define IsJustPressed(key) keyStates[key] == EAction::Press

using namespace GEnums;

GPlanetApplication::GPlanetApplication()
	: IGApplication(SWindowParameters{ 1024, 720, "Procedural planet demo" })
{
	Scope = Window.GetRenderer().GetScope();
	engineListener.SetUserPointer(this);
	Window.SetUpEvents(engineListener);
	Window.GetRenderer().AddRenderStage<GUI>(Window);

	engineListener.SubscribeKeyPressEvent(this, &GPlanetApplication::_keyPress);
	engineListener.SubscribeScrollEvent(this, &GPlanetApplication::_wheelScroll);
	engineListener.SubscribeMouseMoveEvent(this, &GPlanetApplication::_mouseMove);
	engineListener.SubscribeMousePressEvent(this, &GPlanetApplication::_mousePress);

	world._initDebug(Scope);

	_debugCube = world.NewActor();
	_debugCube->AddComponent<GDrawable>(GVkMeshFactory::Cube(Scope, 100, 100, 100), GMeshMaterial::Create(Scope));
	_debugCube->Translate(-glm::dvec3(100, 0, 400));
}

GPlanetApplication::~GPlanetApplication()
{
}

void GPlanetApplication::_wheelScroll(GEvents::ScrollDelta Event, void* Data)
{
	camera_speed += Event.y * 100.0;
}

void GPlanetApplication::_mouseMove(GEvents::MousePosition Event, void* Data)
{
	if (mouseStates[GEnums::EMouse::Left] == GEnums::EAction::Press && !GUI::IsFocused())
	{
		const double Mult = 0.3;
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
	
	if (IsNotReleased(EKey::W))
		translation.z += camera_speed * Delta;

	if (IsNotReleased(EKey::S))
		translation.z -= camera_speed * Delta;

	if (IsNotReleased(EKey::A))
		translation.x += camera_speed * Delta;

	if (IsNotReleased(EKey::D))
		translation.x -= camera_speed * Delta;

	if (IsNotReleased(EKey::E) || IsNotReleased(EKey::Space))
		translation.y += camera_speed * Delta;

	if (IsNotReleased(EKey::Q) || IsNotReleased(EKey::LeftControl))
		translation.y -= camera_speed * Delta;

	if (translation.x != 0.0 || translation.y != 0.0 || translation.z != 0.0)
		Camera.GetWorldMatrix().Translate(translation);
}

void GPlanetApplication::Update(float Delta)
{
	Window.SetTitle(("Procedural planet demo " + std::format("{:.1f}", 1.0 / Delta)).c_str());
	_updateCamera(Delta);

	Camera.GetWorldMatrix().LookAt(_debugCube->GetTransform().GetPosition());

	// world.updateChunks(Scope, Camera);
}