#include <iostream>
#include <fstream>

#include <glew.h>
#include <GLFWInclude.h>
#include <CL\cl.h>

#include "OCL_Environment.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void APIENTRY glErrorCallback( GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length, const char* _message, void* _userParam) 
{
    char* source = "";

    if( _source == GL_DEBUG_SOURCE_API_ARB )
        source = "The GL";
    else if( _source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB )
        source = "The GLSL Shader compiler";
    else if( _source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB )
        source = "The source window system arb";

	std::cout << "OPENGL_ERROR: id:" << source << ":" << _type << ":"  << _message << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
}

int InitGLFW(GLFWwindow *&window)
{
	/* Initialize the library */
    if (!glfwInit())
    {
		std::cout << "Unable to init glfw." << std::endl;
        return -1;
    }

	/* Set context version to forward compatible 4.2 mode */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );

	int windowX = WINDOW_WIDTH;
	int windowY = WINDOW_HEIGHT;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowX, windowY, "Rayz", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
		std::cout << "Unable to create window\n" << std::endl;
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
	std::cout << glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR) << "." << glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR) << std::endl;

	// if 1 then limits system to max 60 fps!
	glfwSwapInterval( 0 ); 
	glClearColor( 0, 0, 0, 0 );

	// init glew
	glewExperimental = true;
	GLenum err = glewInit();
	if ( GLEW_OK != err ) {
		std::cout << "Unable to initialize GLEW.\n" << std::endl;
	}

	// assign callback functions...
	glDebugMessageCallbackARB(glErrorCallback, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

int main( int argc, char* argv[] )
{
	GLFWwindow* window;
    
	InitGLFW( window );

	// Create and initialize openCL environment
	OCL_Environment env;
	env.Initialize();

	/* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window))
	{	
		env.Execute();


		/* Swap front and back buffers */
        glfwSwapBuffers(window);

		 /* Poll for and process events */
        glfwPollEvents();

		// Exit with excape
		if( glfwGetKey( window, GLFW_KEY_ESCAPE ) )
			return 0;
		std::cin.get();
	}
	// Release the openCL environment
	env.Release();

	// Release glfw window
	glfwDestroyWindow(window);
    glfwTerminate();
	return 0;
}
