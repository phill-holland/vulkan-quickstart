#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include "vector.h"

#ifndef _VULKAN_PRIMATIVE_VERTEX
#define _VULKAN_PRIMATIVE_VERTEX

namespace vulkan
{
    namespace primatives
    {        
        class vertex
        {
        public:
            vector3 position;
            vector3 colour;

        public:
            vertex(vector3 _position = { 0.0f, 0.0f, 0.0f } , vector3 _colour = { 1.0f, 1.0f ,1.0f } )
            {                
                position = _position;
                colour = _colour;
            }

        public:
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
                a.format = VK_FORMAT_R32G32B32_SFLOAT;
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
    }
};

#endif