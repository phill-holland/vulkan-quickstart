#include <chrono>
#include <thread>
#include "vulkan.h"

int main(int argc, char *argv[])
{
	vulkan::vulkan v;
	vulkan::shader *vertex = v.createShader(std::string("shaders/vert.spv"));
	if(vertex == NULL) return 0;

	vulkan::shader *fragment = v.createShader(std::string("shaders/frag.spv"));
	if(fragment == NULL) return 0;

	vulkan::pipeline *pipeline = v.createPipeline(vertex, fragment);
	if(pipeline == NULL) return 0;

	while(true)
	{
		pipeline->render();
	}

	return 0;
}
