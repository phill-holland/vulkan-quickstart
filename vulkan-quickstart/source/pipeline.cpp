#include "pipeline.h"
#include "vulkan.h"
#include "primatives/matrix.h"
#include <stdio.h>

bool vulkan::pipeline::render()
{
    uint32_t imageIndex;
    vkAcquireNextImageKHR(device->vkDevice, device->vkSwapChain, UINT64_MAX, vkImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { vkImageAvailableSemaphore };
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = { vkRenderFinishedSemaphore };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(device->vkGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) return false;

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {device->vkSwapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    vkQueuePresentKHR(device->vkPresentQueue, &presentInfo);

    return true;
}

bool vulkan::pipeline::create(vulkan *device, std::vector<shader::shader*> shaders, std::vector<mesh*> meshes, std::vector<buffer*> buffers, ::vulkan::constants *constants)
{
    this->device = device;

    VkDevice vkDevice = device->vkDevice;
    VkFormat vkFormat = device->vkSurfaceFormat.format;
    VkExtent2D vkExtent = device->vkExtent;

    if(!createDepthImage(vkDevice, vkExtent)) return false;
    if(!createRenderPass(vkDevice, vkFormat)) return false;
    if(!createBufferDescriptors(vkDevice, buffers)) return false;
    if(!createPipelineLayout(vkDevice, constants)) return false;
    if(!createPipeline(vkDevice, vkExtent, shaders)) return false;
    if(!createFrameBuffers(vkDevice, vkExtent, device->swapChainImageViews)) return false;
    if(!createCommandPool(vkDevice, device->queueFamilyIndex)) return false;
    if(!createCommandBuffers(vkDevice)) return false;
    if(!bindCommandQueue(shaders, meshes, buffers, constants)) return false;

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if(vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &vkImageAvailableSemaphore) != VK_SUCCESS) return false;
    if(vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &vkRenderFinishedSemaphore) != VK_SUCCESS) return false;

    return true;
}

