#include "opengl.h"
#include <stdio.h>
//#include <cuda_runtime_api.h>
#include "error.h"
#include "log.h"

#ifdef WIN32

void opengl::std::region::set(regions::context *destination)
{
	context = destination;
}

bool opengl::std::region::initalise(graphics::device *d)
{
	if (!d->setDevice())
	{
		setLastError<opengl::std::region>(string("initalise::setDevice"));
		return false;
	}

	if (!wglMakeCurrent((HDC)d->getDevice(), (HGLRC)d->getContext())) 
	{
		setLastError<opengl::std::region>(string("initalise::wglMakeCurrent")); 
		return false;
	}

	extensions::glGenBuffers(1, &pixel);
	extensions::glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixel);
	extensions::glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * 4, NULL, GL_STREAM_DRAW);
	extensions::glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8UI_EXT, width, height, 0, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	wglMakeCurrent(NULL, NULL);

	return true;
}

bool opengl::std::region::render(graphics::device *d)
{
	if (!wglMakeCurrent((HDC)d->getDevice(), (HGLRC)d->getContext()))
	{
		setLastError<opengl::std::region>(string("render::wglMakeCurrent"));
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, texture);
	
	extensions::glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8UI_EXT, width, height, 0, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_BYTE, NULL);

	extensions::glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixel);
	extensions::glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * 4, NULL, GL_STREAM_DRAW);
	void *result = extensions::glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

	context->render(result);

	extensions::glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	extensions::glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	wglMakeCurrent(NULL, NULL);

	return true;
}

void opengl::std::region::makeNull()
{
	texture = 0;
	pixel = 0;
}

void opengl::std::region::cleanup()
{
	if (texture != 0) glDeleteTextures(1, &texture);
	if (pixel != 0) extensions::glDeleteBuffers(1, &pixel);
}

char *opengl::std::screen::texture_vertex_source =
"void main(void)\n"
"{\n"
"	gl_Position = gl_Vertex;\n"
"	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;\n"
"}\n";

char *opengl::std::screen::texture_fragment_source =
"#version 130\n"
"uniform usampler2D texImage;\n"
"void main()\n"
"{\n"
"   vec4 c = texture(texImage, gl_TexCoord[0].xy);\n"
"	gl_FragColor = c / 255.0;\n"
"}\n";

long opengl::std::screen::increment = 0L;

LRESULT CALLBACK opengl::std::screen::WindowProc(HWND hwnd, unsigned uMsg, WPARAM wparam, LPARAM lparam)
{
	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCT *pCreate = (CREATESTRUCT*)lparam;
		opengl::std::screen *ptr = (opengl::std::screen*)pCreate->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)ptr);
	}
	else if (uMsg == WM_DESTROY)
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		PostQuitMessage(0);
	}
	else if ((uMsg == WM_KEYDOWN) && (wparam == VK_ESCAPE))
	{
		DestroyWindow(hwnd);
	}
	else if (uMsg == WM_SETCURSOR)
	{
		opengl::std::screen *ptr = (opengl::std::screen*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (ptr != NULL)
		{
			if (ptr->windowed) SetCursor(LoadCursor(NULL, IDC_ARROW));
			else SetCursor(NULL);
		}
		else SetCursor(NULL);
	}
	else if (uMsg == WM_SIZE)
	{
		opengl::std::screen *ptr = (opengl::std::screen*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (ptr != NULL)
		{
			ptr->resizeWindow(LOWORD(lparam), HIWORD(lparam));
		}
	}

	return DefWindowProc(hwnd, uMsg, wparam, lparam);
}

void opengl::std::screen::reset(unsigned int device, unsigned int width, unsigned int height, bool windowed)
{
	init = false; cleanup();

	deviceID = device;

	screen::width = width; screen::height = height;
	window_width = width; window_height = height;

	screen::windowed = windowed;

	hwnd = createWindow();
	if (hwnd == NULL)
	{
		setLastError<opengl::std::screen>(string("reset::createWindow"));

		return;
	}

	ShowWindow(hwnd, 1);
	UpdateWindow(hwnd);

	if (!createOpenGL())
	{
		setLastError<opengl::std::screen>(string("reset::createOpenGL"));

		return;
	}

	init = true;
}

bool opengl::std::screen::title(string source)
{
	if (hwnd != NULL)
	{
		if (SetWindowText(hwnd, source.c_str()) == TRUE) return true;
	}

	setLastError<opengl::std::screen>(string("title::SetWindowText"));

	return false;
}

HWND opengl::std::screen::createWindow()
{
	HWND window;
	WNDCLASS wc;

	char caption[20] = "GLScreen\0";
	if (increment > 0L) _snprintf_s(caption, 20, "GLScreen_%d", increment);
	++increment;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(DWORD);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = caption;

	if (!RegisterClass(&wc))
	{
		setLastError<opengl::std::screen>(string("createWindow::RegisterClass"));

		return NULL;
	}

	window = CreateWindow(caption, caption, WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_POPUP, x, y, (int)width, (int)height, HWND_DESKTOP, NULL, GetModuleHandle(NULL), this);

	return window;
}

bool opengl::std::screen::createOpenGL()
{
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	hdc = GetDC(hwnd);

	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	iFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, iFormat, &pfd);

	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	program = extensions::glCreateProgramObjectARB();

	if (!compile(texture_fragment_source, GL_FRAGMENT_SHADER, fragment))
	{
		setLastError<opengl::std::screen>(string("createOpenGL::compile::texture_fragment_source"));

		return false;
	}

	if (!compile(texture_vertex_source, GL_VERTEX_SHADER, vertex))
	{
		setLastError<opengl::std::screen>(string("createOpenGL::compile::texture_vertex_source"));

		return false;
	}

	extensions::glAttachObjectARB(program, fragment);
	extensions::glAttachObjectARB(program, vertex);

	extensions::glLinkProgramARB(program);

	glViewport(0, 0, width, height);

	return true;
}

