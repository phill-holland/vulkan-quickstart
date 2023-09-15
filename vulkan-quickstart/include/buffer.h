#include <vulkan/vulkan.h>

#ifndef _VULKAN_BUFFER
#define _VULKAN_BUFFER

namespace vulkan
{
    class vulkan;
    class pipeline;

    class buffer
    {
    public:
        enum TYPE
        {
            uniform = 0,
            storage = 1,
            indirect = 2,
            count = 3
        };
        
    private:
        VkBuffer vkBuffer;
        VkDeviceMemory vkBufferMemory;
        VkDescriptorSet vkDescriptorSet;

        vulkan *device;

        void *_data;
        int _length;

        TYPE _type;

    private:
        friend class vulkan;
        friend class pipeline;

    private:
        buffer() { }

        bool create(vulkan *device, void *data, size_t size, TYPE type = TYPE::uniform);
        void destroy();        
        
        TYPE type() { return _type; }
        
    public:
        void update();

    protected:
        size_t alignMemoryBuffer(size_t size, TYPE type);        
    };
};

#endif