#include "imgui_ui.hpp"

#include "imgui/imgui_impl_vulkan.h"
#include "imgui/imgui_impl_glfw.h"
#include "planet_app.hpp"

GUI::GUI(std::shared_ptr<RenderScope> Scope, const GVkSharedResources &Resources, GWindow &Window)
	: IRenderStage(Scope, VK_QUEUE_GRAPHICS_BIT)
{
	ImContext = ImGui::CreateContext();
	ImGui_ImplGlfw_InitForVulkan(Window.PlatformWindow(), true);

	RenderPassDescriptor RPDesc{};
	RPDesc.AddAttachmentLoadOp(Resources.FinalTarget->GetFormat(), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	RenderPass = RPDesc.Construct(Scope);

	Framebuffer = std::make_shared<GVkFramebuffer>(Scope, RenderPass, std::vector{Resources.FinalTarget});

	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.ApiVersion = VK_API_VERSION_1_3;
	init_info.Instance = Scope->GetInstance();
	init_info.PhysicalDevice = Scope->GetPhysicalDevice();
	init_info.Device = Scope->GetDevice();
	init_info.Queue = Scope->GetQueue(VK_QUEUE_GRAPHICS_BIT).GetQueue();
	init_info.DescriptorPoolSize = 512;
	init_info.MinImageCount = 3;
	init_info.ImageCount = 3;
	init_info.PipelineInfoMain.Subpass = 0;
	init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.PipelineInfoMain.RenderPass = RenderPass->GetRenderPass();
	ImGui_ImplVulkan_Init(&init_info);
}

GUI::~GUI()
{
	CommandBuffer->Wait();

	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext(ImContext);
}

void GUI::_weatherControls(IWorld &World)
{
	static float _sun = 0.16f;
	const glm::vec3 top = glm::normalize(glm::vec3(1.0, 1.0, 1.0));
	const glm::vec3 bot = glm::normalize(glm::vec3(1.0, 0.0, 0.0));

	ImGui::SliderFloat("Sun position", &_sun, 0.f, 1.f);
	World.SetSunDirection(glm::mix(bot, top, _sun));
}

void GUI::_cameraPos(const GCamera &Camera, const IWorld &World)
{
	auto cp = Camera.GetLocalMatrix().GetPosition();
	ImGui::Text("%3.f %3.f %3.f", cp.x, cp.y, cp.z);

	glm::dvec3 pos = Camera.GetWorldMatrix().GetPosition();

	glm::dvec3 globalVoxelPos = glm::floor((pos / static_cast<double>(CHUNK_ROW))) * static_cast<double>(CHUNK_ROW);
	glm::dvec3 globalChunkPos = glm::floor((pos / CHUNK_SIZE)) * CHUNK_SIZE;
	glm::dvec3 voxelPosToCheck = glm::floor((globalVoxelPos - globalChunkPos) / static_cast<double>(VOXEL_SIZE));
	ImGui::Text("%3.2f %3.2f %3.2f", voxelPosToCheck.x, voxelPosToCheck.y, voxelPosToCheck.z);
}

void GUI::Execute(const GCamera &Camera, const IWorld &World)
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Debug window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SetWindowPos(ImVec2{0, 0});
	_weatherControls(const_cast<IWorld &>(World));
	_cameraPos(const_cast<GCamera &>(Camera), const_cast<IWorld &>(World));
	ImGui::End();

	ImGui::Render();

	CommandBuffer->BeginRenderPass(RenderPass, Framebuffer);
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), CommandBuffer->GetCommandBuffer());
	CommandBuffer->Submit();
}