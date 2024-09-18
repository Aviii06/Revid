#pragma once
#define GLFW_INCLUDE_VULKAN
#include <revid_engine/renderer/Renderer.h>
#include <vulkan/vulkan.h>
#include "Vertex.h"
#include "types/SmartPointers.h"

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

namespace Revid
{
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

    class VulkanRenderer : public Renderer
    {
    public:
        void Init(const RendererSettings&) override;
        void Shutdown() override;
        void Render() override;
        void UpdateVertices(Vector<SimpleVertex>) override;
        void UpdateIndices(Vector<uint16_t>) override;
        void UpdateObj(String path) override;

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

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
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
        std::vector<VkImage> m_swapChainImages;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;
        VkPipelineLayout m_pipelineLayout;
        VkRenderPass m_renderPass;
        std::vector<VkImageView> m_swapChainImageViews;
        std::vector<VkFramebuffer> m_swapChainFramebuffers;
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
        VkBuffer m_vertexBuffer;
        size_t m_vertexCount;
        VkDeviceMemory m_vertexBufferMemory;
        VkDescriptorSetLayout m_descriptorSetLayout;

        VkBuffer m_indexBuffer;
        VkDeviceMemory m_indexBufferMemory;

        std::vector<VkBuffer> m_uniformBuffers;
        std::vector<VkDeviceMemory> m_uniformBuffersMemory;
        std::vector<void*> m_uniformBuffersMapped;
        VkDescriptorPool m_descriptorPool;
        Vector<VkDescriptorSet> m_descriptorSets;


        Vector<SimpleVertex> m_vertices = {
            {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
          {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
          {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
          {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},

            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
          {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
          {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
          {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}
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
    };
}
