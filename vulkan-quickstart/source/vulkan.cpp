#include "vulkan.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <set>
#include <cstdint>
#include <algorithm>

void vulkan::vulkan::reset()
{
    init = false; cleanup();

    width = WIDTH; height = HEIGHT;

    if (!createInstance()) return;

    uint32_t index = 0;
    vkPhysicalDevice = findDevice(index);
    if(vkPhysicalDevice == nullptr) return;

    if(!findDeviceExtensionSupport(vkPhysicalDevice)) return;

    queueFamilyIndex = 0;
    if(!findQueueFamily(vkPhysicalDevice, queueFamilyIndex)) return;

    if(!createWindow(index)) return;
    if(!createSurface(vkPhysicalDevice, queueFamilyIndex)) return;

    queuePresentIndex = 0;
    if(!findQueuePresentationFamily(vkPhysicalDevice, queuePresentIndex)) return;

    if(!createDevice(vkPhysicalDevice, queueFamilyIndex, queuePresentIndex)) return;
    if(!createSwapChain(vkPhysicalDevice)) return;
    if(!createImageViews()) return;

    init = true;
}

vulkan::shader::shader *vulkan::vulkan::createShader(shader::parameters params)
{
    shader::shader *temp = new shader::shader();
    if(temp != NULL)
    {
        if(temp->create(vkDevice, params))
        {
            shaders.push_back(temp);
            return temp;
        }

        delete temp;
    }

    return NULL;
}

vulkan::mesh *vulkan::vulkan::createMesh(primatives::mesh vertices)
{
    mesh *temp = new mesh();
    if(temp != NULL)
    {
        if(temp->create(this, vertices))
        {
            meshes.push_back(temp);
            return temp;
        }

        delete temp;
    }

    return NULL;
}

vulkan::buffer *vulkan::vulkan::createBuffer(void *data, size_t size)
{
    buffer *temp = new buffer();
    if(temp != NULL)
    {
        if(temp->create(this, data, size))
        {
            buffers.push_back(temp);
            return temp;
        }

        delete temp;
    }

    return NULL;
}

vulkan::pipeline *vulkan::vulkan::createPipeline(std::vector<shader::shader*> shaders, std::vector<mesh*> mesh, std::vector<buffer*> buffers, ::vulkan::constants *constants)
{
    pipeline *temp = new pipeline();
    if(temp != NULL)
    {
        if(temp->create(this, shaders, mesh, buffers, constants))
        {
            pipelines.push_back(temp);
            return temp;
        }

        delete temp;
    }

    return NULL;
}

bool vulkan::vulkan::createInstance(bool enableLayer)
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

    std::set<std::string> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_XLIB_SURFACE_EXTENSION_NAME };
    std::vector<const char*> extensionNames;

    for(uint32_t i = 0; i < extensionCount; ++i)
    {
        std::cout << extensionProperties[i].extensionName;
        bool found = false;
        for(const auto &de:deviceExtensions)
        {
            if(strcmp(extensionProperties[i].extensionName, de.c_str()) == 0)
            {
                extensionNames.push_back(extensionProperties[i].extensionName);
                found = true;
            }
        }
        if(found) std::cout << "*\n";
        else std::cout << "\n";
    }

    createInfo.enabledExtensionCount = extensionNames.size();
    createInfo.ppEnabledExtensionNames = extensionNames.data();

    if(enableLayer)
    {
        createInfo.enabledLayerCount = 0;
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        if(findLayers(validationLayers))
        {
            createInfo.enabledLayerCount = 1;
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
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

            //if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            //{
                std::cout << deviceProperties.deviceName << "\n";
                if(index == counter) return d;

                ++counter;
            //}
        }
    }

    return VK_NULL_HANDLE;
}