bool opengl::std::screen::compile(const char *source, GLenum type, GLenum &result)
{
	result = extensions::glCreateShaderObjectARB(type);

	extensions::glShaderSourceARB(result, 1, &(const GLcharARB*)source, NULL);

	extensions::glCompileShaderARB(result);

	GLint compiled = 0;
	extensions::glGetShaderiv(result, GL_COMPILE_STATUS, &compiled);

	if (!compiled) return false;

	return true;
}

void opengl::std::screen::resizeWindow(unsigned int w, unsigned int h)
{
	window_width = w; window_height = h;
}

bool opengl::std::screen::get(graphics::region *destination)
{
	return destination->initalise(&device(deviceID, hdc, hrc));
}

bool opengl::std::screen::render(graphics::region *source)
{
	if (!source->render(&device(deviceID, hdc, hrc))) return false;

	wglMakeCurrent(hdc, hrc);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0, 0, window_width, window_height);

	extensions::glUseProgramObjectARB(program);
	GLint id = extensions::glGetUniformLocationARB(program, "texImage");
	extensions::glUniform1iARB(id, 0);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, 0.0);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	SwapBuffers(hdc);

	wglMakeCurrent(NULL, NULL);

	return true;
}

void opengl::std::screen::makeNull()
{
	hwnd = NULL;

	hdc = NULL;
	hrc = NULL;

	program = 0;
	fragment = 0;
	vertex = 0;
}

