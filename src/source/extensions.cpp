#include "extensions.h"

PFNGLGENBUFFERSARBPROC opengl::extensions::_glGenBuffersARB = NULL;
PFNGLGENBUFFERSPROC opengl::extensions::_glGenBuffers = NULL;
PFNGLBINDBUFFERARBPROC opengl::extensions::_glBindBufferARB = NULL;
PFNGLBUFFERDATAARBPROC opengl::extensions::_glBufferDataARB = NULL;
PFNGLBUFFERDATAPROC opengl::extensions::_glBufferData = NULL;
PFNGLDELETEBUFFERSARBPROC opengl::extensions::_glDeleteBuffersARB = NULL;
PFNGLDELETEBUFFERSPROC opengl::extensions::_glDeleteBuffers = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC opengl::extensions::_glCreateProgramObjectARB = NULL;
PFNGLCREATESHADEROBJECTARBPROC opengl::extensions::_glCreateShaderObjectARB = NULL;
PFNGLSHADERSOURCEARBPROC opengl::extensions::_glShaderSourceARB = NULL;
PFNGLCOMPILESHADERPROC opengl::extensions::_glCompileShader = NULL;
PFNGLCOMPILESHADERARBPROC opengl::extensions::_glCompileShaderARB = NULL;
PFNGLATTACHOBJECTARBPROC opengl::extensions::_glAttachObjectARB = NULL;
PFNGLLINKPROGRAMARBPROC opengl::extensions::_glLinkProgramARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC opengl::extensions::_glUseProgramObjectARB = NULL;
PFNGLDELETESHADERPROC opengl::extensions::_glDeleteShader = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC opengl::extensions::_glGetUniformLocationARB = NULL;
PFNGLUNIFORM1IARBPROC opengl::extensions::_glUniform1iARB = NULL;
PFNGLDELETEOBJECTARBPROC opengl::extensions::_glDeleteObjectARB = NULL;
PFNGLGETSHADERIVPROC opengl::extensions::_glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC opengl::extensions::_glGetShaderInfoLog = NULL;
PFNGLMAPBUFFERPROC opengl::extensions::_glMapBuffer = NULL;
PFNGLUNMAPBUFFERPROC opengl::extensions::_glUnmapBuffer = NULL;
PFNGLBINDBUFFERPROC opengl::extensions::_glBindBuffer = NULL;
PFNGLMAPBUFFERARBPROC opengl::extensions::_glMapBufferARB = NULL;
PFNGLUNMAPBUFFERARBPROC opengl::extensions::_glUnmapBufferARB = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC opengl::extensions::_glEnableVertexAttribArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC opengl::extensions::_glDisableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC opengl::extensions::_glVertexAttribPointer = NULL;


#ifdef WIN32

void opengl::extensions::glGenBuffersARB(GLsizei n, GLuint *buffers)
{
	if (_glGenBuffersARB == NULL) _glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
	_glGenBuffersARB(n, buffers);
}

void opengl::extensions::glGenBuffers(GLsizei n, GLuint *buffers)
{
	if (_glGenBuffers == NULL) _glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	_glGenBuffers(n, buffers);
}

void opengl::extensions::glBindBufferARB(GLenum target, GLuint buffer)
{
	if (_glBindBufferARB == NULL) _glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
	_glBindBufferARB(target, buffer);
}

void opengl::extensions::glBufferDataARB(GLenum target, GLsizei size, const void *data, GLenum usage)
{
	if (_glBufferDataARB == NULL) _glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
	_glBufferDataARB(target, size, data, usage);
}

void opengl::extensions::glBufferData(GLenum target, GLsizei size, const void *data, GLenum usage)
{
	if (_glBufferData == NULL) _glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	_glBufferData(target, size, data, usage);
}

void opengl::extensions::glDeleteBuffersARB(GLsizei n, GLuint *buffers)
{
	if (_glDeleteBuffersARB == NULL) _glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
	_glDeleteBuffersARB(n, buffers);
}