bool vulkan::vulkan::createDevice(VkPhysicalDevice &device, uint32_t queueFamilyIndex, uint32_t queuePresentIndex)
{
    if(device == nullptr) return false;

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> queues = { queueFamilyIndex, queuePresentIndex };

    for(uint32_t family: queues)
    {
        VkDeviceQueueCreateInfo queueCreateFamilyInfo{};
        queueCreateFamilyInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateFamilyInfo.queueFamilyIndex = family;
        queueCreateFamilyInfo.queueCount = 1;
        queueCreateFamilyInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = queueCreateInfos.size();

    createInfo.pEnabledFeatures = &deviceFeatures;

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    createInfo.enabledExtensionCount = deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    VkResult result = vkCreateDevice(device, &createInfo, nullptr, &vkDevice);
    if(result != VK_SUCCESS) return false;

    vkGetDeviceQueue(vkDevice, queuePresentIndex, 0, &vkPresentQueue);
    vkGetDeviceQueue(vkDevice, queueFamilyIndex, 0, &vkGraphicsQueue);

    return true;
}

bool vulkan::vulkan::findQueueFamily(VkPhysicalDevice &device, uint32_t &queueFamilyIndex)
{
    queueFamilyIndex = 0;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    uint32_t index = 0;
    
    for(const auto& queueFamily:queueFamilies)
    {
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queueFamilyIndex = index;
            return true;
        }

        ++index;
    }

    return false;
}

bool vulkan::vulkan::findQueuePresentationFamily(VkPhysicalDevice &device, uint32_t &queuePresentIndex)//, uint32_t &queuePresentIn
{
    queuePresentIndex = 0;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    uint32_t index = 0;
    for(const auto& queueFamily:queueFamilies)
    {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, index, vkSurface, &presentSupport);

        if(presentSupport) 
        {
            queuePresentIndex = index;
            return true;
        }

        ++index;
    }

    return false;
}

bool vulkan::vulkan::createSurface(VkPhysicalDevice &device, uint32_t queue)
{
    VkXlibSurfaceCreateInfoKHR createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    createInfo.dpy = display;
    createInfo.window = window;

    if(vkCreateXlibSurfaceKHR(vkInstance, &createInfo, nullptr, &vkSurface) != VK_SUCCESS) return false;

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, queue, vkSurface, &presentSupport);

    return presentSupport;
}

bool vulkan::vulkan::findDeviceExtensionSupport(VkPhysicalDevice &device)
{
    std::set<std::string> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    for(const auto &extension:availableExtensions)
    {
        //std::cout << extension.extensionName << "\n";
        deviceExtensions.erase(extension.extensionName);
    }

    return deviceExtensions.empty();
}


bool vulkan::vulkan::createSwapChain(VkPhysicalDevice &device)
{
    vkPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    VkSurfaceCapabilitiesKHR vkSurfaceCapabilities;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vkSurface, &vkSurfaceCapabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, vkSurface, &formatCount, nullptr);

    if(formatCount > 0)
    {
        std::vector<VkSurfaceFormatKHR> formats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, vkSurface, &formatCount, formats.data());

        bool found = false;
        for(const auto& format:formats)
        {
            if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                vkSurfaceFormat = format;
                found = true;
            }
        }

        if(found)
        {
            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, vkSurface, &presentModeCount, nullptr);

            if(presentModeCount > 0)
            {
                std::vector<VkPresentModeKHR> modes(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, vkSurface, &presentModeCount, modes.data());

                for(const auto& mode:modes)
                {
                    //std::cout << mode << "\n";
                    if(mode == VK_PRESENT_MODE_MAILBOX_KHR)
                    {
                        vkPresentMode = mode;
                    }
                }
            }

            // ****
            vkExtent = findSwapExtent(vkSurfaceCapabilities);

            VkSwapchainCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = vkSurface;
            createInfo.minImageCount = vkSurfaceCapabilities.minImageCount + 1;
            createInfo.imageFormat = vkSurfaceFormat.format;
            createInfo.imageColorSpace = vkSurfaceFormat.colorSpace;
            createInfo.imageExtent = vkExtent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            if(queueFamilyIndex != queuePresentIndex)
            {
                uint32_t indices[] = { queueFamilyIndex, queuePresentIndex };
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = indices;
            }
            else
            {
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                createInfo.queueFamilyIndexCount = 0;
                createInfo.pQueueFamilyIndices = nullptr;
            }

            createInfo.preTransform = vkSurfaceCapabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode = vkPresentMode;
            createInfo.clipped = VK_TRUE;
            createInfo.oldSwapchain = VK_NULL_HANDLE;


            return vkCreateSwapchainKHR(vkDevice, &createInfo, nullptr, &vkSwapChain) == VK_SUCCESS;

            // ****


            // ****

            //return true;
        }
    }

    return false;
}

