#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include "Vertex.h"
#include "VertexBuffer.h"
#include "types/Containers.h"
#include "types/SmartPointers.h"

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

namespace Revid
{
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
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanRenderer
    {
    public:
        VulkanRenderer();
		void FramebufferResized() { m_framebufferResized = true; }
        void Init(const RendererSettings&);
        void Shutdown();
        void Render();
        void BindVertexBuffer(Ref<VertexBuffer> vb);
        void UpdateIndices(Vector<uint16_t>);
        //void UpdateObj(String path);

        VkDevice GetDevice() { return m_device; }
        VkCommandPool GetCommandPool() { return m_commandPool; }
        VkPhysicalDevice GetPhysicalDevice() { return m_physicalDevice; }
        VkQueue GetGraphicsQueue() { return m_graphicsQueue; }

    private:
        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevices();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createDescriptorSetLayout();
        void createGraphicsPipeline();
        void createFramebuffers();
        void createCommandPool();
        void createVertexBuffer();
        void createIndexBuffer();
        void createUniformBuffers();
        void createDescriptorPool();
        void createDescriptorSets();
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

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        VkShaderModule createShaderModule(const std::vector<char>& code);
        void recordCommandBuffer(const VkCommandBuffer& commandBuffer, uint32_t imageIndex);

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
            VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void updateUniformBuffer(uint32_t currentImage);


    private:
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
        Vector<VkImage> m_swapChainImages;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;
        VkPipelineLayout m_pipelineLayout;
        VkRenderPass m_renderPass;
        Vector<VkImageView> m_swapChainImageViews;
        Vector<VkFramebuffer> m_swapChainFramebuffers;
        VkCommandPool m_commandPool;
        Vector<VkCommandBuffer> m_commandBuffers;
        VkPipeline m_graphicsPipeline;


        const std::vector<const char*> m_validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        const std::vector<const char*> m_deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        RendererSettings m_rendererSettings;

        Vector<VkSemaphore> m_imageAvailableSemaphores;
        Vector<VkSemaphore> m_renderFinishedSemaphores;
        Vector<VkFence> m_inFlightFences;

        // Vertext Buffer
        VkDescriptorSetLayout m_descriptorSetLayout;
        Ref<VertexBuffer> m_vertexBuffer;

        VkBuffer m_indexBuffer;
        VkDeviceMemory m_indexBufferMemory;

        std::vector<VkBuffer> m_uniformBuffers;
        std::vector<VkDeviceMemory> m_uniformBuffersMemory;
        std::vector<void*> m_uniformBuffersMapped;
        VkDescriptorPool m_descriptorPool;
        Vector<VkDescriptorSet> m_descriptorSets;

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

		bool m_framebufferResized = false;
    };
}
