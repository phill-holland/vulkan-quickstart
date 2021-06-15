#include "vulkan.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <set>

void vulkan::vulkan::reset()
{
    init = false; cleanup();

    if (!createInstance()) return;

    uint32_t index = 0;
    VkPhysicalDevice vkPhysicalDevice = findDevice(index);
    if(vkPhysicalDevice == nullptr) return;

    queueFamilyIndex = 0;

    if(!findQueueFamily(vkPhysicalDevice, queueFamilyIndex)) return;

    if(!createWindow(index)) return;
    if(!createSurface(vkPhysicalDevice, queueFamilyIndex)) return;

    queuePresentIndex = 0;
    if(!findQueuePresentationFamily(vkPhysicalDevice, queuePresentIndex)) return;

    if(!createDevice(vkPhysicalDevice, queueFamilyIndex, queuePresentIndex)) return;
    
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

    //uint32_t index = 0;
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

// ***
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

    if(vkCreateDevice(device, &createInfo, nullptr, &vkDevice) != VK_SUCCESS) return false;

    vkGetDeviceQueue(vkDevice, queuePresentIndex, 0, &vkPresentQueue);

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
    //bool setPresent = false, setFamily = false;
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

/*
bool vulkan::vulkan::createWaylandWindow()
{
    //wlDisplay = wl_display_connect(NULL);
    //if(!wlDisplay) return false;

    return true;

        return false;
    
    VkWaylandSurfaceCreateInfoKHR createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    //createInfo.display = wlDisplay;

    VkSurfaceKHR vkSurface;
    return (vkCreateWaylandSurfaceKHR(vkInstance,&createInfo, nullptr, &vkSurface) == VK_SUCCESS);\
    
    // ****
}
*/

bool vulkan::vulkan::createWindow(uint32_t index)
{
    int x = 0, y = 0;
    int width = 500, height = 500;

	XInitThreads();
	
	display = XOpenDisplay(NULL);
/*
	GLint glxAttribs[] = {
		GLX_RGBA,
		GLX_DOUBLEBUFFER,
		GLX_DEPTH_SIZE,     24,
		GLX_STENCIL_SIZE,   8,
		GLX_RED_SIZE,       8,
		GLX_GREEN_SIZE,     8,
		GLX_BLUE_SIZE,      8,
		GLX_SAMPLE_BUFFERS, 0,
		GLX_SAMPLES,        0,
		None
	};

	visual = glXChooseVisual(display, (int)deviceID, glxAttribs);
*/
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
//   InputOutput, 
    CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &windowAttrib);

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
    vkSurface = VK_NULL_HANDLE;

    display = NULL;
    //window = NULL;
}

void vulkan::vulkan::cleanup()
{
    if (display != NULL)
	{
		XFreeColormap(display, windowAttrib.colormap);
		XDestroyWindow(display, window);
		XCloseDisplay(display);
	}

    if(vkSurface != VK_NULL_HANDLE) vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);
    if(vkDevice != VK_NULL_HANDLE) vkDestroyDevice(vkDevice, nullptr);
    if(vkInstance != VK_NULL_HANDLE) vkDestroyInstance(vkInstance, nullptr);
}