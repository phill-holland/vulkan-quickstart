#include "vulkan.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>

void vulkan::vulkan::reset()
{
    init = false; cleanup();

    if (!createInstance()) return;

    uint32_t index = 0;
    VkPhysicalDevice vkPhysicalDevice = findDevice(index);
    if(vkPhysicalDevice == nullptr) return;

    if(!createDevice(vkPhysicalDevice)) return;

    if(!createWaylandWindow()) return;

    if(!createSurface()) return;

    init = true;
}

bool vulkan::vulkan::createInstance()
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

    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
    std::vector<VkExtensionProperties> extensionProperties(extensionCount);
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensionProperties.data());

    std::vector<const char*> extensionNames;

    for(uint32_t i = 0; i < extensionCount; ++i)
    {
        if(strcmp(extensionProperties[i].extensionName, "VK_KHR_surface_protected_capabilities") != 0)
        {
            extensionNames.push_back(extensionProperties[i].extensionName);
            std::cout << extensionProperties[i].extensionName << "\n";
        }
    }

    createInfo.enabledExtensionCount = extensionNames.size();
    createInfo.ppEnabledExtensionNames = extensionNames.data();

    createInfo.enabledLayerCount = 0;
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    if(findLayers(validationLayers))
    {
        createInfo.enabledLayerCount = 1;
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }

    VkResult result = vkCreateInstance(&createInfo, nullptr, &vkInstance);

    return result == VK_SUCCESS;
}

bool vulkan::vulkan::findLayers(const std::vector<const char*> &validationLayers)
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

VkPhysicalDevice vulkan::vulkan::findDevice(uint32_t index)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);

    if(deviceCount > 0)
    {
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

        uint32_t counter = 0;
        for(const auto &d:devices)
        {
            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(d, &deviceProperties);

            if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                std::cout << deviceProperties.deviceName << "\n";
                if(index == counter) return d;

                ++counter;
            }
        }
    }

    return VK_NULL_HANDLE;
}

bool vulkan::vulkan::createDevice(VkPhysicalDevice &device)
{
    if(device == nullptr) return false;

    uint32_t index = 0;
    if(!findQueueFamilies(device, index)) return false;

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = index;
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &deviceFeatures;

    return (vkCreateDevice(device, &createInfo, nullptr, &vkDevice) == VK_SUCCESS);
}

bool vulkan::vulkan::findQueueFamilies(VkPhysicalDevice &device, uint32_t &index)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    index = 0;
    for(const auto& queueFamily:queueFamilies)
    {
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) return true;
        ++index;
    }

    return false;
}

bool vulkan::vulkan::createSurface()
{
    VkWaylandSurfaceCreateInfoKHR createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    createInfo.display = wlDisplay;

    VkSurfaceKHR vkSurface;
    return (vkCreateWaylandSurfaceKHR(vkInstance,&createInfo, nullptr, &vkSurface) == VK_SUCCESS);\
    
    // ****
/*
    VkXlibSurfaceCreateInfoKHR createInfo2{};

    createInfo2.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    //createInfo2.dpy =
    vkCreateXlibSurfaceKHR()
*/
}

bool vulkan::vulkan::createWaylandWindow()
{
    wlDisplay = wl_display_connect(NULL);
    if(!wlDisplay) return false;

    return true;
}
/*
bool vulkan::vulkan::createX11Window()
{

}*/
void vulkan::vulkan::makeNull()
{
    vkInstance = VK_NULL_HANDLE;
    vkDevice = VK_NULL_HANDLE;
}

void vulkan::vulkan::cleanup()
{
    if(vkDevice != VK_NULL_HANDLE) vkDestroyDevice(vkDevice, nullptr);
    if(vkInstance != VK_NULL_HANDLE) vkDestroyInstance(vkInstance, nullptr);
}