void opengl::std::screen::cleanup()
{
	if (program != 0) extensions::glDeleteObjectARB(program);
	if (vertex != 0) extensions::glDeleteObjectARB(vertex);
	if (fragment != 0) extensions::glDeleteObjectARB(fragment);

	wglMakeCurrent(NULL, NULL);

	if (hrc != NULL) wglDeleteContext(hrc);
	if (hdc != NULL) ReleaseDC(hwnd, hdc);
}
/*
void opengl::cuda::region::set(regions::context *destination)
{
	context = destination;
}

bool opengl::cuda::region::initalise(graphics::device *d)
{
	if (!d->setDevice())
	{
		string temp("initalise::setDevice(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}

	if (!wglMakeCurrent((HDC)d->getDevice(), (HGLRC)d->getContext()))
	{
		setLastError<opengl::cuda::region>(string("initalise::wglMakeCurrent"));

		return false;
	}

	glGenTextures(1, &pbo);
	glBindTexture(GL_TEXTURE_2D, pbo);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8UI_EXT, width, height, 0, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_BYTE, NULL);
	
	if (cudaGraphicsGLRegisterImage(&cudaResource, pbo, GL_TEXTURE_2D, cudaGraphicsMapFlagsWriteDiscard) != cudaSuccess)
	{
		string temp("initalise::cudaGraphicsGLRegisterImage(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}

	ppResources[0] = cudaResource;

	if (cudaMalloc((void**)&cudaLinearMemory, sizeof(GLubyte) * width * height * 4) != cudaSuccess)
	{
		string temp("initalise::cudaMalloc(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}

	if (cudaMemset(cudaLinearMemory, 1, width * height * sizeof(GLubyte) * 4) != cudaSuccess)
	{
		setLastError<opengl::cuda::region>(string("initalise::wglMakeCurrent"));

		return false;
	}

	wglMakeCurrent(NULL, NULL);

	return true;
}

bool opengl::cuda::region::render(graphics::device *d)
{
	if (!wglMakeCurrent((HDC)d->getDevice(), (HGLRC)d->getContext()))
	{
		setLastError<opengl::cuda::region>(string("render::wglMakeCurrent"));

		return false;
	}

	cudaArray *cuArray;

	if (cudaGraphicsMapResources(1, ppResources, NULL) != cudaSuccess)
	{
		string temp("render::cudaGraphicsMapResources(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}

	if (cudaGraphicsSubResourceGetMappedArray(&cuArray, cudaResource, 0, 0) != cudaSuccess)
	{
		string temp("render::cudaGraphicsSubResourceGetMappedArray(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}

	context->render(cudaLinearMemory);

	if (cudaMemcpy2DToArray(cuArray, 0, 0, cudaLinearMemory, sizeof(GLubyte) * 4 * width, sizeof(GLubyte) * 4 * width, height, cudaMemcpyDeviceToDevice) != cudaSuccess)
	{
		string temp("render::cudaMemcpy2DToArray(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}

	if (cudaGraphicsUnmapResources(1, ppResources, NULL) != cudaSuccess)
	{
		string temp("render::cudaGraphicsUnMapResources(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}

	glBindTexture(GL_TEXTURE_2D, pbo);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	wglMakeCurrent(NULL, NULL);

	return true;
}

void opengl::cuda::region::makeNull()
{
	pbo = 0;
	cudaLinearMemory = NULL;
	cudaResource = NULL;
}

void opengl::cuda::region::cleanup()
{
	if (cudaResource != NULL) cudaGraphicsUnregisterResource(cudaResource);
	if (cudaLinearMemory != NULL) cudaFree(cudaLinearMemory);
	if (pbo != 0) glDeleteTextures(1, &pbo);
}

char *opengl::cuda::screen::texture_vertex_source =
"void main(void)\n"
"{\n"
"	gl_Position = gl_Vertex;\n"
"	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;\n"
"}\n";

char *opengl::cuda::screen::texture_fragment_source =
"#version 130\n"
"uniform usampler2D texImage;\n"
"void main()\n"
"{\n"
"   vec4 c = texture(texImage, gl_TexCoord[0].xy);\n"
"	gl_FragColor = c / 255.0;\n"
"}\n";

long opengl::cuda::screen::increment = 0L;

LRESULT CALLBACK opengl::cuda::screen::WindowProc(HWND hwnd, unsigned uMsg, WPARAM wparam, LPARAM lparam)
{
	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCT *pCreate = (CREATESTRUCT*)lparam;
		opengl::cuda::screen *ptr = (opengl::cuda::screen*)pCreate->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)ptr);
	}
	else if (uMsg == WM_DESTROY)
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		PostQuitMessage(0);
	}
	else if ((uMsg == WM_KEYDOWN) && (wparam == VK_ESCAPE))
	{
		DestroyWindow(hwnd);
	}
	else if (uMsg == WM_SETCURSOR)
	{
		opengl::cuda::screen *ptr = (opengl::cuda::screen*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (ptr != NULL)
		{
			if (ptr->windowed) SetCursor(LoadCursor(NULL, IDC_ARROW));
			else SetCursor(NULL);
		}
		else SetCursor(NULL);
	}
	else if (uMsg == WM_SIZE)
	{
		opengl::cuda::screen *ptr = (opengl::cuda::screen*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (ptr != NULL)
		{
			ptr->resizeWindow(LOWORD(lparam), HIWORD(lparam));
		}
	}

	return DefWindowProc(hwnd, uMsg, wparam, lparam);
}

void opengl::cuda::screen::reset(unsigned int device, unsigned int width, unsigned int height, bool windowed)
{
	init = false; cleanup();

	deviceID = device;

	screen::width = width; screen::height = height;
	window_width = width; window_height = height;

	screen::windowed = windowed;

	hwnd = createWindow();
	if (hwnd == NULL)
	{
		setLastError<opengl::cuda::screen>(string("reset::createWindow"));

		return;
	}

	ShowWindow(hwnd, 1);
	UpdateWindow(hwnd);

	if (!createOpenGL())
	{
		setLastError<opengl::cuda::screen>(string("reset::createOpenGL"));

		return;
	}

	init = true;
}

bool opengl::cuda::screen::title(string source)
{
	if (hwnd != NULL)
	{
		if (SetWindowText(hwnd, source.c_str()) == TRUE) return true;
	}

	setLastError<opengl::cuda::screen>(string("title::SetWindowText"));

	return false;
}

HWND opengl::cuda::screen::createWindow()
{
	HWND window;
	WNDCLASS wc;

	char caption[20] = "Screen\0";
	if (increment > 0L) _snprintf_s(caption, 20, "Screen_%d", increment);
	++increment;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(DWORD);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = caption;

	if (!RegisterClass(&wc))
	{
		setLastError<opengl::cuda::screen>(string("createWindow::RegisterClass"));

		return NULL;
	}

	window = CreateWindow(caption, caption, WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_POPUP, x, y, (int)width, (int)height, HWND_DESKTOP, NULL, GetModuleHandle(NULL), this);

	return window;
}

bool opengl::cuda::screen::createOpenGL()
{
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	hdc = GetDC(hwnd);
	//hdc = CreateDCA("\\\\.\\DISPLAY1", "\\\\.\\DISPLAY1", NULL, NULL);

	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	iFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, iFormat, &pfd);

	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	program = extensions::glCreateProgramObjectARB();

	if (!compile(texture_fragment_source, GL_FRAGMENT_SHADER, fragment))
	{
		setLastError<opengl::cuda::screen>(string("createOpenGL::compile::texture_fragment_source"));

		return false;
	}

	if (!compile(texture_vertex_source, GL_VERTEX_SHADER, vertex))
	{
		setLastError<opengl::cuda::screen>(string("createOpenGL::compile::texture_vertex_source"));

		return false;
	}

	extensions::glAttachObjectARB(program, fragment);
	extensions::glAttachObjectARB(program, vertex);

	extensions::glLinkProgramARB(program);

	glViewport(0, 0, width, height);

	return true;
}

bool opengl::cuda::screen::compile(const char *source, GLenum type, GLenum &result)
{
	result = extensions::glCreateShaderObjectARB(type);

	extensions::glShaderSourceARB(result, 1, &(const GLcharARB*)source, NULL);

	extensions::glCompileShaderARB(result);

	GLint compiled = 0;
	extensions::glGetShaderiv(result, GL_COMPILE_STATUS, &compiled);

	if (!compiled) return false;

	return true;	
}

void opengl::cuda::screen::resizeWindow(unsigned int w, unsigned int h)
{
	window_width = w; window_height = h;
}

bool opengl::cuda::screen::get(graphics::region *destination)
{
	return destination->initalise(&device(deviceID,hdc,hrc));	
}

bool opengl::cuda::screen::render(graphics::region *source)
{	
	if (!source->render(&device(deviceID,hdc,hrc))) return false;

	wglMakeCurrent(hdc, hrc);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0, 0, window_width, window_height);

	extensions::glUseProgramObjectARB(program);
	GLint id = extensions::glGetUniformLocationARB(program, "texImage");
	extensions::glUniform1iARB(id, 0);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, 0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, 0.5);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	SwapBuffers(hdc);

	wglMakeCurrent(NULL, NULL);

	return true;
}

void opengl::cuda::screen::makeNull()
{
	hwnd = NULL;
	
	hdc = NULL;
	hrc = NULL;

	program = 0;
	fragment = 0; 
	vertex = 0;
}

void opengl::cuda::screen::cleanup()
{
	if (program != 0) extensions::glDeleteObjectARB(program);
	if (vertex != 0) extensions::glDeleteObjectARB(vertex);
	if (fragment != 0) extensions::glDeleteObjectARB(fragment);

	wglMakeCurrent(NULL, NULL);

	if(hrc != NULL) wglDeleteContext(hrc);
	if(hdc != NULL) ReleaseDC(hwnd, hdc);
}
*/
#elif defined(__linux__)
#include <X11/Xatom.h>
#include <X11/Xutil.h>

