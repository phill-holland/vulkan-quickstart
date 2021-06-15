#include <vulkan/vulkan.h>
#include <vulkan/vulkan_wayland.h>
#include <vulkan/vulkan_xlib.h>
#include <vector>
#include <wayland-client.h>

#ifndef _VULKAN
#define _VULKAN

namespace vulkan
{
    class vulkan
    {
        VkInstance vkInstance;
        VkDevice vkDevice;

        struct wl_display *wlDisplay;

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
        bool createDevice(VkPhysicalDevice &device);
        bool findQueueFamilies(VkPhysicalDevice &device, uint32_t &index);
        bool createSurface();

    protected:
        bool createWaylandWindow();

    protected:
        void makeNull();
        void cleanup();
    };
};

#endif