#include "Application.h"
#include "Renderer/RenderCommand.h"
#include "Editor/EditorLayer.h"
#include "DAQ/AcquisitionLayer.h"

#include <filesystem>


namespace Daqromancy {

    Application* Application::s_instance = nullptr;

    Application* CreateApplication(const ApplicationArgs& args) { return new Application(args); }

    Application::Application(const ApplicationArgs& args) :
        m_args(args), m_running(true)
    {
        if(s_instance != nullptr)
        {
            DY_ERROR("Attempting to create more than one application! CreateApplication should only be called once!");
        }

        s_instance = this;

        DY_INFO("Application created.");

        //Set current path to find assets
        if(!m_args.runtimePath.empty())
            std::filesystem::current_path(m_args.runtimePath);

        DY_INFO("Runtime path set to {0}", std::filesystem::current_path().string());

        m_window = std::unique_ptr<Window>(Window::Create({ m_args.name, 1280, 720 }));
        m_window->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));

        m_imguiLayer = new ImGuiLayer();
        PushOverlay(m_imguiLayer);

        m_project = std::make_shared<DYProject>();

        EditorLayer* editor = new EditorLayer(m_project);
        editor->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));
        PushLayer(editor);

        AcquisitionLayer* acq = new AcquisitionLayer(m_project);
        acq->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));
        PushLayer(acq);

    }

    Application::~Application() {}

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatch(e);
        dispatch.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(Application::OnWindowCloseEvent));

        for (auto iter = m_layerStack.rbegin(); iter != m_layerStack.rend(); iter++)
        {
            if (e.handled)
                break;

            (*iter)->OnEvent(e);
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_layerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    bool Application::OnWindowCloseEvent(WindowCloseEvent& e)
    {
        m_running = false;
        return true;
    }

    void Application::Run()
    {
        if (!m_running)
            DY_ERROR("Application attempting to run after having already exited");

        float bckgndColor[4] = { 0.1, 0.1, 0.1, 1.0 };

        //Time stuff
        double currentTime = 0.0;
        double prevTime = 0.0;
        double timestep;

        while (m_running)
        {
            RenderCommand::SetClearColor(bckgndColor);
            RenderCommand::Clear();

            currentTime = RenderCommand::GetFrameTime();
            timestep = currentTime - prevTime;
            prevTime = currentTime;

            for (auto& layer : m_layerStack)
                layer->OnUpdate();

            m_imguiLayer->Begin();
            for (auto& layer : m_layerStack)
                layer->OnImGuiRender(timestep);
            m_imguiLayer->End();

            m_window->OnUpdate();
        }

    }
}