void opengl::std::region::set(regions::context *destination)
{
	context = destination;
}

bool opengl::std::region::initalise(graphics::device *d)
{
	if (!d->setDevice())
	{
		//setLastError<opengl::std::region>(string("initalise::setDevice"));

		return false;
	}

	if (!glXMakeCurrent((Display*)d->getDevice(), (Window)d->getWindow(), (GLXContext)d->getContext()))
	{
		//setLastError<opengl::std::region>(string("initalise::glxMakeCurrent"));

		return false;
	}

	extensions::glGenBuffers(1, &pixel);
	extensions::glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixel);
	extensions::glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * 4, NULL, GL_STREAM_DRAW);
	extensions::glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8UI_EXT, width, height, 0, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glXMakeCurrent(NULL, None, NULL);

	return true;
}

bool opengl::std::region::render(graphics::device *d)
{
	if (!glXMakeCurrent((Display*)d->getDevice(), (Window)d->getWindow(), (GLXContext)d->getContext()))
	{
		//setLastError<opengl::std::region>(string("render::glxMakeCurrent"));

		return false;
	}

	glBindTexture(GL_TEXTURE_2D, texture);

	extensions::glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8UI_EXT, width, height, 0, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_BYTE, NULL);

	extensions::glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixel);
	extensions::glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * 4, NULL, GL_STREAM_DRAW);
	void *result = extensions::glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

	context->render(result);

	extensions::glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	extensions::glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	glXMakeCurrent(NULL, None, NULL);

	return true;
}

