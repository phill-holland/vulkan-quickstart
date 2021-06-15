#include <X11/Xlib.h>
#include <vulkan/vulkan.h>
//#include <vulkan/vulkan_wayland.h>

#include <vulkan/vulkan_xlib.h>
#include <vector>

//#include <wayland-client.h>

#ifndef _VULKAN
#define _VULKAN

namespace vulkan
{
    class vulkan
    {
        VkInstance vkInstance;
        VkDevice vkDevice;
        VkSurfaceKHR vkSurface;
        VkQueue vkPresentQueue;

        XSetWindowAttributes windowAttrib;
        Display *display;
        Window window;

        uint32_t queueFamilyIndex;
        uint32_t queuePresentIndex;
        //struct wl_display *wlDisplay;

        bool init;

    public:
        vulkan() { makeNull(); reset(); }
        ~vulkan() { cleanup(); }

        void reset();
        bool initalised() { return init; }

    protected:
        bool createInstance();
        bool findLayers(const std::vector<const char*> &validationLayers);
        VkPhysicalDevice findDevice(uint32_t index);
        bool createDevice(VkPhysicalDevice &device, uint32_t queueFamilyIndex, uint32_t queuePresentIndex);
        bool findQueueFamily(VkPhysicalDevice &device, uint32_t &queueFamilyIndex);
        bool findQueuePresentationFamily(VkPhysicalDevice &device, uint32_t &queuePresentIndex);
        bool createSurface(VkPhysicalDevice &device, uint32_t queue);
        //bool createQueue(uint32_t queueFamilyIndex, uint32_t queuePresentIndex);

    protected:
        //bool createWaylandWindow();
        bool createWindow(uint32_t index);

    protected:
        void makeNull();
        void cleanup();
    };
};

#endif