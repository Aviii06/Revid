#pragma once

#include "VulkanDevice.h"
#include "revid_engine/ServiceLocater.h"

// std lib headers
#include <string>
#include <vector>

namespace Revid
{
    class VulkanSwapchain
    {
    public:
        VulkanSwapchain(VulkanDevice& device, uint8_t MAX_FRAME_IN_FLIGHT) : m_device{device}, m_maximumFrames(MAX_FRAME_IN_FLIGHT)  { init(); }

        ~VulkanSwapchain()
        {
            cleanupSwapChain();
            cleanupSyncObjects();
        }

        VulkanSwapchain(const VulkanSwapchain&) = delete;
        void operator=(const VulkanSwapchain&) = delete;

        VkFramebuffer getFrameBuffer(int index) { return m_swapChainFramebuffers[index]; }
        VkRenderPass getRenderPass() { return m_renderPass; }
        VkImageView getImageView(int index) { return swapChainImageViews[index]; }
        void recreateSwapChain();
        void cleanupSwapChain();
        size_t imageCount() { return swapChainImages.size(); }
        VkFormat getSwapChainImageFormat() { return m_swapChainImageFormat; }
        VkExtent2D getSwapChainExtent() { return m_swapChainExtent; }
        uint32_t width() { return m_swapChainExtent.width; }
        uint32_t height() { return m_swapChainExtent.height; }

        float extentAspectRatio()
        {
            return static_cast<float>(m_swapChainExtent.width) / static_cast<float>(m_swapChainExtent.height);
        }

        VkFormat findDepthFormat();

        VkResult acquireNextImage(uint32_t imageIndex);
        VkResult submitCommandBuffers(const Vector<VkCommandBuffer>& buffer, uint32_t imageIndex);

    private:
        void init();
        void cleanupSyncObjects();
        void createSwapChain();
        void createImageViews();
        void createDepthResources();
        void createRenderPass();
        void createFramebuffers();
        void createSyncObjects();

        // Helper functions
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(
            const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        void recordCommandBuffer(const VkCommandBuffer& commandBuffer, uint32_t imageIndex);

        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;

        std::vector<VkFramebuffer> m_swapChainFramebuffers;
        VkRenderPass m_renderPass;

        std::vector<VkImage> depthImages;
        std::vector<VkDeviceMemory> depthImageMemorys;
        std::vector<VkImageView> depthImageViews;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;

        VulkanDevice& m_device;

        VkSwapchainKHR m_swapChain;

        Vector<VkSemaphore> m_imageAvailableSemaphores;
        Vector<VkSemaphore> m_renderFinishedSemaphores;
        Vector<VkFence> m_inFlightFences;
        Vector<VkFence> imagesInFlight;
        size_t m_currentFrame = 0;

        uint8_t m_maximumFrames;
    };
} // namespace lve
