#include <vulkan/vulkan.h>
#include <vector>

#ifndef _VULKAN_VERTEX
#define _VULKAN_VERTEX

namespace vulkan
{
    namespace primatives
    {
        struct vec2
        {
            float x,y;
        };

        struct vec3
        {
            float x,y,z;
        };

        struct vertex
        {
            vec2 position;
            vec3 colour;

            static VkVertexInputBindingDescription getBindingDescription() 
            {
                VkVertexInputBindingDescription bindingDescription {};

                bindingDescription.binding = 0;
                bindingDescription.stride = sizeof(vertex);
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                return bindingDescription;
            }

            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() 
            {
                std::vector<VkVertexInputAttributeDescription> attributeDescription;

                VkVertexInputAttributeDescription a;
                
                a.binding = 0;
                a.location = 0;
                a.format = VK_FORMAT_R32G32_SFLOAT;
                a.offset = offsetof(vertex, position);

                VkVertexInputAttributeDescription b;

                b.binding = 0;
                b.location = 1;
                b.format = VK_FORMAT_R32G32B32_SFLOAT;
                b.offset = offsetof(vertex, colour);

                attributeDescription.push_back(a);
                attributeDescription.push_back(b);

                return attributeDescription;
            }
        };

        struct mesh
        {
            std::vector<vertex> vertices;
        };
    }
};

#endif