#include "buffer.h"
#include "vulkan.h"
#include <string.h>

bool vulkan::buffer::create(vulkan *device, void *data, size_t size)
{
    _data = data;
    _length = size;

    this->device = device;

    VkDevice vkDevice = device->vkDevice;

    VkBufferCreateInfo bufferInfo = {};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.pNext = nullptr;

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

void vulkan::buffer::destroy()
{
    VkDevice vkDevice = device->vkDevice;
    
    vkDestroyBuffer(vkDevice, vkBuffer, nullptr);
    vkFreeMemory(vkDevice, vkBufferMemory, nullptr);
}