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
        VkDescriptorSetLayout vkGlobalSetLayout;
        VkDescriptorSetLayout vkObjectSetLayout;
        VkDescriptorPool vkDescriptorPool;
        VkPipeline vkGraphicsPipeline;
        VkCommandPool vkCommandPool;

        VkImage vkDepthImage;
        VkImageView vkDepthImageView;
        VkDeviceMemory vkDepthImageMemory;

        std::vector<VkFramebuffer> swapChainFramebuffers;
        std::vector<VkCommandBuffer> commandBuffers;

        VkSemaphore vkImageAvailableSemaphore;
        VkSemaphore vkRenderFinishedSemaphore;

        buffer indirectCommandBuffer;
        std::vector<VkDrawIndirectCommand> indirectCommands;

        vulkan *device;

    private:
        friend class vulkan;

    private:
        pipeline() { }

        bool create(vulkan *device, std::vector<shader::shader*> shaders, std::vector<mesh*> meshes, std::vector<buffer*> buffers, constants *constants);
        void destroy();

    public:
        bool render();
        bool update(int mesh_index, int instance_count, int first_instance);

    protected:
        bool createDepthImage(VkDevice vkDevice, VkExtent2D vkExtent);
        bool createRenderPass(VkDevice vkDevice, VkFormat vkFormat);
        bool createPipelineLayout(VkDevice vkDevice, ::vulkan::constants* constants);
        bool createPipeline(VkDevice vkDevice, VkExtent2D vkExtent, std::vector<shader::shader*> shaders);
        bool createFrameBuffers(VkDevice vkDevice,  VkExtent2D vkExtent, std::vector<VkImageView> &swapChainImageViews);
        bool createCommandPool(VkDevice vkDevice, uint32_t queueFamilyIndex);
        bool createCommandBuffers(VkDevice vkDevice);
        bool indirectDraw(std::vector<shader::shader*> shaders, std::vector<mesh*> meshes, std::vector<buffer*> buffers, ::vulkan::constants *constants);
        bool bindCommandQueue(std::vector<shader::shader*> shaders, std::vector<mesh*> meshes, std::vector<buffer*> buffers, ::vulkan::constants* constants);

    protected:
        bool createBufferDescriptors(VkDevice vkDevice, std::vector<buffer*> buffers);
    };
};

#endif