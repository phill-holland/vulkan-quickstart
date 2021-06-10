#include "geometry.h"

#ifndef _OPENGL
#define _OPENGL

#ifdef WIN32

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"
//#include <cuda_gl_interop.h>
//#include <cuda_runtime_api.h>
#include "graphics.h"
#include "regions.h"
#include "extensions.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

namespace opengl
{
	namespace std
	{
		class device : public graphics::device
		{
			unsigned int deviceID;

			HDC hdc;
			HGLRC hrc;

		public:
			device(unsigned int id, HDC device, HGLRC context)
			{
				deviceID = id;

				hdc = device;
				hrc = context;
			}
			unsigned int getDeviceID() { return deviceID; }

			void *getDevice() { return (void*)hdc; }
			void *getContext() { return (void*)hrc; }
			void *getWindow() { return (void*)NULL; }

			bool setDevice() { return true; }
		};

		class region : public regions::region
		{
			UINT width, height;
			GLuint texture, pixel;

			regions::context *context;

		public:
			region() { makeNull(); }
			~region() { cleanup(); }

			void reset(unsigned int width, unsigned int height)
			{
				cleanup();

				context = NULL;

				region::width = width; region::height = height;
			}

			void set(regions::context *destination);

			unsigned int getWidth() { return width; }
			unsigned int getHeight() { return height; }

		protected:
			bool initalise(graphics::device *d);
			bool render(graphics::device *d);

		protected:
			void makeNull();
			void cleanup();
		};

		class screen : public graphics::screen
		{
			bool init;

			GLenum program;
			GLenum fragment, vertex;

			unsigned int deviceID;

			unsigned int width, height;
			unsigned int window_width, window_height;

			bool windowed;

			static char *texture_fragment_source, *texture_vertex_source;
			static long increment;

			HDC hdc;
			HGLRC hrc;

			HWND hwnd;

		public:
			const static long x = 0L, y = 50L;

		protected:
			static LRESULT CALLBACK WindowProc(HWND hwnd, unsigned uMsg, WPARAM wparam, LPARAM lparam);

		public:
			screen() { makeNull(); init = false; }
			~screen() { cleanup(); }

			bool initalised() { return init; }

			void reset(unsigned int device, unsigned int width, unsigned int height, bool windowed);

			bool get(graphics::region *destination);
			bool render(graphics::region *source);

			bool title(string source);

		protected:
			HWND createWindow();
			bool createOpenGL();
			bool compile(const char *source, GLenum type, GLenum &result);

			void resizeWindow(unsigned int w, unsigned int h);

		protected:
			void makeNull();
			void cleanup();
		};
	};

	namespace cuda
	{
		class device : public graphics::device
		{
			unsigned int deviceID;

			HDC hdc;
			HGLRC hrc;

		public:
			device(unsigned int id, HDC device, HGLRC context)
			{
				deviceID = id;

				hdc = device;
				hrc = context;
			}
			unsigned int getDeviceID() { return deviceID; }

			void *getDevice() { return (void*)hdc; }
			void *getContext() { return (void*)hrc; }
			void *getWindow() { return (void*)NULL; }

			bool setDevice() { return cudaSetDevice(deviceID) == ::cudaSuccess; }
		};

		class region : public regions::region
		{
			UINT width, height;
			GLuint pbo;

			regions::context *context;

			cudaGraphicsResource *cudaResource;
			cudaGraphicsResource *ppResources[1];

			void *cudaLinearMemory;

		public:
			region() { makeNull(); }
			~region() { cleanup(); }

			void reset(unsigned int width, unsigned int height)
			{
				cleanup();

				context = NULL;

				region::width = width; region::height = height;
			}

			void set(regions::context *destination);

			unsigned int getWidth() { return width; }
			unsigned int getHeight() { return height; }

		protected:
			bool initalise(graphics::device *d);
			bool render(graphics::device *d);

		protected:
			void makeNull();
			void cleanup();
		};

		class screen : public graphics::screen
		{
			bool init;

			GLenum program;
			GLenum fragment, vertex;

			unsigned int deviceID;

			unsigned int width, height;
			unsigned int window_width, window_height;

			bool windowed;

			static char *texture_fragment_source, *texture_vertex_source;
			static long increment;

			HDC hdc;
			HGLRC hrc;

			HWND hwnd;

		public:
			const static long x = 0L, y = 50L;

		protected:
			static LRESULT CALLBACK WindowProc(HWND hwnd, unsigned uMsg, WPARAM wparam, LPARAM lparam);

		public:
			screen() { makeNull(); init = false; }
			~screen() { cleanup(); }

			bool initalised() { return init; }

			void reset(unsigned int device, unsigned int width, unsigned int height, bool windowed);

			bool get(graphics::region *destination);
			bool render(graphics::region *source);

			bool title(string source);

		protected:
			HWND createWindow();
			bool createOpenGL();
			bool compile(const char *source, GLenum type, GLenum &result);

