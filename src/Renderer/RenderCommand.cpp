#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Daqromancy {

	RendererAPI* RenderCommand::s_api = new OpenGLRendererAPI();

}