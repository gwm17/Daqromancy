#include "Application.h"

int main(int argc, char** argv)
{
    Daqromancy::Logger::Init();
    DY_INFO("Welcome to Daqromancy.");

    Daqromancy::ApplicationArgs args;
    args.name = "Daqromancy";
    args.runtimePath = "";
    args.argc = argc;
    args.argv = argv;

    Daqromancy::Application* app = Daqromancy::CreateApplication(args);

    app->Run();

    delete app;
}