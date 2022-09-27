#include "OpenGLContext.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Daqromancy {

	OpenGLContext::OpenGLContext(GLFWwindow* window) :
		m_window(window)
	{
	}

	OpenGLContext::~OpenGLContext()
	{
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		DY_TRACE("Loaded OpenGL with glad Init status {0}", status);

		DY_INFO("Loaded OpenGL renderer");
		DY_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		DY_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		DY_INFO("Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_window);
	}
}