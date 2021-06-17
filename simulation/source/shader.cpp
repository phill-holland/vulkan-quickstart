#include "shader.h"
#include <iostream>
#include <fstream>
#include <vector>

bool vulkan::shader::create(VkDevice vkDevice, std::string filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (file.is_open())
    {
        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = buffer.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

        if (vkCreateShaderModule(vkDevice, &createInfo, nullptr, &vkShader) == VK_SUCCESS) return true;
        //VkResult vkResult = vkCreateShaderModule(vkDevice, &createInfo, nullptr, &shaderModule);
        //if(vkResult == VK_SUCCESS) return true;
    }

    return false;
}