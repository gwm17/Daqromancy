#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Core/DYCore.h"
#include "Core/Layer.h"
#include "FileDialog.h"
#include "Events/AcqEvent.h"
#include "Core/DYProject.h"
#include "DigitizerPanel.h"
#include "SyncDialog.h"
#include "ScopePanel.h"
#include "ScalarPanel.h"

#include "imgui.h"

namespace Daqromancy {

	class EditorLayer : public Layer
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		EditorLayer(const DYProject::Ref& project);
		~EditorLayer();

		void SetEventCallback(const EventCallbackFunc& func) { m_eventCallback = func; }

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender(double timestep) override;

	private:
		void UpdateDigitizerPanels();
		void SaveProject(); //For call to save project to current path

		EventCallbackFunc m_eventCallback;

		DYProject::Ref m_project;
		std::string m_projectPath;
		uint32_t m_runNumber;
		std::vector<DigitizerArgs> m_digitizerArgList;

		FileDialog m_fileDialog;
		std::vector<DigitizerPanel> m_digiPanels;
		SyncDialog m_syncDialog;
		ScopePanel* m_scopePanel;
		ScalarPanel m_scalarPanel;

		//ImGui Settings
		bool dockspaceOpen = true;
		bool opt_fullscreen = true;
		bool opt_fullscreen_persistant = true;
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	};
}

#endif