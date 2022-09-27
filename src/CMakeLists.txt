add_executable(Daqromancy)

target_include_directories(Daqromancy
    PUBLIC ../vendor/imgui
    ../vendor/glfw/include
    ../vendor/spdlog/include
    ../vendor/glad/include
    ../vendor/yaml-cpp/include
    ../vendor/implot/
    ../vendor/asio/asio/include
    ./
)

#For Windows, CAENDigitizer is installed non-globally (we assume default pathing cause why would ever be otherwise)
if(MSVC)
    target_include_directories(Daqromancy SYSTEM PUBLIC ${CAEN_WIN_INCL_PATH})
endif()

target_precompile_headers(Daqromancy PRIVATE dypch.h)

target_sources(Daqromancy PRIVATE
    DAQ/Digitizer.h
    DAQ/Digitizer.cpp
    DAQ/DigitizerDefs.h
    Core/Logger.h
    Core/Logger.cpp
    Core/main.cpp
    Core/Application.h
    Core/Application.cpp
    Events/Event.h
    Core/DYCore.h
    Events/AppEvent.h
    Events/MouseEvent.h
    Events/AcqEvent.h
    Core/Layer.h
    Core/Layer.cpp
    Core/LayerStack.h
    Core/LayerStack.cpp
    Core/Window.h
    Platform/OpenGL/OpenGLWindow.h
    Renderer/GraphicsContext.h
    Platform/OpenGL/OpenGLContext.h
    Platform/OpenGL/OpenGLContext.cpp
    Platform/OpenGL/OpenGLWindow.cpp
    Renderer/RendererAPI.h
    Renderer/RendererAPI.cpp
    Renderer/RenderCommand.h
    Renderer/RenderCommand.cpp
    Platform/OpenGL/OpenGLRendererAPI.cpp
    ImGui/ImGuiLayer.h
    ImGui/ImGuiLayer.cpp
    ImGui/ImGuiBuild.cpp
    Editor/EditorLayer.h
    Editor/EditorLayer.cpp
    Editor/FileDialog.h
    Editor/FileDialog.cpp
    DAQ/AcquisitionLayer.h
    DAQ/AcquisitionLayer.cpp
    Core/DYProject.h
    Core/DYProject.cpp
    Editor/DigitizerPanel.h
    Editor/DigitizerPanel.cpp
    DAQ/DigitizerDefs.cpp
    Editor/SyncDialog.cpp
    Core/ProjectSerializer.h
    Core/ProjectSerializer.cpp
    Editor/ScopePanel.h
    Editor/ScopePanel.cpp
    DYio/DYFile.h
    DYio/DYFile.cpp
    DYio/DYRun.h
    DYio/DYRun.cpp
    DYio/TCPServer.h
    DYio/TCPServer.cpp
    DYio/DYListData.h
    DYio/DYMessage.h
    Core/ThreadSafeQueue.h
    Core/DataDistributor.h
    Core/DataDistributor.cpp
    DAQ/DigitizerChain.h
    DAQ/DigitizerChain.cpp
    Core/ScalarDistributor.h
    Core/ScalarDistributor.cpp
    Editor/ScalarPanel.h
    Editor/ScalarPanel.cpp
)

#ImPlot sources
target_sources(Daqromancy PRIVATE
    ../vendor/implot/implot.cpp
    ../vendor/implot/implot.h
    ../vendor/implot/implot_demo.cpp
    ../vendor/implot/implot_internal.h
    ../vendor/implot/implot_items.cpp
    ../vendor/implot/backends/implot_backend.h
    ../vendor/implot/backends/implot_impl_opengl3.cpp
    ../vendor/implot/backends/implot_impl_opengl3.h
)

#Packaged dependencies
target_link_libraries(Daqromancy PRIVATE imgui glfw glad yaml-cpp)

#CAEN dependencies
target_link_libraries(Daqromancy PRIVATE ${CAEN_DGTZ})

#Link OS graphics implementations
set(THREADS_PREFER_PTHREAD_FLAG On)
find_package(Threads REQUIRED)
if(APPLE)
    target_link_libraries(Daqromancy PRIVATE "-framework Cocoa" "-framework CoreVideo" "-framework IOKit" "-framework OpenGL" "-framework Carbon" ${CMAKE_DL_LIBS} Threads::Threads)
    target_compile_definitions(Daqromancy PRIVATE DY_APPLE)
elseif(UNIX)
    target_link_libraries(Daqromancy PRIVATE libGL.so libX11.so ${CMAKE_DL_LIBS} Threads::Threads)
    target_compile_definitions(Daqromancy PRIVATE DY_LINUX)
elseif(MSVC)
    target_link_libraries(Daqromancy PRIVATE opengl32.lib Threads::Threads)
    target_compile_definitions(Daqromancy PRIVATE DY_WINDOWS)
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(Daqromancy PRIVATE BS_RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Daqromancy PRIVATE BS_DEBUG)
endif()

#Some extra defs
target_compile_definitions(Daqromancy PRIVATE GLFW_INCLUDE_NONE YAML_CPP_STATIC_DEFINE IMGUI_IMPL_OPENGL_LOADER_GLAD IMPLOT_BACKEND_ENABLE_OPENGL3)

set_target_properties(Daqromancy PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DY_BINARY_DIR})