void vulkan::pipeline::destroy()
{
    VkDevice vkDevice = device->vkDevice;

    vkDestroySemaphore(vkDevice, vkRenderFinishedSemaphore, nullptr);
    vkDestroySemaphore(vkDevice, vkImageAvailableSemaphore, nullptr);

    vkDestroyCommandPool(vkDevice, vkCommandPool, nullptr);

    for (auto framebuffer : swapChainFramebuffers)
    {
        vkDestroyFramebuffer(vkDevice, framebuffer, nullptr);
    }

    vkDestroyDescriptorPool(vkDevice, vkDescriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(vkDevice, vkObjectSetLayout, nullptr);
    vkDestroyDescriptorSetLayout(vkDevice, vkGlobalSetLayout, nullptr);
    
    vkDestroyPipeline(vkDevice, vkGraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(vkDevice, vkPipelineLayout, nullptr);
    vkDestroyRenderPass(vkDevice, vkRenderPass, nullptr);

    vkDestroyImageView(vkDevice, vkDepthImageView, nullptr);
    vkFreeMemory(vkDevice, vkDepthImageMemory, nullptr);
    vkDestroyImage(vkDevice, vkDepthImage, nullptr);
}

bool vulkan::pipeline::createDepthImage(VkDevice vkDevice, VkExtent2D vkExtent)
{    
    VkExtent3D depthExtent = {
        vkExtent.width,
        vkExtent.height,
        1
    };

    VkImageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.imageType = VK_IMAGE_TYPE_2D;
    
    createInfo.format = VK_FORMAT_D32_SFLOAT;
    createInfo.extent = depthExtent;

    createInfo.mipLevels = 1;
    createInfo.arrayLayers = 1;
    createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    createInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if(vkCreateImage(vkDevice, &createInfo, nullptr, &vkDepthImage) != VK_SUCCESS) return false;

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(vkDevice, vkDepthImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    
    if(vkAllocateMemory(vkDevice, &allocInfo, nullptr, &vkDepthImageMemory) != VK_SUCCESS) return false;
    vkBindImageMemory(vkDevice, vkDepthImage, vkDepthImageMemory, 0);

    VkImageViewCreateInfo createViewInfo = {};

    createViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createViewInfo.pNext = nullptr;
    createViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createViewInfo.image = vkDepthImage;
    createViewInfo.format = VK_FORMAT_D32_SFLOAT;
    createViewInfo.subresourceRange.baseMipLevel = 0;
    createViewInfo.subresourceRange.levelCount = 1;
    createViewInfo.subresourceRange.baseArrayLayer = 0;
    createViewInfo.subresourceRange.layerCount = 1;
    createViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    
    if(vkCreateImageView(vkDevice, &createViewInfo, nullptr, &vkDepthImageView) != VK_SUCCESS) return false;
    
    return true;
}

bool vulkan::pipeline::createRenderPass(VkDevice vkDevice, VkFormat vkFormat)
{
    VkAttachmentDescription colourAttachment{};
    colourAttachment.format = vkFormat;
    colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colourAttachmentRef{};
    colourAttachmentRef.attachment = 0;
    colourAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.flags = 0;
    depthAttachment.format = VK_FORMAT_D32_SFLOAT;
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colourAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkAttachmentDescription attachments[2] = { colourAttachment, depthAttachment };

    VkSubpassDependency colourDependency{};
    colourDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    colourDependency.dstSubpass = 0;
    colourDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    colourDependency.srcAccessMask = 0;
    colourDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    colourDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkSubpassDependency depthDependency{};
    depthDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    depthDependency.dstSubpass = 0;
    depthDependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    depthDependency.srcAccessMask = 0;
    depthDependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    depthDependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkSubpassDependency dependencies[2] = { colourDependency, depthDependency };

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 2;
    renderPassInfo.pAttachments = &attachments[0];
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 2;
    renderPassInfo.pDependencies = &dependencies[0];

    if (vkCreateRenderPass(vkDevice, &renderPassInfo, nullptr, &vkRenderPass) != VK_SUCCESS) return false;

    return true;
}

bool vulkan::pipeline::createPipelineLayout(VkDevice vkDevice, ::vulkan::constants *constants)
{
    VkPushConstantRange push_constant;
    push_constant.offset = 0;
    push_constant.size = sizeof(::vulkan::constants);
    push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;    

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    
    VkDescriptorSetLayout setLayouts[] = { vkGlobalSetLayout, vkObjectSetLayout };

    pipelineLayoutInfo.setLayoutCount = 2;
    pipelineLayoutInfo.pSetLayouts = setLayouts;
    
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    
    if(constants != NULL)
    {
        pipelineLayoutInfo.pPushConstantRanges = &push_constant;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
    }
    
    if (vkCreatePipelineLayout(vkDevice, &pipelineLayoutInfo, nullptr, &vkPipelineLayout) != VK_SUCCESS) return false;

    return true;
}

bool vulkan::pipeline::createPipeline(VkDevice vkDevice, VkExtent2D vkExtent, std::vector<shader::shader*> shaders)
{
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    std::vector<VkPipelineShaderStageCreateInfo> shaderInfo;

    for(std::vector<shader::shader*>::iterator it = shaders.begin(); it < shaders.end(); ++it)
    {
        shader::parameters *params = &(*it)->_params;

        VkPipelineShaderStageCreateInfo shaderStageInfo{};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        
        if(params->type == shader::TYPE::vertex) 
        {
            shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; 
            if(!params->vertexInputDescriptions.is_empty())
            {
                vertexInputInfo.vertexBindingDescriptionCount = 1;
                vertexInputInfo.pVertexBindingDescriptions = &(params->vertexInputDescriptions.inputBindingDescription);
                
                vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(params->vertexInputDescriptions.inputAttributeDescriptions.size());
                vertexInputInfo.pVertexAttributeDescriptions = params->vertexInputDescriptions.inputAttributeDescriptions.data();
            }       
        }
        else shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;

        shaderStageInfo.module = (*it)->vkShader;
        shaderStageInfo.pName = params->name.c_str();

        shaderInfo.push_back(shaderStageInfo);
    }

    VkPipelineShaderStageCreateInfo *shaderStages = &shaderInfo[0];

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) vkExtent.width;
    viewport.height = (float) vkExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = vkExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.pNext = nullptr;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f;
    depthStencil.maxDepthBounds = 1.0f;
    depthStencil.stencilTestEnable = VK_FALSE;

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shaderInfo.size();
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.layout = vkPipelineLayout;
    pipelineInfo.renderPass = vkRenderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vkGraphicsPipeline) != VK_SUCCESS) return false;

    return true;
}

