#include <vulkan/vulkan.h>
#include <string>

#ifndef _VULKAN_SHADER
#define _VULKAN_SHADER

namespace vulkan
{
    class vulkan;
    class pipeline;

    class shader
    {
        VkShaderModule vkShader;

    private:
        friend class vulkan;
        friend class pipeline;

    private:
        shader() { }

        bool create(VkDevice vkDevice, std::string filename);
        void destroy(VkDevice vkDevice)
        {
            vkDestroyShaderModule(vkDevice, vkShader, nullptr);
        }
    };
};

#endif