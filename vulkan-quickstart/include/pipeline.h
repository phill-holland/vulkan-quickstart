#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include "shader.h"
#include "mesh.h"
#include "buffer.h"
#include "constants.h"

#ifndef _VULKAN_PIPELINE
#define _VULKAN_PIPELINE

namespace vulkan
{
    class vulkan;

    class pipeline
    {
        VkRenderPass vkRenderPass;
        VkPipelineLayout vkPipelineLayout;
        VkDescriptorSetLayout vkSetLayout;
        VkDescriptorPool vkDescriptorPool;
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

        bool create(vulkan *device, std::vector<shader::shader*> shaders, std::vector<mesh*> meshes, std::vector<buffer*> buffers, constants *constants);
        void destroy();

    public:
        bool render();

    protected:
        bool createRenderPass(VkDevice vkDevice, VkFormat vkFormat);
        bool createPipelineLayout(VkDevice vkDevice, ::vulkan::constants* constants);
        bool createPipeline(VkDevice vkDevice, VkExtent2D vkExtent, std::vector<shader::shader*> shaders);
        bool createFrameBuffers(VkDevice vkDevice,  VkExtent2D vkExtent, std::vector<VkImageView> &swapChainImageViews);
        bool createCommandPool(VkDevice vkDevice, uint32_t queueFamilyIndex);
        bool createCommandBuffers(VkDevice vkDevice);
        bool bindCommandQueue(std::vector<shader::shader*> shaders, std::vector<mesh*> meshes, std::vector<buffer*> buffers, ::vulkan::constants* constants);

    protected:
        bool createBufferDescriptors(VkDevice vkDevice, std::vector<buffer*> buffers);
    };
};

#endif