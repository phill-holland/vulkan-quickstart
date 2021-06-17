#include <chrono>
#include <thread>
#include "vulkan.h"
//#include "canvas.h"
//#include "image.h"
//#include "pstring.h"
//#include "plasma.h"
//#include "cube.h"


/*
void Run()//long type, bool override, long device)
{
	//configuration::configuration config;
	
	//if (config.load(CONFIGURATION))
	//{		
		canvas *c = new canvas();
		if(c == NULL) return;
		if(!c->initalised()) return;

		if(!c->start()) return;

		image *i = image::createImage(string("images/test.jpg"));
		if(i == NULL) return;
		if(!i->initalised()) return;

		plasma *p = new plasma(6,40,163,36);
		if(p == NULL) return;

		cube *square = new cube();
		if(square == NULL) return;
		if(!square->initalised()) return;

		c->render(2,2,i);

		bool started = true;
		if (started)
		{
#ifdef WIN32
				MSG message;
				while (GetMessage(&message, NULL, 0, 0) > 0)
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				};
#elif defined __linux__
				while (1)
				{
					p->update();
					c->render(0,0,p);
					square->render();
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				};
#endif
		}

		if(square != NULL) delete square;
		if(p != NULL) delete p;
		if(i != NULL) delete i;
		if(c != NULL) delete c;
}
*/
void test()
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

	std::chrono::milliseconds milliseconds = std::chrono::milliseconds(2000);
	std::this_thread::sleep_for(milliseconds);

	std::chrono::high_resolution_clock::time_point interval = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(now - interval);

	double temp = time_span.count();
}

int main(int argc, char *argv[])
{
	//fmt::print("Hello, {}!","world");
//	Run();
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

	//v.create();

	return 0;
}
