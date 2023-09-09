#include "mesh.h"
#include "vulkan.h"
#include <string.h>

bool vulkan::mesh::create(vulkan *device, primatives::mesh vertices)
{
    this->device = device;

    VkDevice vkDevice = device->vkDevice;

    VkBufferCreateInfo bufferInfo{};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(vertices.vertices[0]) * vertices.vertices.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if(vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &vkVertexBuffer) != VK_SUCCESS) return false;

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vkDevice, vkVertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};

    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if(vkAllocateMemory(vkDevice, &allocInfo, nullptr, &vkVertexBufferMemory) != VK_SUCCESS) return false;

    vkBindBufferMemory(vkDevice, vkVertexBuffer, vkVertexBufferMemory, 0);

    void *data;

    vkMapMemory(vkDevice, vkVertexBufferMemory, 0, bufferInfo.size, 0, &data);
    memcpy(data, vertices.vertices.data(), (size_t)bufferInfo.size);
    vkUnmapMemory(vkDevice, vkVertexBufferMemory);

    length = vertices.vertices.size();

    return true;
}

void vulkan::mesh::destroy()
{
    VkDevice vkDevice = device->vkDevice;

    vkDestroyBuffer(vkDevice, vkVertexBuffer, nullptr);
    vkFreeMemory(vkDevice, vkVertexBufferMemory, nullptr);
}