void opengl::extensions::glDeleteBuffers(GLsizei n, GLuint *buffers)
{
	if (_glDeleteBuffers == NULL) _glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	_glDeleteBuffers(n, buffers);
}

GLhandleARB opengl::extensions::glCreateProgramObjectARB()
{
	if (_glCreateProgramObjectARB == NULL) _glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
	return _glCreateProgramObjectARB();
}

GLhandleARB opengl::extensions::glCreateShaderObjectARB(GLenum shaderType)
{
	if (_glCreateShaderObjectARB == NULL) _glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
	return _glCreateShaderObjectARB(shaderType);
}

void opengl::extensions::glShaderSourceARB(GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length)
{
	if (_glShaderSourceARB == NULL) _glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB");
	_glShaderSourceARB(shaderObj, count, string, length);
}

void opengl::extensions::glCompileShader(GLuint shader)
{
	if (_glCompileShader == NULL) _glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	_glCompileShader(shader);
}

void opengl::extensions::glCompileShaderARB(GLhandleARB shaderObj)
{
	if (_glCompileShaderARB == NULL) _glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB");
	_glCompileShaderARB(shaderObj);
}

void opengl::extensions::glAttachObjectARB(GLhandleARB containerObj, GLhandleARB obj)
{
	if (_glAttachObjectARB == NULL) _glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB");
	_glAttachObjectARB(containerObj, obj);
}

void opengl::extensions::glLinkProgramARB(GLhandleARB programObj)
{
	if (_glLinkProgramARB == NULL) _glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
	_glLinkProgramARB(programObj);
}

void opengl::extensions::glUseProgramObjectARB(GLhandleARB programObj)
{
	if (_glUseProgramObjectARB == NULL) _glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
	_glUseProgramObjectARB(programObj);
}

void opengl::extensions::glDeleteShader(GLuint shader)
{
	if (_glDeleteShader == NULL) _glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	_glDeleteShader(shader);
}

GLint opengl::extensions::glGetUniformLocationARB(GLhandleARB programObj, const GLcharARB *name)
{
	if (_glGetUniformLocationARB == NULL) _glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
	return _glGetUniformLocationARB(programObj, name);
}

void opengl::extensions::glUniform1iARB(GLint location, GLint v0)
{
	if (_glUniform1iARB == NULL) _glUniform1iARB = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
	_glUniform1iARB(location, v0);
}

void opengl::extensions::glDeleteObjectARB(GLhandleARB obj)
{
	if (_glDeleteObjectARB == NULL) _glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)wglGetProcAddress("glDeleteObjectARB");
	_glDeleteObjectARB(obj);
}

void opengl::extensions::glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
	if (_glGetShaderiv == NULL) _glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	_glGetShaderiv(shader, pname, params);
}

void opengl::extensions::glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infolog)
{
	if (_glGetShaderInfoLog == NULL) _glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	_glGetShaderInfoLog(shader, bufSize, length, infolog);
}

void *opengl::extensions::glMapBuffer(GLenum target, GLenum access)
{
	if (_glMapBuffer == NULL) _glMapBuffer = (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");
	return _glMapBuffer(target, access);
}

GLboolean opengl::extensions::glUnmapBuffer(GLenum target)
{
	if (_glUnmapBuffer == NULL) _glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
	return _glUnmapBuffer(target);
}

void opengl::extensions::glBindBuffer(GLenum target, GLuint buffer)
{
	if (_glBindBuffer == NULL) _glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	_glBindBuffer(target, buffer);
}

void *opengl::extensions::glMapBufferARB(GLenum target, GLenum access)
{
	if (_glMapBufferARB == NULL) _glMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
	return _glMapBufferARB(target, access);
}

GLboolean opengl::extensions::glUnmapBufferARB(GLenum target)
{
	if (_glUnmapBufferARB == NULL) _glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");
	return _glUnmapBufferARB(target);
}

#elif defined (__linux__)

void opengl::extensions::glGenBuffersARB(GLsizei n, GLuint *buffers)
{
	if (_glGenBuffersARB == NULL) _glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)glXGetProcAddress((const GLubyte*)"glGenBuffersARB");
	_glGenBuffersARB(n, buffers);
}

