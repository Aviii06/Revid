#pragma once

#include "VulkanDevice.h"
#include "revid_engine/ServiceLocater.h"

// std lib headers
#include <string>
#include <vector>

namespace Revid {

class VulkanSwapchain {
 public:
  static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

  VulkanSwapchain(VulkanDevice &device) : m_device{device} { init(); }

  ~VulkanSwapchain() {
    cleanupSwapChain();
    cleanupSyncObjects();
  }

  VulkanSwapchain(const VulkanSwapchain &) = delete;
  void operator=(const VulkanSwapchain &) = delete;

  VkFramebuffer getFrameBuffer(int index) { return m_swapChainFramebuffers[index]; }
  VkRenderPass getRenderPass() { return renderPass; }
  VkImageView getImageView(int index) { return swapChainImageViews[index]; }
  void recreateSwapChain();
  void cleanupSwapChain();
  size_t imageCount() { return swapChainImages.size(); }
  VkFormat getSwapChainImageFormat() { return m_swapChainImageFormat; }
  VkExtent2D getSwapChainExtent() { return m_swapChainExtent; }
  uint32_t width() { return m_swapChainExtent.width; }
  uint32_t height() { return m_swapChainExtent.height; }

  float extentAspectRatio() {
    return static_cast<float>(m_swapChainExtent.width) / static_cast<float>(m_swapChainExtent.height);
  }
  VkFormat findDepthFormat();

  VkResult acquireNextImage(uint32_t *imageIndex);
  VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

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
      const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

  VkFormat m_swapChainImageFormat;
  VkExtent2D m_swapChainExtent;

  std::vector<VkFramebuffer> m_swapChainFramebuffers;
  VkRenderPass renderPass;

  std::vector<VkImage> depthImages;
  std::vector<VkDeviceMemory> depthImageMemorys;
  std::vector<VkImageView> depthImageViews;
  std::vector<VkImage> swapChainImages;
  std::vector<VkImageView> swapChainImageViews;

  VulkanDevice &m_device;

  VkSwapchainKHR swapChain;

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  std::vector<VkFence> imagesInFlight;
  size_t currentFrame = 0;
};

}  // namespace lve