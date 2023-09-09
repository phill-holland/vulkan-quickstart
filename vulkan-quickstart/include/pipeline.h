#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include "shader.h"
#include "mesh.h"

#ifndef _VULKAN_PIPELINE
#define _VULKAN_PIPELINE

namespace vulkan
{
    class vulkan;

    class pipeline
    {
        VkRenderPass vkRenderPass;
        VkPipelineLayout vkPipelineLayout;
        VkPipeline vkGraphicsPipeline;
        VkCommandPool vkCommandPool;

        std::vector<VkFramebuffer> swapChainFramebuffers;
        std::vector<VkCommandBuffer> commandBuffers;

        VkSemaphore vkImageAvailableSemaphore;
        VkSemaphore vkRenderFinishedSemaphore;

        vulkan *device;

    private:
        friend class vulkan;

    private:
        pipeline() { }

        bool create(vulkan *device, std::vector<shader::shader*> shaders, std::vector<mesh*> meshes);
        void destroy();

    public:
        bool render();

    protected:
        bool createRenderPass(VkDevice vkDevice, VkFormat vkFormat);
        bool createPipelineLayout(VkDevice vkDevice);
        bool createPipeline(VkDevice vkDevice, VkExtent2D vkExtent, std::vector<shader::shader*> shaders);
        bool createFrameBuffers(VkDevice vkDevice,  VkExtent2D vkExtent, std::vector<VkImageView> &swapChainImageViews);
        bool createCommandPool(VkDevice vkDevice, uint32_t queueFamilyIndex);
        bool createCommandBuffers(VkDevice vkDevice);
        bool bindCommandQueue(std::vector<shader::shader*> shaders, std::vector<mesh*> meshes);
    };
};

#endif