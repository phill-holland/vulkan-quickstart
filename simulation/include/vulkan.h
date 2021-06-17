#include <X11/Xlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xlib.h>
#include <vector>
#include "shader.h"
#include "pipeline.h"

#ifndef _VULKAN
#define _VULKAN

namespace vulkan
{
    class pipeline;

    class vulkan
    {
        const static int WIDTH = 800;
        const static int HEIGHT = 600;

        friend class pipeline;

    private:
        int width, height;

        VkInstance vkInstance;
        VkDevice vkDevice;
        VkSurfaceKHR vkSurface;
        VkQueue vkPresentQueue, vkGraphicsQueue;
        VkSwapchainKHR vkSwapChain;
        VkExtent2D vkExtent;

        VkSurfaceFormatKHR vkSurfaceFormat;
        VkPresentModeKHR vkPresentMode;

        std::vector<VkImageView> swapChainImageViews;

        std::vector<shader*> shaders;
        std::vector<pipeline*> pipelines;

        XSetWindowAttributes windowAttrib;
        Display *display;
        Window window;

        uint32_t queueFamilyIndex;
        uint32_t queuePresentIndex;

        bool init;

    public:
    // pass in device index, width and height
        vulkan() { makeNull(); reset(); }
        ~vulkan() { cleanup(); }

        void reset();
        bool initalised() { return init; }

        shader *createShader(std::string filename);
        pipeline *createPipeline(shader *vertex, shader *fragment);

    protected:
        bool createInstance(bool enableLayer = false);
        bool findLayers(const std::vector<const char*> &validationLayers);
        VkPhysicalDevice findDevice(uint32_t index);
        bool createDevice(VkPhysicalDevice &device, uint32_t queueFamilyIndex, uint32_t queuePresentIndex);
        bool findQueueFamily(VkPhysicalDevice &device, uint32_t &queueFamilyIndex);
        bool findQueuePresentationFamily(VkPhysicalDevice &device, uint32_t &queuePresentIndex);
        bool createSurface(VkPhysicalDevice &device, uint32_t queue);
        bool findDeviceExtensionSupport(VkPhysicalDevice &device);
        bool createSwapChain(VkPhysicalDevice &device);
        bool createImageViews();
        VkExtent2D findSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    //protected:
    //    bool createPipeline();

    protected:
        bool createWindow(uint32_t index);

    protected:
        void makeNull();
        void cleanup();
    };
};

#endif