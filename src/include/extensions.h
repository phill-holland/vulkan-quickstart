#ifndef _OPENGL_EXTENSIONS
#define _OPENGL_EXTENSIONS

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#ifdef WIN32
#include "glext.h"
#elif defined (__linux__)
#include <GL/glx.h>
#include <GL/glext.h>
#endif
//#include <cuda_gl_interop.h>
//#include <cuda_runtime_api.h>
#include "graphics.h"
#include "regions.h"

#ifdef WIN32
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif

namespace opengl
{
	class extensions
	{
	protected:
		static PFNGLGENBUFFERSARBPROC _glGenBuffersARB;
		static PFNGLGENBUFFERSPROC _glGenBuffers;
		static PFNGLBINDBUFFERARBPROC _glBindBufferARB;
		static PFNGLBUFFERDATAARBPROC _glBufferDataARB;
		static PFNGLBUFFERDATAPROC _glBufferData;
		static PFNGLDELETEBUFFERSARBPROC _glDeleteBuffersARB;
		static PFNGLDELETEBUFFERSPROC _glDeleteBuffers;
		static PFNGLCREATEPROGRAMOBJECTARBPROC _glCreateProgramObjectARB;
		static PFNGLCREATESHADEROBJECTARBPROC _glCreateShaderObjectARB;
		static PFNGLSHADERSOURCEARBPROC _glShaderSourceARB;
		static PFNGLCOMPILESHADERPROC _glCompileShader;
		static PFNGLCOMPILESHADERARBPROC _glCompileShaderARB;
		static PFNGLATTACHOBJECTARBPROC _glAttachObjectARB;
		static PFNGLLINKPROGRAMARBPROC _glLinkProgramARB;
		static PFNGLUSEPROGRAMOBJECTARBPROC _glUseProgramObjectARB;
		static PFNGLDELETESHADERPROC _glDeleteShader;
		static PFNGLGETUNIFORMLOCATIONARBPROC _glGetUniformLocationARB;
		static PFNGLUNIFORM1IARBPROC _glUniform1iARB;
		static PFNGLDELETEOBJECTARBPROC _glDeleteObjectARB;
		static PFNGLGETSHADERIVPROC _glGetShaderiv;
		static PFNGLGETSHADERINFOLOGPROC _glGetShaderInfoLog;
		static PFNGLMAPBUFFERPROC _glMapBuffer;
		static PFNGLUNMAPBUFFERPROC _glUnmapBuffer;
		static PFNGLBINDBUFFERPROC _glBindBuffer;
		static PFNGLMAPBUFFERARBPROC _glMapBufferARB;
		static PFNGLUNMAPBUFFERARBPROC _glUnmapBufferARB;
		static PFNGLENABLEVERTEXATTRIBARRAYPROC _glEnableVertexAttribArray;
		static PFNGLDISABLEVERTEXATTRIBARRAYPROC _glDisableVertexAttribArray;
		static PFNGLVERTEXATTRIBPOINTERPROC _glVertexAttribPointer;

	public:
		static void glGenBuffersARB(GLsizei n, GLuint *buffers);
		static void glGenBuffers(GLsizei n, GLuint *buffers);
		static void glBindBufferARB(GLenum target, GLuint buffer);
		static void glBufferDataARB(GLenum target, GLsizei size, const void *data, GLenum usage);
		static void glBufferData(GLenum target, GLsizei size, const void *data, GLenum usage);
		static void glDeleteBuffersARB(GLsizei n, GLuint *buffers);
		static void glDeleteBuffers(GLsizei n, GLuint *buffers);
		static GLhandleARB glCreateProgramObjectARB();
		static GLhandleARB glCreateShaderObjectARB(GLenum shaderType);
		static void glShaderSourceARB(GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length);
		static void glCompileShader(GLuint shader);
		static void glCompileShaderARB(GLhandleARB shaderObj);
		static void glAttachObjectARB(GLhandleARB containerObj, GLhandleARB obj);
		static void glLinkProgramARB(GLhandleARB programObj);
		static void glUseProgramObjectARB(GLhandleARB programObj);
		static void glDeleteShader(GLuint shader);
		static GLint glGetUniformLocationARB(GLhandleARB programObj, const GLcharARB *name);
		static void glUniform1iARB(GLint location, GLint v0);
		static void glDeleteObjectARB(GLhandleARB obj);
		static void glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
		static void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infolog);
		static void *glMapBuffer(GLenum target, GLenum access);
		static GLboolean glUnmapBuffer(GLenum target);
		static void glBindBuffer(GLenum target, GLuint buffer);
		static void  *glMapBufferARB(GLenum target, GLenum access);
		static GLboolean glUnmapBufferARB(GLenum target);
		static void glEnableVertexAttribArray(GLuint index);
		static void glDisableVertexAttribArray(GLuint index);
		static void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
	};
};

#endif