void opengl::std::region::makeNull()
{
	texture = 0;
	pixel = 0;
}

void opengl::std::region::cleanup()
{
	if (texture != 0) glDeleteTextures(1, &texture);
	if (pixel != 0) extensions::glDeleteBuffers(1, &pixel);
}

const char *opengl::std::screen::texture_vertex_source =
"void main(void)\n"
"{\n"
"	gl_Position = gl_Vertex;\n"
"	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;\n"
"}\n";

const char *opengl::std::screen::texture_fragment_source =
"#version 130\n"
"uniform usampler2D texImage;\n"
"void main()\n"
"{\n"
"   vec4 c = texture(texImage, gl_TexCoord[0].xy);\n"
"	gl_FragColor = c / 255.0;\n"
"}\n";

long opengl::std::screen::increment = 0L;

void opengl::std::screen::reset(unsigned int device, unsigned int width, unsigned int height, bool windowed)
{
	init = false; cleanup();

	deviceID = device;

	screen::width = width; screen::height = height;
	window_width = width; window_height = height;

	screen::windowed = windowed;

	if (!createWindow())
	{
		//setLastError<opengl::std::screen>(string("reset::createWindow"));

		return;
	}

	if (!createOpenGL())
	{
		//setLastError<opengl::std::screen>(string("reset::createOpenGL"));

		return;
	}

	init = true;
}

bool opengl::std::screen::title(string source)
{
	if (display == NULL) return false;

	XStoreName(display, window, source.c_str());

	return true;
}

