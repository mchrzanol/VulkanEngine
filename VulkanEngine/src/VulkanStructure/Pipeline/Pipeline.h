#pragma once

#include "Libraries.h"

class pipeline {
private:
    struct pipelineData {
        VkPipeline pipeline;
        VkPipelineLayout pipelineLayout;
    };


    void createGraphicsPipeline(VkDevice device, std::vector<char> vertShaderCode, std::vector<char> fragShaderCode, std::string name, std::vector<VkDescriptorSetLayout> descriptorSetLayouts, VkPrimitiveTopology topology);
public:

    void createRenderPass(VkDevice device, VkFormat format, VkFormat swapChainImageFormat);

    void createGraphicsPipeline(VkDevice device, std::string name, std::string vertShaderPath, std::string fragShaderPath, std::vector<VkDescriptorSetLayout> descriptorSetLayouts, VkPrimitiveTopology topology);

    void cleanup(VkDevice device);

public:
    std::map<std::string, pipelineData> graphicsPipeline;
    VkRenderPass renderPass = nullptr;

};