#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>

#include "Mesh.h"
#include "Vertex.h"
#include "types/SmartPointers.h"
#include <optional>
#include <backends/imgui_impl_vulkan.h>

#define MAX_MESHES_ALLOWED 1000

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

namespace Revid
{
	struct SimpleVertex;
    struct RendererSettings
    {
        uint8_t MAX_FRAMES_IN_FLIGHT;
        String appName;
        const char** windowExtentions;
        uint32_t windowExtentionCount;
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        Vector<VkSurfaceFormatKHR> formats;
        Vector<VkPresentModeKHR> presentModes;
    };

    static void check_vk_result(VkResult err)
    {
        if (err == VK_SUCCESS)
            return;
        fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
        if (err < 0)
            abort();
    }

    class VulkanRenderer
    {
    public:
        void Init(const RendererSettings&);
        void Shutdown();
        void Render();
        void UpdateVertices(Vector<SimpleVertex>);
        void UpdateIndices(Vector<uint16_t>);
        void UpdateObj(String path);
        void AddMeshToScene(Ref<Mesh> mesh);
        VkDevice GetDeivce() const { return m_device; }
        void FramebufferResized() { m_framebufferResized = true; }

        ImGui_ImplVulkan_InitInfo GetInitInfo() const
        {
            ImGui_ImplVulkan_InitInfo init_info = {};
            init_info.Instance = m_instance;
            init_info.PhysicalDevice = m_physicalDevice;
            init_info.Device = m_device;
            init_info.QueueFamily = findQueueFamilies(m_physicalDevice).graphicsFamily.value();
            init_info.Queue = m_graphicsQueue;
            init_info.PipelineCache = VK_NULL_HANDLE;
            init_info.DescriptorPool = m_imguiDescriptorPool;
            init_info.MinImageCount = 3;
            init_info.ImageCount = 3;
            init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
            init_info.Allocator = nullptr;
            init_info.CheckVkResultFn = check_vk_result;
            init_info.Subpass = 1;
            init_info.RenderPass = m_renderPass;

            return init_info;
        }
        void CreateImguiDescriptorPool();


    private:
        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevices();
        void createSwapChain();
        void createGbufferImages();
        void createImageViews();
        void createRenderPass();
        void createDescriptorSetLayout();
        void createGbufferPipeline();
        void createLightingPipeline();
        void createFramebuffers();
        void createCommandPool();
        void createVertexBuffer();
        void createIndexBuffer();
        void addUniformBuffers();
        void updateGbufferDescriptorPool();
        void addGbufferDescriptorSets();
        void createLightingDescriptorPool();
        void createLightingDescriptorSets();
        void createDescriptorSetsForMeshes();
        void createCommandBuffer();
        void createSyncObjects();
        void recreateSwapChain();
        void cleanupSwapChain();

    private:
        bool checkValidationLayers();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

