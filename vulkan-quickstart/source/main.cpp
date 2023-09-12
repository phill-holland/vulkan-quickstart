#include <chrono>
#include <thread>
#include "vulkan.h"
#include "primatives/vertex.h"
#include "primatives/mesh.h"
#include "primatives/matrix.h"
#include "buffer.h"
#include "constants.h"
#include <iostream>

using namespace vulkan;

int basicVertexAndFragmentShaders()
{
	vulkan::vulkan v;
	
	shader::shader *vertex = v.createShader(shader::parameters(std::string("assets/shaders/compiled/vert.spv"), shader::TYPE::vertex, 3 ));
	if(vertex == NULL) return 0;

	shader::shader *fragment = v.createShader(shader::parameters(std::string("assets/shaders/compiled/frag.spv"), shader::TYPE::fragment));
	if(fragment == NULL) return 0;

	std::vector<shader::shader*> shaders;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	std::vector<mesh*> meshes;
	std::vector<buffer*> buffers;

	pipeline *pipeline = v.createPipeline(shaders, meshes, buffers, NULL);
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
	
	shader::parameters params(std::string("assets/shaders/compiled/basic.spv"), shader::TYPE::vertex);
	params.vertexInputDescriptions.inputBindingDescription = primatives::vertex::getBindingDescription();
	params.vertexInputDescriptions.inputAttributeDescriptions = primatives::vertex::getAttributeDescriptions();

	shader::shader *vertex = v.createShader(params);
	if(vertex == NULL) return 0;
	
	shader::shader *fragment = v.createShader(shader::parameters(std::string("assets/shaders/compiled/frag.spv"), shader::TYPE::fragment));
	if(fragment == NULL) return 0;

	std::vector<shader::shader*> shaders;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	std::vector<mesh*> meshes;

	primatives::mesh pmesh;
	pmesh.vertices = {{{0.0f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
    				  {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    				  {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}};

	mesh *triangle = v.createMesh(pmesh);
	if(triangle == NULL) return 0;

	meshes.push_back(triangle);

	std::vector<buffer*> buffers;

	pipeline *pipeline = v.createPipeline(shaders, meshes, buffers, NULL);
	if(pipeline == NULL) return 0;

	while(true)
	{
		pipeline->render();
	}

	return 0;
}

int basicLoadObjMeshShaders()
{
	primatives::mesh pmesh;
	if(!pmesh.load("assets/meshes/triangle.obj")) return 0;

	vulkan::vulkan v;
	
	shader::parameters params(std::string("assets/shaders/compiled/mesh.spv"), shader::TYPE::vertex);
	params.vertexInputDescriptions.inputBindingDescription = primatives::vertex::getBindingDescription();
	params.vertexInputDescriptions.inputAttributeDescriptions = primatives::vertex::getAttributeDescriptions();

	shader::shader *vertex = v.createShader(params);
	if(vertex == NULL) return 0;
	
	shader::shader *fragment = v.createShader(shader::parameters(std::string("assets/shaders/compiled/frag.spv"), shader::TYPE::fragment));
	if(fragment == NULL) return 0;

	std::vector<shader::shader*> shaders;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	std::vector<mesh*> meshes;

	mesh *triangle = v.createMesh(pmesh);
	if(triangle == NULL) return 0;

	meshes.push_back(triangle);

	std::vector<buffer*> buffers;

	primatives::matrices::translation a({-0.3f,0.3f,0.0f});

	vulkan::buffer *buffer = v.createBuffer(&a, sizeof(primatives::matrices::translation));
	buffers.push_back(buffer);

	::vulkan::constants constants;
	constants.m = primatives::matrices::translation({0.3f,0.3f,0.0f});

	pipeline *pipeline = v.createPipeline(shaders, meshes, buffers, &constants);
	if(pipeline == NULL) return 0;

	float offset = -0.3f;
	while(true)
	{
		pipeline->render();

		a.x(offset);
		offset += 0.0001f;
		buffer->update();
	}

	return 0;
}

int basicMeshProjection()
{
	vulkan::vulkan v;
	
	shader::parameters params(std::string("assets/shaders/compiled/projection.spv"), shader::TYPE::vertex);
	params.vertexInputDescriptions.inputBindingDescription = primatives::vertex::getBindingDescription();
	params.vertexInputDescriptions.inputAttributeDescriptions = primatives::vertex::getAttributeDescriptions();

	shader::shader *vertex = v.createShader(params);
	if(vertex == NULL) return 0;
	
	shader::shader *fragment = v.createShader(shader::parameters(std::string("assets/shaders/compiled/frag.spv"), shader::TYPE::fragment));
	if(fragment == NULL) return 0;

	std::vector<shader::shader*> shaders;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	std::vector<mesh*> meshes;

	primatives::mesh pmesh;
	pmesh.vertices = {{{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
    				  {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    				  {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
					  {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
					  {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},

					  {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
					  {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
					  {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}
					  };
	
	mesh *triangle = v.createMesh(pmesh);
	if(triangle == NULL) return 0;

	meshes.push_back(triangle);

	std::vector<buffer*> buffers;

	class transformations
	{
	public:
		primatives::matrices::matrix4x4 world;
		primatives::matrices::matrix4x4 object;
	};

	transformations t;
	t.world = primatives::matrices::translation({0.0f,0.0f,-2.0f});
	t.object = primatives::matrices::rotation::z(0.0f);

	vulkan::buffer *buffer = v.createBuffer(&t, sizeof(transformations));
	buffers.push_back(buffer);

	float fov = 90.0f;
	float near = 0.1, far = 100.0;
	float ar = (600.0f / 800.0f);

	::vulkan::constants constants;
	constants.m = primatives::matrices::projection(fov, ar, near, far);

	pipeline *pipeline = v.createPipeline(shaders, meshes, buffers, &constants);
	if(pipeline == NULL) return 0;

	float angle = 0.0f;

	while(true)
	{
		pipeline->render();

		angle += 0.0001f;
		t.object.copy(primatives::matrices::rotation::x(angle));
		buffer->update();
	}

	return 0;
}

int main(int argc, char *argv[])
{
	//basicVertexAndFragmentShaders();
	//basicMeshShaders();
	//basicLoadObjMeshShaders();
	basicMeshProjection();

	return 0;
}