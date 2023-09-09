#include <chrono>
#include <thread>
#include "vulkan.h"
#include "vertex.h"

using namespace vulkan;

int basicVertexAndFragmentShaders()
{
	vulkan::vulkan v;
	
	shader::shader *vertex = v.createShader(shader::parameters(std::string("shaders/vert.spv"), shader::TYPE::vertex, 3 ));
	if(vertex == NULL) return 0;

	shader::shader *fragment = v.createShader(shader::parameters(std::string("shaders/frag.spv"), shader::TYPE::fragment));
	if(fragment == NULL) return 0;

	std::vector<shader::shader*> shaders;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	std::vector<mesh*> meshes;

	pipeline *pipeline = v.createPipeline(shaders, meshes);
	if(pipeline == NULL) return 0;

	while(true)
	{
		pipeline->render();
	}

	return 0;
}

int basicMeshShaders()
{
	vulkan::vulkan v;
	
	shader::parameters params(std::string("shaders/mesh.spv"), shader::TYPE::vertex);
	params.vertexInputDescriptions.inputBindingDescription = primatives::vertex::getBindingDescription();
	params.vertexInputDescriptions.inputAttributeDescriptions = primatives::vertex::getAttributeDescriptions();

	shader::shader *vertex = v.createShader(params);
	if(vertex == NULL) return 0;
	
	shader::shader *fragment = v.createShader(shader::parameters(std::string("shaders/frag.spv"), shader::TYPE::fragment));
	if(fragment == NULL) return 0;

	std::vector<shader::shader*> shaders;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	std::vector<mesh*> meshes;

	primatives::mesh pmesh;
	pmesh.vertices = {{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
    				{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    				{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

	mesh *triangle = v.createMesh(pmesh);
	if(triangle == NULL) return 0;

	meshes.push_back(triangle);

	pipeline *pipeline = v.createPipeline(shaders, meshes);
	if(pipeline == NULL) return 0;

	while(true)
	{
		pipeline->render();
	}

	return 0;
}

int main(int argc, char *argv[])
{
	//basicVertexAndFragmentShaders();
	basicMeshShaders();

	return 0;
}