bool vulkan::vulkan::createImageViews()
{
    uint32_t imageCount = 0;
    vkGetSwapchainImagesKHR(vkDevice, vkSwapChain, &imageCount, nullptr);

    std::vector<VkImage> swapChainImages(imageCount);
    vkGetSwapchainImagesKHR(vkDevice, vkSwapChain, &imageCount, swapChainImages.data());

    swapChainImageViews.resize(imageCount);

    for(size_t i = 0; i < swapChainImages.size(); ++i)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = vkSurfaceFormat.format;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if(vkCreateImageView(vkDevice, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) return false;
    }

    return true;
}

VkExtent2D vulkan::vulkan::findSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width == UINT32_MAX)
    {
        VkExtent2D actualExtent =
        {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::ranges::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::ranges::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }

    return capabilities.currentExtent;
}

uint32_t vulkan::vulkan::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{  
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &memProperties);
  
    for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;
    }

    return -1;
}

bool vulkan::vulkan::createWindow(uint32_t index)
{
    int x = 0, y = 0;

	XInitThreads();

	display = XOpenDisplay(NULL);

	windowAttrib.border_pixel = BlackPixel(display, (int)index);
	windowAttrib.background_pixel = WhitePixel(display, (int)index);
	windowAttrib.override_redirect = True;
	windowAttrib.colormap = XCreateColormap(display, RootWindow(display, (int)index),
    DefaultVisual(display, index), AllocNone);
	windowAttrib.event_mask = ExposureMask;

	window = XCreateWindow(display, RootWindow(display, (int)index),
                           x, y, width, height, 0,
                           DefaultDepth(display, index),
                           CopyFromParent,
                           DefaultVisual(display, index),
                           CWBackPixel | CWColormap | CWBorderPixel | CWEventMask,
                           &windowAttrib);

	XSelectInput(display, window, ExposureMask | StructureNotifyMask);

	XMapWindow(display, window);

	char caption[20] = "Screen\0";

    XStoreName(display, window, caption);

	return true;
}

void vulkan::vulkan::makeNull()
{
    vkInstance = VK_NULL_HANDLE;
    vkDevice = VK_NULL_HANDLE;

    display = NULL;

    vkSurface = VK_NULL_HANDLE;
    vkSwapChain = VK_NULL_HANDLE;
}

void vulkan::vulkan::cleanup()
{
//    vkDeviceWaitIdle(vkDevice);
    
    for(auto mesh: meshes)
    {
        mesh->destroy();
        delete mesh;
    }

    for(auto shader: shaders)
    {
        shader->destroy(vkDevice);
        delete shader;
    }

    for(auto buffer: buffers)
    {
        buffer->destroy();
        delete buffer;
    }

    for(auto pipeline: pipelines)
    {
        pipeline->destroy();
        delete pipeline;
    }
        

    for(auto imageView: swapChainImageViews)
    {
        vkDestroyImageView(vkDevice, imageView, nullptr);
    }

    if(vkSwapChain != VK_NULL_HANDLE) vkDestroySwapchainKHR(vkDevice, vkSwapChain, nullptr);
    if(vkSurface != VK_NULL_HANDLE) vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);

    if (display != NULL)
	{
		XFreeColormap(display, windowAttrib.colormap);
		XDestroyWindow(display, window);
		XCloseDisplay(display);
	}

    if(vkDevice != VK_NULL_HANDLE) vkDestroyDevice(vkDevice, nullptr);
    if(vkInstance != VK_NULL_HANDLE) vkDestroyInstance(vkInstance, nullptr);
}