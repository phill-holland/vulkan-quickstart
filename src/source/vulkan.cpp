#include "vulkan.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>

void vulkan::vulkan::reset()
{
    init = false; cleanup();

    if (!create()) return;
    if (!device()) return;

    init = true;
}

bool vulkan::vulkan::create()
{
    VkApplicationInfo appInfo {};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "test";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t extensionCount = 0;
    //char **extensionNames;

    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
    std::vector<VkExtensionProperties> extensionProperties(extensionCount);
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensionProperties.data());

    //extensionNames = new char*[extensionCount];
    //if(extensionNames == nullptr) return false;
    std::vector<const char*> extensionNames;

    for(uint32_t i = 0; i < extensionCount; ++i)
    {
        if(strcmp(extensionProperties[i].extensionName, "VK_KHR_surface_protected_capabilities") != 0)
        {
            //extensionNames[i] = extensionProperties[i].extensionName;
            extensionNames.push_back(extensionProperties[i].extensionName);
            std::cout << extensionProperties[i].extensionName << "\n";
        }
    }

    createInfo.enabledExtensionCount = extensionNames.size();//extensionCount;
    createInfo.ppEnabledExtensionNames = extensionNames.data();

    createInfo.enabledLayerCount = 0;
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    if(layers(validationLayers))
    {
        createInfo.enabledLayerCount = 1;
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

    return result == VK_SUCCESS;
}

bool vulkan::vulkan::layers(const std::vector<const char*> &validationLayers)
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for(const char *layerName:validationLayers)
    {
        for(const auto &layerProperties:availableLayers)
        {
            if(strcmp(layerName,layerProperties.layerName) == 0)
            {
                return true;
            }
        }
    }

    return false;
}

bool vulkan::vulkan::device()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if(deviceCount > 0)
    {
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for(const auto &d:devices)
        {
            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(d, &deviceProperties);

            if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                std::cout << deviceProperties.deviceName << "\n";
                physicalDevice = d;
                return true;
            }
        }
    }

    return false;
}

void vulkan::vulkan::makeNull()
{
    instance = VK_NULL_HANDLE;
    physicalDevice = VK_NULL_HANDLE;
}

void vulkan::vulkan::cleanup()
{
    //if(physicalDevice != VK_NULL_HANDLE) vkDestroyPh
    if(instance != VK_NULL_HANDLE) vkDestroyInstance(instance, nullptr);
}