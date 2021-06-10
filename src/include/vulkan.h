#include <vulkan/vulkan.h>
#include <vector>

#ifndef _VULKAN
#define _VULKAN

namespace vulkan
{
    class vulkan
    {
        VkInstance instance;
        VkPhysicalDevice physicalDevice;

        bool init;

    public:
        vulkan() { makeNull(); reset(); }
        ~vulkan() { cleanup(); }

        void reset();
        bool initalised() { return init; }

    protected:
        bool create();
        bool layers(const std::vector<const char*> &validationLayers);
        bool device();

    protected:
        void makeNull();
        void cleanup();
    };
};

#endif