        VkResult createDebugUtilsMessengerEXT(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger);
        void destroyDebugUtilsMessengerEXT(
            VkInstance instance,
            VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks* pAllocator);
        void populateDebugMessengerCreateInfo(
            VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        VkShaderModule createShaderModule(const std::vector<char>& code);
        void recordCommandBuffer(const VkCommandBuffer& commandBuffer, uint32_t imageIndex);
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkFormat findDepthFormat();

    public:
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    private:
        void updateUniformBuffer(uint32_t currentImage, int mesh_index);
        void createImage(
            VkDevice device,
            uint32_t width,
            uint32_t height,
            VkFormat format,
            VkImageUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkImage& image,
            VkDeviceMemory& imageMemory);
        VkImageView createImageView(
            VkDevice device,
            VkImage image,
            VkFormat format,
            VkImageAspectFlags aspectFlags);

    private:
        bool m_framebufferResized = false;
#ifdef NDEBUG
        const bool m_enableValidationLayers = false;
#else
        const bool m_enableValidationLayers = true;
#endif
        uint8_t m_currentFrame = 0;

        VkInstance m_instance;
        VkDebugUtilsMessengerEXT m_debugMessenger;
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_device;
        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;
        VkSurfaceKHR m_surface;
		VkSwapchainKHR m_swapChain;

        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;

        Vector<VkImage> m_swapChainImages;
        Vector<VkImage> m_depthImages;
        Vector<VkImage> m_positionImages;
        Vector<VkImage> m_colorImages;
        Vector<VkImage> m_normalImages;

        Vector<VkImageView> m_swapChainImageViews;
        Vector<VkImageView> m_depthImageViews;
        Vector<VkImageView> m_positionImageViews;
        Vector<VkImageView> m_colorImageViews;
        Vector<VkImageView> m_normalImageViews;

        Vector<VkDeviceMemory> m_depthImageMemories;
        Vector<VkDeviceMemory> m_positionImageMemories;
        Vector<VkDeviceMemory> m_colorImageMemories;
        Vector<VkDeviceMemory> m_normalImageMemories;

        Vector<VkFramebuffer> m_swapChainFramebuffers;
        VkCommandPool m_commandPool;
        Vector<VkCommandBuffer> m_commandBuffers;
        VkPipeline m_gbufferPipeline;
        VkPipeline m_lightingPipeline;

        VkPipelineLayout m_gbufferPipelineLayout;
        VkPipelineLayout m_lightingPipelineLayout;

        VkRenderPass m_renderPass;

        const Vector<const char*> m_validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        const Vector<const char*> m_deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        RendererSettings m_rendererSettings;

        Vector<VkSemaphore> m_imageAvailableSemaphores;
        Vector<VkSemaphore> m_renderFinishedSemaphores;
        Vector<VkFence> m_inFlightFences;

        // Vertext Buffer
        VkBuffer m_gbufferVertexBuffer;
        VkBuffer m_lightingVertexBuffer;
        size_t m_vertexCount;
        VkDeviceMemory m_gbufferVertexBufferMemory = VK_NULL_HANDLE;
        VkDeviceMemory m_lightingVertexBufferMemory = VK_NULL_HANDLE;
        VkDescriptorSetLayout m_gbufferDescriptorSetLayout;
        VkDescriptorSetLayout m_lightingDescriptorSetLayout;

        VkBuffer m_gbufferIndexBuffer;
        VkBuffer m_lightingIndexBuffer;
        VkDeviceMemory m_gbufferIndexBufferMemory = VK_NULL_HANDLE;
        VkDeviceMemory m_lightingIndexBufferMemory = VK_NULL_HANDLE;

        Vector<Vector<VkBuffer>> m_uniformBuffers;
        Vector<Vector<VkDeviceMemory>> m_uniformBuffersMemory;
        Vector<Vector<void*>> m_uniformBuffersMapped;
        VkDescriptorPool m_gbufferDescriptorPool;
        Vector<Vector<VkDescriptorSet>> m_gbufferDescriptorSets;
        VkDescriptorPool m_lightingDescriptorPool;
        Vector<VkDescriptorSet> m_lightingDescriptorSets;

        Vector<SimpleVertex> m_vertices = {
            {{-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
            {{1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
            {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            {{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        };

        Vector<SimpleVertex> m_vertices2 = {
            {{-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
          {{1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
          {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
          {{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        };

        Vector<uint16_t> m_indices = {
            // UP
            0, 1, 2,
            2, 3, 0,

            // DOWN
            4, 6, 5,
            6, 4, 7,

            // LEFT
            0, 5, 1,
            5, 0 ,4,

            // RIGHT
            1, 6, 2,
            1, 5, 6,

            // FRONT
            2, 6, 3,
            3, 6, 7,

            //BACK
            3, 7, 0,
            4, 0, 7

        };

        Vector<uint16_t> m_indices2 = {
            // UP
            0, 2, 1,
            2, 0, 3
		};

        Vector<Ref<Mesh>> m_meshes;

        VkDescriptorPool m_imguiDescriptorPool;
    };
}