bool opengl::std::screen::createWindow()
{
	display = XOpenDisplay(NULL);

	GLint glxAttribs[] = {
		GLX_RGBA,
		GLX_DOUBLEBUFFER,
		GLX_DEPTH_SIZE,     24,
		GLX_STENCIL_SIZE,   8,
		GLX_RED_SIZE,       8,
		GLX_GREEN_SIZE,     8,
		GLX_BLUE_SIZE,      8,
		GLX_SAMPLE_BUFFERS, 0,
		GLX_SAMPLES,        0,
		None
	};

	visual = glXChooseVisual(display, deviceID, glxAttribs);

	windowAttrib.border_pixel = BlackPixel(display, (int)deviceID);
	windowAttrib.background_pixel = WhitePixel(display, (int)deviceID);
	windowAttrib.override_redirect = True;
	windowAttrib.colormap = XCreateColormap(display, RootWindow(display, (int)deviceID), visual->visual, AllocNone);
	windowAttrib.event_mask = ExposureMask;

	window = XCreateWindow(display, RootWindow(display, (int)deviceID), (int)x, (int)y, (int)width, (int)height, 0, visual->depth, InputOutput, visual->visual, CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &windowAttrib);

	XSelectInput(display, window, ExposureMask | StructureNotifyMask);

	XMapWindow(display, window);

	char caption[20] = "GLScreen\0";
	if (increment > 0L) snprintf(caption, 20, "GLScreen_%ld", increment);
	++increment;

	if (!title(string(caption))) return false;

	return true;
}

bool opengl::std::screen::createOpenGL()
{
	context = glXCreateContext(display, visual, NULL, GL_TRUE);
	glXMakeCurrent(display, window, context);

	program = extensions::glCreateProgramObjectARB();

	if (!compile(texture_fragment_source, GL_FRAGMENT_SHADER, fragment))
	{
		//setLastError<opengl::std::screen>(string("createOpenGL::compile::texture_fragment_source"));

		return false;
	}

	if (!compile(texture_vertex_source, GL_VERTEX_SHADER, vertex))
	{
		//setLastError<opengl::std::screen>(string("createOpenGL::compile::texture_vertex_source"));

		return false;
	}

	extensions::glAttachObjectARB(program, fragment);
	extensions::glAttachObjectARB(program, vertex);

	extensions::glLinkProgramARB(program);

	glViewport(0, 0, width, height);

	return true;
}

bool opengl::std::screen::compile(const char *source, GLenum type, GLenum &result)
{
	result = extensions::glCreateShaderObjectARB(type);

	extensions::glShaderSourceARB(result, 1, &source, NULL);
	extensions::glCompileShaderARB(result);

	GLint compiled = 0;
	extensions::glGetShaderiv(result, GL_COMPILE_STATUS, &compiled);

	if (!compiled) return false;

	return true;
}

void opengl::std::screen::resizeWindow(unsigned int w, unsigned int h)
{
	window_width = w; window_height = h;
}

bool opengl::std::screen::get(graphics::region *destination)
{
	device temp(deviceID, context, display, window);
	return destination->initalise(&temp);
}

bool opengl::std::screen::render(graphics::region *source)
{
	device temp(deviceID, context, display, window);
	if (!source->render(&temp)) return false;

	glXMakeCurrent(display, window, context);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0, 0, window_width, window_height);

	extensions::glUseProgramObjectARB(program);
	GLint id = extensions::glGetUniformLocationARB(program, "texImage");
	extensions::glUniform1iARB(id, 0);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, 0.0);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glXSwapBuffers(display, window);
	glXMakeCurrent(NULL, None, NULL);

	return true;
}

bool opengl::std::screen::render(geometry *source)
{
	return true;
}

void opengl::std::screen::makeNull()
{
	display = NULL;
	visual = NULL;

	program = 0;
	fragment = 0;
	vertex = 0;
}

void opengl::std::screen::cleanup()
{
	glXMakeCurrent(NULL, None, NULL);

	if (program != 0) extensions::glDeleteObjectARB(program);
	if (vertex != 0) extensions::glDeleteObjectARB(vertex);
	if (fragment != 0) extensions::glDeleteObjectARB(fragment);

	if (visual != NULL) XFree(visual);
		
	if (display != NULL)
	{
		XFreeColormap(display, windowAttrib.colormap);
		XDestroyWindow(display, window);
		XCloseDisplay(display);
	}	
}

