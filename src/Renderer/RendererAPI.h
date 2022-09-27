#ifndef RENDERER_API_H
#define RENDERER_API_H

namespace Daqromancy {

	class RendererAPI
	{
	public:
		enum class API
		{
			None=0,
			OpenGL
		};

		virtual void Clear() = 0;
		virtual void SetClearColor(float color[4]) = 0;
		virtual double GetFrameTime() = 0;
		
		static API GetAPI() { return s_api; }

	private:
		static API s_api;
	};
}

#endif