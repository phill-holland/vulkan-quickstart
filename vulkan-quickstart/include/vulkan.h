#include <X11/Xlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xlib.h>
#include <vector>
#include "shader.h"
#include "pipeline.h"
#include "mesh.h"
#include "primatives/vertex.h"
#include "primatives/mesh.h"
#include "constants.h"
#include "buffer.h"

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
        friend class mesh;
        friend class buffer;

    private:
        int width, height;

        VkPhysicalDevice vkPhysicalDevice;

        VkInstance vkInstance;
        VkDevice vkDevice;
        VkSurfaceKHR vkSurface;
        VkQueue vkPresentQueue, vkGraphicsQueue;
        VkSwapchainKHR vkSwapChain;
        VkExtent2D vkExtent;

        VkSurfaceFormatKHR vkSurfaceFormat;
        VkPresentModeKHR vkPresentMode;

        std::vector<VkImageView> swapChainImageViews;

        std::vector<shader::shader*> shaders;
        std::vector<mesh*> meshes;
        std::vector<buffer*> buffers;
        std::vector<pipeline*> pipelines;

        XSetWindowAttributes windowAttrib;
        Display *display;
        Window window;

        uint32_t queueFamilyIndex;
        uint32_t queuePresentIndex;

        bool init;

    public:
        vulkan() { makeNull(); reset(); }
        ~vulkan() { cleanup(); }

        void reset();
        bool initalised() { return init; }

        shader::shader *createShader(shader::parameters params);
        mesh *createMesh(primatives::mesh vertices);
        buffer *createBuffer(void *data, size_t size);

    public:
        pipeline *createPipeline(std::vector<shader::shader*> shaders, std::vector<mesh*> mesh, std::vector<buffer*> buffers, constants *constants);

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

    protected:
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    protected:
        bool createWindow(uint32_t index);

    protected:
        void makeNull();
        void cleanup();
    };
};

#endif