bool vulkan::pipeline::createFrameBuffers(VkDevice vkDevice, VkExtent2D vkExtent, std::vector<VkImageView> &swapChainImageViews)
{
     swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        VkImageView attachments[2] = { swapChainImageViews[i], vkDepthImageView };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = vkRenderPass;
        framebufferInfo.attachmentCount = 2;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = vkExtent.width;
        framebufferInfo.height = vkExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(vkDevice, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) return false;
    }

    return true;
}

bool vulkan::pipeline::createCommandPool(VkDevice vkDevice, uint32_t queueFamilyIndex)
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    poolInfo.flags = 0;

    if (vkCreateCommandPool(vkDevice, &poolInfo, nullptr, &vkCommandPool) != VK_SUCCESS) return false;

    return true;
}

bool vulkan::pipeline::createCommandBuffers(VkDevice vkDevice)
{
    commandBuffers.resize(swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = vkCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(vkDevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS) return false;

    return true;
}

bool vulkan::pipeline::indirectDraw(std::vector<shader::shader*> shaders, std::vector<mesh*> meshes, std::vector<buffer*> buffers, ::vulkan::constants *constants)
{        
    indirectCommands.clear();

    int i = 0;
    for(std::vector<mesh*>::iterator it = meshes.begin(); it < meshes.end(); ++it)
    {
        mesh *temp = (*it);

        VkDrawIndirectCommand cmd {};
        cmd.vertexCount = temp->length;
        cmd.instanceCount = 2;
        cmd.firstVertex = 0;
        cmd.firstInstance = i;

        indirectCommands.push_back(cmd);

        ++i;
    }
        
    indirectCommandBuffer.create(device, indirectCommands.data(), meshes.size() * sizeof(VkDrawIndexedIndirectCommand), buffer::TYPE::indirect);
    indirectCommandBuffer.update();
    
    return true;
}

bool vulkan::pipeline::update(int mesh_index, int instance_count, int first_instance)
{
    indirectCommands[mesh_index].instanceCount = instance_count;
    indirectCommands[mesh_index].firstInstance = first_instance;
    indirectCommandBuffer.update();

    return true;
}

bool vulkan::pipeline::bindCommandQueue(std::vector<shader::shader*> shaders, std::vector<mesh*> meshes, std::vector<buffer*> buffers, ::vulkan::constants *constants)
{
    indirectDraw(shaders, meshes, buffers, constants);

    for (size_t i = 0; i < commandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) return false;

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vkRenderPass;
        renderPassInfo.framebuffer = swapChainFramebuffers[i];

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = device->vkExtent;

        VkClearValue clearColour = {0.0f, 0.0f, 0.0f, 1.0f};        
        VkClearValue clearDepth;
        clearDepth.depthStencil.depth = 1.0f;

        VkClearValue clearValues[] = { clearColour, clearDepth };

        renderPassInfo.clearValueCount = 2;
        renderPassInfo.pClearValues = &clearValues[0];

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkGraphicsPipeline);
        
        if(constants != NULL)
        {
            vkCmdPushConstants(commandBuffers[i], vkPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(primatives::matrices::matrix4x4), constants);//&constants[0]);//t);//constants.data());
        }
        
        int kk = 0;
        for(std::vector<buffer*>::iterator it = buffers.begin(); it < buffers.end(); ++it)
        {            
            buffer *b = (*it);
            b->update();

            vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,  vkPipelineLayout, kk, 1, &b->vkDescriptorSet, 0, nullptr);

            ++kk;
        }
        
        int jj = 0;
        for(std::vector<mesh*>::iterator it = meshes.begin(); it < meshes.end(); ++it)
        {
            mesh *temp = (*it);
            VkBuffer vertexBuffers[] = { temp->vkVertexBuffer };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

            VkDeviceSize indirect_offset = jj * sizeof(VkDrawIndirectCommand);
            uint32_t indirect_stride = sizeof(VkDrawIndirectCommand);
            
            
            /*
            for(int instances = 0; instances < 2; ++instances)
            {                
                vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(temp->length), 1, 0, instances);         
            }
            */
            
            
            vkCmdDrawIndirect(commandBuffers[i], indirectCommandBuffer.vkBuffer, indirect_offset, 1, indirect_stride);

            ++jj;
        }
        
        int vertices = 0;
        for(std::vector<shader::shader*>::iterator it = shaders.begin(); it < shaders.end(); ++it)
        {
            shader::parameters *params = &(*it)->_params;
            if(params->type == shader::TYPE::vertex)
            {
                if(params->vertices > 0) vertices = params->vertices;
            }
        }

        if(vertices > 0) 
        {
            //vkCmdDraw(commandBuffers[i], vertices, 0, 0, 0);
            vkCmdDraw(commandBuffers[i], vertices, 1, 0, 0);
        }

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) return false;
    }

    return true;
}