/*
void opengl::cuda::region::set(regions::context *destination)
{
	context = destination;
}

bool opengl::cuda::region::initalise(graphics::device *d)
{
	if (!d->setDevice())
	{
		string temp("initalise::setDevice(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}

	if (!glXMakeCurrent((Display*)d->getDevice(), (Window)d->getWindow(), (GLXContext)d->getContext()))
	{
		setLastError<opengl::cuda::region>(string("initalise::glxMakeCurrent"));

		return false;
	}

	glGenTextures(1, &pbo);
	glBindTexture(GL_TEXTURE_2D, pbo);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8UI_EXT, width, height, 0, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_BYTE, NULL);

	if (cudaGraphicsGLRegisterImage(&cudaResource, pbo, GL_TEXTURE_2D, cudaGraphicsMapFlagsWriteDiscard) != cudaSuccess)
	{
		string temp("initalise::cudaGraphicsGLRegisterImage(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}
	ppResources[0] = cudaResource;

	if (cudaMalloc((void**)&cudaLinearMemory, sizeof(GLubyte) * width * height * 4) != cudaSuccess)
	{
		string temp("initalise::cudaMalloc(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}

	if (cudaMemset(cudaLinearMemory, 1, width * height * sizeof(GLubyte) * 4) != cudaSuccess)
	{
		string temp("initalise::cudaMemset(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}

	glXMakeCurrent(NULL, None, NULL);

	return true;
}

bool opengl::cuda::region::render(graphics::device *d)
{
	if (!glXMakeCurrent((Display*)d->getDevice(), (Window)d->getWindow(), (GLXContext)d->getContext()))
	{
		setLastError<opengl::cuda::region>(string("render::glxMakeCurrent"));

		return false;
	}

	cudaArray *cuArray;

	if (cudaGraphicsMapResources(1, ppResources, NULL) != cudaSuccess) 
	{
		string temp("render::cudaGraphicsMapResources(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}

	if (cudaGraphicsSubResourceGetMappedArray(&cuArray, cudaResource, 0, 0) != cudaSuccess)
	{
		string temp("render::cudaGraphicsSubResourceGetMappedArray(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}

	context->render(cudaLinearMemory);

	if (cudaMemcpy2DToArray(cuArray, 0, 0, cudaLinearMemory, sizeof(GLubyte) * 4 * width, sizeof(GLubyte) * 4 * width, height, cudaMemcpyDeviceToDevice) != cudaSuccess)
	{
		string temp("render::cudaMemcpy2DToArray(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}

	if (cudaGraphicsUnmapResources(1, ppResources, NULL) != cudaSuccess)
	{
		string temp("render::cudaGraphicsUnMapResources(");
		temp.concat(string(cudaGetErrorString(cudaGetLastError())));
		temp.concat(string(")"));

		setLastError<opengl::cuda::region>(temp);

		return false;
	}

	glBindTexture(GL_TEXTURE_2D, pbo);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glXMakeCurrent(NULL, None, NULL);

	return true;
}

void opengl::cuda::region::makeNull()
{
	pbo = 0;
	cudaLinearMemory = NULL;
	cudaResource = NULL;
}

void opengl::cuda::region::cleanup()
{
	if (cudaResource != NULL) cudaGraphicsUnregisterResource(cudaResource);
	if (cudaLinearMemory != NULL) cudaFree(cudaLinearMemory);
	if (pbo != 0) glDeleteTextures(1, &pbo);
}

const char *opengl::cuda::screen::texture_vertex_source =
"void main(void)\n"
"{\n"
"	gl_Position = gl_Vertex;\n"
"	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;\n"
"}\n";

const char *opengl::cuda::screen::texture_fragment_source =
"#version 130\n"
"uniform usampler2D texImage;\n"
"void main()\n"
"{\n"
"   vec4 c = texture(texImage, gl_TexCoord[0].xy);\n"
"	gl_FragColor = c / 255.0;\n"
"}\n";

long opengl::cuda::screen::increment = 0L;

void opengl::cuda::screen::reset(unsigned int device, unsigned int width, unsigned int height, bool windowed)
{
	init = false; cleanup();

	deviceID = device;

	screen::width = width; screen::height = height;
	window_width = width; window_height = height;

	screen::windowed = windowed;

	if (!createWindow())
	{
		setLastError<opengl::cuda::screen>(string("reset::createWindow"));

		return;
	}

	if (!createOpenGL())
	{
		setLastError<opengl::cuda::screen>(string("reset::createOpenGL"));

		return;
	}

	init = true;
}

bool opengl::cuda::screen::title(string source)
{
	if (display == NULL) return false;

	XStoreName(display, window, source.c_str());

	return true;
}

bool opengl::cuda::screen::createWindow()
{
	display = XOpenDisplay(NULL);

	GLint glxAttribs[] = {
		GLX_RGBA,
		GLX_DOUBLEBUFFER,
		GLX_DEPTH_SIZE,     24,
		GLX_STENCIL_SIZE,   8,
		GLX_RED_SIZE,       8,
		GLX_GREEN_SIZE,     8,
		GLX_BLUE_SIZE,      8,
		GLX_SAMPLE_BUFFERS, 0,
		GLX_SAMPLES,        0,
		None
	};

	visual = glXChooseVisual(display, (int)deviceID, glxAttribs);

	windowAttrib.border_pixel = BlackPixel(display, (int)deviceID);
	windowAttrib.background_pixel = WhitePixel(display, (int)deviceID);
	windowAttrib.override_redirect = True;
	windowAttrib.colormap = XCreateColormap(display, RootWindow(display, (int)deviceID), visual->visual, AllocNone);
	windowAttrib.event_mask = ExposureMask;

	window = XCreateWindow(display, RootWindow(display, (int)deviceID), (int)x, (int)y, (int)width, (int)height, 0, visual->depth, InputOutput, visual->visual, CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &windowAttrib);

	XSelectInput(display, window, ExposureMask | StructureNotifyMask);

	XMapWindow(display, window);

	char caption[20] = "Screen\0";
	if (increment > 0L) snprintf(caption, 20, "Screen_%ld", increment);
	++increment;

	if (!title(string(caption))) return false;

	return true;
}

bool opengl::cuda::screen::createOpenGL()
{
	context = glXCreateContext(display, visual, NULL, GL_TRUE);
	glXMakeCurrent(display, window, context);

	program = extensions::glCreateProgramObjectARB();

	if (!compile(texture_fragment_source, GL_FRAGMENT_SHADER, fragment))
	{
		setLastError<opengl::cuda::screen>(string("createOpenGL::compile::texture_fragment_source"));

		return false;
	}

	if (!compile(texture_vertex_source, GL_VERTEX_SHADER, vertex)) 
	{
		setLastError<opengl::cuda::screen>(string("createOpenGL::compile::texture_vertex_source"));

		return false;
	}

	extensions::glAttachObjectARB(program, fragment);
	extensions::glAttachObjectARB(program, vertex);

	extensions::glLinkProgramARB(program);

	glViewport(0, 0, width, height);

	return true;
}

bool opengl::cuda::screen::compile(const char *source, GLenum type, GLenum &result)
{
	result = extensions::glCreateShaderObjectARB(type);

	extensions::glShaderSourceARB(result, 1, &source, NULL);

	extensions::glCompileShaderARB(result);

	GLint compiled = 0;
	extensions::glGetShaderiv(result, GL_COMPILE_STATUS, &compiled);

	if (!compiled) return false;

	return true;
}

void opengl::cuda::screen::resizeWindow(unsigned int w, unsigned int h)
{
	window_width = w; window_height = h;
}

bool opengl::cuda::screen::get(graphics::region *destination)
{
	device temp(deviceID, context, display, window);
	return destination->initalise(&temp);
}

bool opengl::cuda::screen::render(graphics::region *source)
{
	device temp(deviceID, context, display, window);
	if (!source->render(&temp)) return false;

	glXMakeCurrent(display, window, context);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0, 0, window_width, window_height);

	extensions::glUseProgramObjectARB(program);
	GLint id = extensions::glGetUniformLocationARB(program, "texImage");
	extensions::glUniform1iARB(id, 0);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, 0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, 0.5);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glXSwapBuffers(display, window);
	glXMakeCurrent(NULL, None, NULL);

	return true;
}

void opengl::cuda::screen::makeNull()
{
	display = NULL;
	visual = NULL;

	program = 0;
	fragment = 0;
	vertex = 0;
}

void opengl::cuda::screen::cleanup()
{
	glXMakeCurrent(NULL, None, NULL);

	if (program != 0) extensions::glDeleteObjectARB(program);
	if (vertex != 0) extensions::glDeleteObjectARB(vertex);
	if (fragment != 0) extensions::glDeleteObjectARB(fragment);

	if (visual != NULL) XFree(visual);
		
	if (display != NULL)
	{
		XFreeColormap(display, windowAttrib.colormap);
		XDestroyWindow(display, window);
		XCloseDisplay(display);
	}
}
*/
#endif