void opengl::extensions::glGenBuffers(GLsizei n, GLuint *buffers)
{
	if (_glGenBuffers == NULL) _glGenBuffers = (PFNGLGENBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glGenBuffers");
	_glGenBuffers(n, buffers);
}

void opengl::extensions::glBindBufferARB(GLenum target, GLuint buffer)
{
	if (_glBindBufferARB == NULL) _glBindBufferARB = (PFNGLBINDBUFFERARBPROC)glXGetProcAddress((const GLubyte*)"glBindBufferARB");
	_glBindBufferARB(target, buffer);
}

void opengl::extensions::glBufferDataARB(GLenum target, GLsizei size, const void *data, GLenum usage)
{
	if (_glBufferDataARB == NULL) _glBufferDataARB = (PFNGLBUFFERDATAARBPROC)glXGetProcAddress((const GLubyte*)"glBufferDataARB");
	_glBufferDataARB(target, size, data, usage);
}

void opengl::extensions::glBufferData(GLenum target, GLsizei size, const void *data, GLenum usage)
{
	if (_glBufferData == NULL) _glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress((const GLubyte*)"glBufferData");
	_glBufferData(target, size, data, usage);
}

void opengl::extensions::glDeleteBuffersARB(GLsizei n, GLuint *buffers)
{
	if (_glDeleteBuffersARB == NULL) _glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)glXGetProcAddress((const GLubyte*)"glDeleteBuffersARB");
	_glDeleteBuffersARB(n, buffers);
}

void opengl::extensions::glDeleteBuffers(GLsizei n, GLuint *buffers)
{
	if (_glDeleteBuffers == NULL) _glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glDeleteBuffers");
	_glDeleteBuffers(n, buffers);
}

GLhandleARB opengl::extensions::glCreateProgramObjectARB()
{
	if (_glCreateProgramObjectARB == NULL) _glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)glXGetProcAddress((const GLubyte*)"glCreateProgramObjectARB");
	return _glCreateProgramObjectARB();
}

GLhandleARB opengl::extensions::glCreateShaderObjectARB(GLenum shaderType)
{
	if (_glCreateShaderObjectARB == NULL) _glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)glXGetProcAddress((const GLubyte*)"glCreateShaderObjectARB");
	return _glCreateShaderObjectARB(shaderType);
}

void opengl::extensions::glShaderSourceARB(GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length)
{
	if (_glShaderSourceARB == NULL) _glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)glXGetProcAddress((const GLubyte*)"glShaderSourceARB");
	_glShaderSourceARB(shaderObj, count, string, length);
}

void opengl::extensions::glCompileShader(GLuint shader)
{
	if (_glCompileShader == NULL) _glCompileShader = (PFNGLCOMPILESHADERPROC)glXGetProcAddress((const GLubyte*)"glCompileShader");
	_glCompileShader(shader);
}

void opengl::extensions::glCompileShaderARB(GLhandleARB shaderObj)
{
	if (_glCompileShaderARB == NULL) _glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)glXGetProcAddress((const GLubyte*)"glCompileShaderARB");
	_glCompileShaderARB(shaderObj);
}

void opengl::extensions::glAttachObjectARB(GLhandleARB containerObj, GLhandleARB obj)
{
	if (_glAttachObjectARB == NULL) _glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)glXGetProcAddress((const GLubyte*)"glAttachObjectARB");
	_glAttachObjectARB(containerObj, obj);
}

void opengl::extensions::glLinkProgramARB(GLhandleARB programObj)
{
	if (_glLinkProgramARB == NULL) _glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)glXGetProcAddress((const GLubyte*)"glLinkProgramARB");
	_glLinkProgramARB(programObj);
}

