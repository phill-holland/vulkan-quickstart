#include <vulkan/vulkan.h>
#include <string>
#include <vector>

#ifndef _VULKAN_SHADER
#define _VULKAN_SHADER

namespace vulkan
{
    class vulkan;
    class pipeline;
        
    namespace shader
    {        
        enum TYPE
        {
            fragment = 0,
            vertex = 1
        };

        class inputs
        {
        public:
            VkVertexInputBindingDescription inputBindingDescription;
            std::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions;

        public:
            bool is_empty() { return inputAttributeDescriptions.size() == 0; }
        };

        class parameters
        {
        public:            
            std::string filename;
            std::string name;
            int vertices;
            TYPE type;

        public:
            inputs vertexInputDescriptions;

        public:
            parameters() { }
            parameters(std::string _filename, TYPE _type, int _vertices = 0, std::string _name = "main")
            {
                filename = _filename;
                type = _type;
                vertices = _vertices;
                name = _name;
            }
        };

        class shader
        {   
            VkShaderModule vkShader;

            parameters _params;

        private:
            friend class ::vulkan::vulkan;
            friend class ::vulkan::pipeline;

        private:
            shader() { }

            bool create(VkDevice vkDevice, parameters params);
            void destroy(VkDevice vkDevice)
            {
                vkDestroyShaderModule(vkDevice, vkShader, nullptr);
            }
        };
    };
};

#endif