#pragma once

#include "Libraries.h"

extern GlobalUtl utils;

class pipeline {
private:
    void createGraphicsPipeline(VkDevice device, std::vector<char> vertShaderCode, std::vector<char> fragShaderCode, std::string name, std::vector<VkDescriptorSetLayout> descriptorSetLayouts);
public:

    void createRenderPass(VkDevice device, VkFormat format, VkFormat swapChainImageFormat);

    void createGraphicsPipeline(VkDevice device, std::string name, std::string vertShaderPath, std::string fragShaderPath, std::vector<VkDescriptorSetLayout> descriptorSetLayouts);

    void cleanup(VkDevice device);

public:
    std::map<std::string, VkPipeline> graphicsPipeline;
    VkRenderPass renderPass = nullptr;
    VkPipelineLayout pipelineLayout;

};