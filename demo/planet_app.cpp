#include "pch.hpp"
#include "planet_app.hpp"
#include "Factories/VkMeshFactory.hpp"
#include "Materials/mesh_material.hpp"
#include "Factories/VkImageFactory.hpp"

GPlanetApplication::GPlanetApplication()
	: IGApplication(SWindowParameters{ 1024, 720, "Procedural planet demo" })
{
	Scope = Window.GetRenderer().GetScope();
	engineListener.SetUserPointer(this);
	Window.SetUpEvents(engineListener);

	engineListener.SubscribeKeyPressEvent(this, &GPlanetApplication::_keyPress);
	engineListener.SubscribeMouseMoveEvent(this, &GPlanetApplication::_mouseMove);
	engineListener.SubscribeMousePressEvent(this, &GPlanetApplication::_mousePress);

	MaterialDescriptor MDescriptor;
	MDescriptor.SubmeshTextures.push_back(TexturePack{ .Albedo = GVkImageFactory::SolidColor(Scope, COLOR(255, 0,   0))   });
	MDescriptor.SubmeshTextures.push_back(TexturePack{ .Albedo = GVkImageFactory::SolidColor(Scope, COLOR(0,   255, 0))   });
	MDescriptor.SubmeshTextures.push_back(TexturePack{ .Albedo = GVkImageFactory::SolidColor(Scope, COLOR(0,   0,   255)) });
	MDescriptor.SubmeshTextures.push_back(TexturePack{ .Albedo = GVkImageFactory::SolidColor(Scope, COLOR(255, 255, 0))   });
	MDescriptor.SubmeshTextures.push_back(TexturePack{ .Albedo = GVkImageFactory::SolidColor(Scope, COLOR(255, 0,   255)) });
	MDescriptor.SubmeshTextures.push_back(TexturePack{ .Albedo = GVkImageFactory::SolidColor(Scope, COLOR(0,   255, 255)) });
	// MDescriptor.PolygonMode = VK_POLYGON_MODE_LINE;
	// MDescriptor.CullMode = VK_CULL_MODE_NONE;

	GDrawable testCube{};
	testCube.Mesh = GVkMeshFactory::Cube(Scope, 150, 150, 150, 4, 4, true);
	testCube.Material = GMeshMaterial::Create(Scope, MDescriptor);
	testCube.WorldMatrix.Translate(Camera.GetWorldMatrix().GetForward() * 300.0);
	world.Add(std::move(testCube));
}

GPlanetApplication::~GPlanetApplication()
{
}

void GPlanetApplication::_mouseMove(GEvents::MousePosition Event, void* Data)
{
	if (mouseStates[GEnums::EMouse::Left] == GEnums::EAction::Press)
	{
		const double Mult = 0.01;
		Camera.GetWorldMatrix().Rotate(Event.delta_y, -Event.delta_x, 0.0);
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
	const float speed = 1000.0;

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