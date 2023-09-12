#include <vulkan/vulkan.h>

#ifndef _VULKAN_BUFFER
#define _VULKAN_BUFFER

namespace vulkan
{
    class vulkan;
    class pipeline;

    class buffer
    {
        VkBuffer vkBuffer;
        VkDeviceMemory vkBufferMemory;
        VkDescriptorSet vkDescriptorSet;

        vulkan *device;

        void *_data;
        int _length;

    private:
        friend class vulkan;
        friend class pipeline;

    private:
        buffer() { }

        bool create(vulkan *device, void *data, size_t size);        

        void destroy();        

    public:
        void update();
    };
};

#endif