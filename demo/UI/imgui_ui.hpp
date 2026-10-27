#pragma once
#include "imgui/imgui.h"
#include "World/planet_world.hpp"
#include "Engine/window.hpp"
#include "VulkanAPI/scope.hpp"
#include "VulkanAPI/command_buffer.hpp"
#include "RenderStages/IRenderStage.hpp"

class GUI : public IRenderStage
{
	std::shared_ptr<RenderScope> Scope = VK_NULL_HANDLE;
	ImGuiContext* ImContext = nullptr;

private:
	void _weatherControls(IWorld& World);
	void _cameraPos(const GCamera& Camera, const GPlanetWorld& World);
public:
	GUI(std::shared_ptr<RenderScope> Scope, const GVkSharedResources& Resources, GWindow& Window);
	~GUI();

	void Execute(const GCamera& Camera, const IWorld& World) override;

	static bool IsFocused()
	{
		return ImGui::IsAnyItemFocused() || ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive();
	}
};