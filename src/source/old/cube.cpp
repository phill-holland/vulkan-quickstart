#include "cube.h"
#include "log.h"

using namespace opengl;

const char *cube::texture_vertex_source =
"void main(void)\n"
"{\n"
"	gl_Position = gl_Vertex;\n"
"	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;\n"
"}\n";

const char *cube::texture_fragment_source =
"#version 130\n"
"uniform usampler2D texImage;\n"
"void baa()\n"
"{\n"
"   vec4 c = texture(texImage, gl_TexCoord[0].xy);\n"
"	gl_FragColor = c / 255.0;\n"
"}\n";


void cube::reset()
{
    init = false; cleanup();

    init = createOpenGL();
    
    //init = true;
}

bool cube::createOpenGL()
{
	//context = glXCreateContext(display, visual, NULL, GL_TRUE);
	//glXMakeCurrent(display, window, context);

	//program = extensions::glCreateProgramObjectARB();

	if (!compile(texture_fragment_source, GL_FRAGMENT_SHADER, fragment))
	{
        errorGL();
        //glGetErrorString
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

	//glViewport(0, 0, width, height);

	return true;

    // create object virutal class,
    // with points to draw
    // pass into opengl::screen class to render

    // gemoetry -- xywz<float> get()   length
    // include local matrix
}

void cube::errorGL()
{
    GLenum errCode;
    const GLubyte *errString;
    if ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
    }

    GLint maxLength = 0;
	extensions::glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &maxLength);

    GLchar moo[1024];
    //std::vector<GLchar> errorLog(maxLength);
	extensions::glGetShaderInfoLog(fragment, maxLength, &maxLength, &moo[0]);//errorLog[0]);

    Log << (char *)moo;

}

bool cube::compile(const char *source, GLenum type, GLenum &result)
{
	result = extensions::glCreateShaderObjectARB(type);

	extensions::glShaderSourceARB(result, 1, &source, NULL);
	extensions::glCompileShaderARB(result);

	GLint compiled = 0;
	extensions::glGetShaderiv(result, GL_COMPILE_STATUS, &compiled);

	if (!compiled) return false;

	return true;
}

void cube::render()
{
    const GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };
/*
    const GLfloat g_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };
*/
    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    extensions::glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    extensions::glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    extensions::glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    extensions::glEnableVertexAttribArray(0);
    extensions::glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    extensions::glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );
    // Draw the triangle !
    //glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDrawArrays(GL_TRIANGLES, 0, 12*3);

    extensions::glDisableVertexAttribArray(0);
}

void cube::makeNull()
{

}

void cube::cleanup()
{

}