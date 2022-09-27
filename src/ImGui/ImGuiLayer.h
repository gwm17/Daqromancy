#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "Core/Layer.h"

namespace Daqromancy {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender(double timestep) override;

		void Begin();
		void End();

	};
}

#endif