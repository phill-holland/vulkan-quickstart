#include "buffer.h"
#include "vulkan.h"
#include <string.h>

bool vulkan::buffer::create(vulkan *device, void *data, size_t size, TYPE type)
{
    _data = data;
    _length = size;
    _type = type;

    this->device = device;

    VkDevice vkDevice = device->vkDevice;

    VkBufferCreateInfo bufferInfo = {};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;    
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.pNext = nullptr;

    if(type == TYPE::storage) 
        bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    else if(type == TYPE::uniform) 
        bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    else if(type == TYPE::indirect) 
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |  VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
    else if(type == TYPE::count) 
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;

    if(vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &vkBuffer) != VK_SUCCESS) return false;

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vkDevice, vkBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};

    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if(vkAllocateMemory(vkDevice, &allocInfo, nullptr, &vkBufferMemory) != VK_SUCCESS) return false;

    if(vkBindBufferMemory(vkDevice, vkBuffer, vkBufferMemory, 0) != VK_SUCCESS) return false;

    return true;
}

void vulkan::buffer::update()
{
    void *data_map;

    vkMapMemory(device->vkDevice, vkBufferMemory, 0, _length, 0, &data_map);
    memcpy(data_map, _data, (size_t)_length);
    vkUnmapMemory(device->vkDevice, vkBufferMemory);
}

size_t vulkan::buffer::alignMemoryBuffer(size_t size, TYPE type)
{            
    size_t minUboAlignment = device->vkPhysicalDeviceProperties.limits.minUniformBufferOffsetAlignment;
    if(type == TYPE::storage) minUboAlignment = device->vkPhysicalDeviceProperties.limits.minStorageBufferOffsetAlignment;

    size_t alignedSize = size;
    if(minUboAlignment > 0)
        alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);

    return alignedSize;
}

void vulkan::buffer::destroy()
{
    VkDevice vkDevice = device->vkDevice;
    
    vkDestroyBuffer(vkDevice, vkBuffer, nullptr);
    vkFreeMemory(vkDevice, vkBufferMemory, nullptr);
}