bool vulkan::pipeline::createBufferDescriptors(VkDevice vkDevice, std::vector<buffer*> buffers)
{
    auto isUniform = [](buffer *b) { return b->type() == buffer::TYPE::uniform; };
    auto isStorage = [](buffer *b) { return b->type() == buffer::TYPE::storage; };

    VkDescriptorSetLayoutBinding globalBindings = {};
    globalBindings.binding = 0;
    globalBindings.descriptorCount = 1;
    globalBindings.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    globalBindings.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo globalInfo = {};
    globalInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    globalInfo.pNext = nullptr;
    globalInfo.bindingCount = 1;
    globalInfo.flags = 0;
    globalInfo.pBindings = &globalBindings;

    if(vkCreateDescriptorSetLayout(vkDevice, &globalInfo, nullptr, &vkGlobalSetLayout) != VK_SUCCESS) return false;

    VkDescriptorSetLayoutBinding storageBindings = {};
    storageBindings.binding = 0;
    storageBindings.descriptorCount = 1;
    storageBindings.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    storageBindings.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo storageInfo = {};
    storageInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    storageInfo.pNext = nullptr;
    storageInfo.bindingCount = 1;
    storageInfo.flags = 0;
    storageInfo.pBindings = &storageBindings;

    if(vkCreateDescriptorSetLayout(vkDevice, &storageInfo, nullptr, &vkObjectSetLayout) != VK_SUCCESS) return false;
    
    std::vector<VkDescriptorPoolSize> sizes =
    {
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 10 }
    };

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = 0;
    pool_info.maxSets = 10;
    pool_info.poolSizeCount = (uint32_t)sizes.size();
    pool_info.pPoolSizes = sizes.data();

    if(vkCreateDescriptorPool(vkDevice, &pool_info, nullptr, &vkDescriptorPool) != VK_SUCCESS) return false;

    for(std::vector<buffer*>::iterator it = buffers.begin(); it < buffers.end(); ++it)
    {
        buffer *b= *it;

        VkDescriptorSetAllocateInfo alloc_info = {};
        alloc_info.pNext = nullptr;
        alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info.descriptorPool = vkDescriptorPool;
        alloc_info.descriptorSetCount = 1;
        if(b->_type == buffer::TYPE::uniform) alloc_info.pSetLayouts = &vkGlobalSetLayout;
        else if(b->_type == buffer::TYPE::storage) alloc_info.pSetLayouts = &vkObjectSetLayout;
        else if(b->_type == buffer::TYPE::count) alloc_info.pSetLayouts = &vkGlobalSetLayout;

        if(vkAllocateDescriptorSets(vkDevice, &alloc_info, &b->vkDescriptorSet) != VK_SUCCESS) return false;

        VkDescriptorBufferInfo buffer_info;
        buffer_info.buffer = b->vkBuffer;
        buffer_info.offset = 0;
        buffer_info.range = b->_length;

        VkWriteDescriptorSet setWrite = {};
        setWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        setWrite.pNext = nullptr;
        setWrite.dstBinding = 0;
        setWrite.dstSet = b->vkDescriptorSet;
        setWrite.descriptorCount = 1;
        if(b->_type == buffer::TYPE::uniform) setWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        else if(b->_type == buffer::TYPE::storage) setWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        else if(b->_type == buffer::TYPE::count) setWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

        setWrite.pBufferInfo = &buffer_info;

        vkUpdateDescriptorSets(vkDevice, 1, &setWrite, 0, nullptr);
    }

    return true;
}
