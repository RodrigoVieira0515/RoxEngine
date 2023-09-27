#include "VImGuiLayer.h"
#ifdef  USE_IMGUI

#include <Platform/Vulkan/VRendererApi.h>

//#define IMGUI_DEFINE_MATH_OPERATORS
//#include <imgui/backends/imgui_impl_glfw.h>
//#include <imgui/backends/imgui_impl_vulkan.h>

#include <imgui/imgui.cpp>
#include <imgui/imgui_draw.cpp>
#include <imgui/imgui_widgets.cpp>
#include <imgui/imgui_tables.cpp>
#include <imgui/misc/cpp/imgui_stdlib.cpp>
#include <imgui/backends/imgui_impl_glfw.cpp>
#include <imgui/backends/imgui_impl_vulkan.cpp>


#include <GLFW/glfw3.h>

#include <RoxEngine/core/Logger.h>
#include <iostream>

namespace RoxEngine::Vulkan {
	void check_vk_result(VkResult err)
	{
		if (err == 0)
			return;
		
		RE_CORE_ERROR("[vulkan] Error: VkResult = {}", err);

		if (err < 0) {
			exit(-1);
		}
	}
	static std::shared_ptr<RendererApi> sApi;
	static vk::DescriptorPool descriptorPool;
	void ImGuiLayer::Init()
	{
		sApi = std::dynamic_pointer_cast<RendererApi>(RendererApi::Get());

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		auto descriptorPoolSize = vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, 1);
		vk::DescriptorPoolCreateInfo dp_info({}, 1,1, &descriptorPoolSize);

		descriptorPool = sApi->mDevice.createDescriptorPool(dp_info);

		ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)sApi->mWindow->GetNativeWindow(), true);
		ImGui_ImplVulkan_InitInfo info = {};
		info.Instance = sApi->mInstance;
		info.PhysicalDevice = sApi->mPhysicalDevice;
		info.Device = sApi->mDevice;
		info.QueueFamily = sApi->mQueueFamilyIndices.graphicsFamily.value();
		info.Queue = sApi->mGraphicsQueue;
		info.PipelineCache = nullptr;
		info.DescriptorPool = descriptorPool;
		info.Subpass = 0;
		info.MinImageCount = static_cast<uint32_t>(sApi->mSwapchainImages.size());
		info.ImageCount = static_cast<uint32_t>(sApi->mSwapchainImages.size());
		info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		info.Allocator = nullptr;
		info.CheckVkResultFn = check_vk_result;
		ImGui_ImplVulkan_Init(&info, sApi->mRenderPass);

		// upload font
		{
			sApi->mDevice.resetCommandPool(sApi->mCommandPool);
			vk::CommandBufferBeginInfo begininfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
			sApi->mCommandBuffers[0].begin(begininfo);
			ImGui_ImplVulkan_CreateFontsTexture(sApi->mCommandBuffers[0]);
			sApi->mCommandBuffers[0].end();
			vk::SubmitInfo submit_info(nullptr, nullptr, sApi->mCommandBuffers[0], nullptr);
			sApi->mGraphicsQueue.submit(submit_info);
			sApi->mDevice.waitIdle();
			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}
	}
	void ImGuiLayer::Shutdown()
	{

		sApi.reset();
	}
	void ImGuiLayer::NewFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void ImGuiLayer::EndFrame()
	{
		ImGui::Render();
	}
}
#endif