void opengl::extensions::glUseProgramObjectARB(GLhandleARB programObj)
{
	if (_glUseProgramObjectARB == NULL) _glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)glXGetProcAddress((const GLubyte*)"glUseProgramObjectARB");
	_glUseProgramObjectARB(programObj);
}

void opengl::extensions::glDeleteShader(GLuint shader)
{
	if (_glDeleteShader == NULL) _glDeleteShader = (PFNGLDELETESHADERPROC)glXGetProcAddress((const GLubyte*)"glDeleteShader");
	_glDeleteShader(shader);
}

GLint opengl::extensions::glGetUniformLocationARB(GLhandleARB programObj, const GLcharARB *name)
{
	if (_glGetUniformLocationARB == NULL) _glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)glXGetProcAddress((const GLubyte*)"glGetUniformLocationARB");
	return _glGetUniformLocationARB(programObj, name);
}

void opengl::extensions::glUniform1iARB(GLint location, GLint v0)
{
	if (_glUniform1iARB == NULL) _glUniform1iARB = (PFNGLUNIFORM1IARBPROC)glXGetProcAddress((const GLubyte*)"glUniform1iARB");
	_glUniform1iARB(location, v0);
}

void opengl::extensions::glDeleteObjectARB(GLhandleARB obj)
{
	if (_glDeleteObjectARB == NULL) _glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)glXGetProcAddress((const GLubyte*)"glDeleteObjectARB");
	_glDeleteObjectARB(obj);
}

void opengl::extensions::glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
	if (_glGetShaderiv == NULL) _glGetShaderiv = (PFNGLGETSHADERIVPROC)glXGetProcAddress((const GLubyte*)"glGetShaderiv");
	_glGetShaderiv(shader, pname, params);
}

void opengl::extensions::glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infolog)
{
	if (_glGetShaderInfoLog == NULL) _glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddress((const GLubyte*)"glGetShaderInfoLog");
	_glGetShaderInfoLog(shader, bufSize, length, infolog);
}

void *opengl::extensions::glMapBuffer(GLenum target, GLenum access)
{
	if (_glMapBuffer == NULL) _glMapBuffer = (PFNGLMAPBUFFERPROC)glXGetProcAddress((const GLubyte*)"glMapBuffer");
	return _glMapBuffer(target, access);
}

GLboolean opengl::extensions::glUnmapBuffer(GLenum target)
{
	if (_glUnmapBuffer == NULL) _glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)glXGetProcAddress((const GLubyte*)"glUnmapBuffer");
	return _glUnmapBuffer(target);
}

void opengl::extensions::glBindBuffer(GLenum target, GLuint buffer)
{
	if (_glBindBuffer == NULL) _glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress((const GLubyte*)"glBindBuffer");
	_glBindBuffer(target, buffer);
}

void *opengl::extensions::glMapBufferARB(GLenum target, GLenum access)
{
	if (_glMapBufferARB == NULL) _glMapBufferARB = (PFNGLMAPBUFFERARBPROC)glXGetProcAddress((const GLubyte*)"glMapBufferARB");
	return _glMapBufferARB(target, access);
}

GLboolean opengl::extensions::glUnmapBufferARB(GLenum target)
{
	if (_glUnmapBufferARB == NULL) _glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)glXGetProcAddress((const GLubyte*)"glUnmapBufferARB");
	return _glUnmapBufferARB(target);
}

void opengl::extensions::glEnableVertexAttribArray(GLuint index)
{
	if (_glEnableVertexAttribArray == NULL) _glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((const GLubyte*)"glEnableVertexAttribArray");
	_glEnableVertexAttribArray(index);
}

void opengl::extensions::glDisableVertexAttribArray(GLuint index)
{
	if (_glDisableVertexAttribArray == NULL) _glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((const GLubyte*)"glDisableVertexAttribArray");
	_glDisableVertexAttribArray(index);
}

void opengl::extensions::glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
	if (_glVertexAttribPointer == NULL) _glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress((const GLubyte*)"glVertexAttribPointer");
	_glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

#endif