#pragma once
#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"

class ENGINE_API object {
public:
	VertexBuffer m_Vertices;
	IndexBuffer m_Indices;

public:
	void virtual create() = 0;

	void virtual update() = 0;

	void virtual destroy() = 0;

};

class ObjectVector
{
public:
    template <typename T>
    void RegisterTest(const std::string& name)
    {
        std::cout << "Registering test: " << name << std::endl;
        m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
    }

    std::vector<std::pair<std::string, std::function<object* ()>>> m_Tests;
};