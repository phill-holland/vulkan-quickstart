#include <vulkan/vulkan.h>
#include "primatives/vertex.h"
#include "primatives/mesh.h"

#ifndef _VULKAN_MESH
#define _VULKAN_MESH

namespace vulkan
{
    class vulkan;
    class pipeline;

    class mesh
    {
        VkBuffer vkVertexBuffer;
        VkDeviceMemory vkVertexBufferMemory;

        vulkan *device;

        int length;

    private:
        friend class vulkan;
        friend class pipeline;

    private:
        mesh() { }

        bool create(vulkan *device, primatives::mesh vertices);
        void destroy();        
    };
};

#endif