			void resizeWindow(unsigned int w, unsigned int h);

		protected:
			void makeNull();
			void cleanup();
		};
	};
};

#elif defined __linux__

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glext.h>
//#include <cuda_gl_interop.h>
//#include <cuda_runtime_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "graphics.h"
#include "regions.h"
#include "extensions.h"

namespace opengl
{
	namespace std
	{
		class device : public graphics::device
		{
			unsigned int deviceID;

			GLXContext context;
			Display *display;
			Window window;

		public:
			device(unsigned int id, GLXContext context, Display *display, Window window)
			{
				deviceID = id;

				this->context = context;
				this->display = display;
				this->window = window;
			}

			unsigned int getDeviceID() { return deviceID; }

			void *getDevice() { return (void*)display; }
			void *getContext() { return (void*)context; }
			void *getWindow() { return (void*)window; }

			bool setDevice() { return true; }
		};

		class region : public regions::region
		{
			unsigned int width, height;
			GLuint texture, pixel;

			regions::context *context;

		public:
			region() { makeNull(); }
			~region() { cleanup(); }

			void reset(unsigned int width, unsigned int height)
			{
				cleanup();

				context = NULL;

				region::width = width; region::height = height;
			}

			void set(regions::context *destination);

			unsigned int getWidth() { return width; }
			unsigned int getHeight() { return height; }

		protected:
			bool initalise(graphics::device *d);
			bool render(graphics::device *d);

		protected:
			void makeNull();
			void cleanup();
		};

		class screen : public graphics::screen
		{
			bool init;

			GLenum program;
			GLenum fragment, vertex;

			unsigned int deviceID;

			unsigned int width, height;
			unsigned int window_width, window_height;

			bool windowed;

			static const char *texture_fragment_source, *texture_vertex_source;
			static long increment;

			GLXContext context;

			Display *display;
			Window window;

			XVisualInfo *visual;
			XSetWindowAttributes windowAttrib;

		public:
			const static long x = 0L, y = 50L;

		public:
			screen() { makeNull(); init = false; }
			~screen() { cleanup(); }

			bool initalised() { return init; }

			void reset(unsigned int device, unsigned int width, unsigned int height, bool windowed);

			bool get(graphics::region *destination);

			bool render(graphics::region *source);
			bool render(geometry *source);

			bool title(string source);

		protected:
			bool createWindow();
			bool createOpenGL();
			bool compile(const char *source, GLenum type, GLenum &result);

			void resizeWindow(unsigned int w, unsigned int h);

		protected:
			void makeNull();
			void cleanup();
		};
	};
/*
	namespace cuda
	{
		class device : public graphics::device
		{
			unsigned int deviceID;

			GLXContext context;
			Display *display;
			Window window;

		public:
			device(unsigned int id, GLXContext context, Display *display, Window window)
			{
				deviceID = id;
				
				this->context = context;
				this->display = display;
				this->window = window;
			}

			unsigned int getDeviceID() { return deviceID; }

			void *getDevice() { return (void*)display; }
			void *getContext() { return (void*)context; }
			void *getWindow() { return (void*)window; }

			bool setDevice() { return cudaSetDevice(deviceID) == ::cudaSuccess; }
		};

		class region : public regions::region
		{
			unsigned int width, height;
			GLuint pbo;

			regions::context *context;

			cudaGraphicsResource *cudaResource;
			cudaGraphicsResource *ppResources[1];

			void *cudaLinearMemory;

		public:
			region() { makeNull(); }
			~region() { cleanup(); }

			void reset(unsigned int width, unsigned int height)
			{
				cleanup();

				context = NULL;

				region::width = width; region::height = height;
			}

			void set(regions::context *destination);

			unsigned int getWidth() { return width; }
			unsigned int getHeight() { return height; }

		protected:
			bool initalise(graphics::device *d);
			bool render(graphics::device *d);

		protected:
			void makeNull();
			void cleanup();
		};

		class screen : public graphics::screen
		{ 
			bool init;

			GLenum program;
			GLenum fragment, vertex;

			unsigned int deviceID;

			unsigned int width, height;
			unsigned int window_width, window_height;

			bool windowed;

			static const char *texture_fragment_source, *texture_vertex_source;
			static long increment;

			GLXContext context;

			Display *display;
			XVisualInfo *visual;
			XSetWindowAttributes windowAttrib;
			Window window;

		public:
			const static long x = 0L, y = 50L;

		public:
			screen() { makeNull(); init = false; }
			~screen() { cleanup(); }

			bool initalised() { return init; }

			void reset(unsigned int device, unsigned int width, unsigned int height, bool windowed);

			bool get(graphics::region *destination);
			bool render(graphics::region *source);

			bool title(string source);

		protected:
			bool createWindow();
			bool createOpenGL();
			bool compile(const char *source, GLenum type, GLenum &result);

			void resizeWindow(unsigned int w, unsigned int h);

		protected:
			void makeNull();
			void cleanup();
		};
	};
	*/
};

#endif

#endif