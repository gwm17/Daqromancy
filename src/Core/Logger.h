#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Daqromancy {

    class Logger
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger> GetLogger() { return s_logger; }

    private:
        static std::shared_ptr<spdlog::logger> s_logger;
    };


//Macros for simple logging.

#define DY_CRITICAL(...) ::Daqromancy::Logger::GetLogger()->critical(__VA_ARGS__)
#define DY_WARN(...) ::Daqromancy::Logger::GetLogger()->warn(__VA_ARGS__)
#define DY_INFO(...) ::Daqromancy::Logger::GetLogger()->info(__VA_ARGS__)
#define DY_TRACE(...) ::Daqromancy::Logger::GetLogger()->trace(__VA_ARGS__)
#define DY_ERROR(...) ::Daqromancy::Logger::GetLogger()->error(__VA_ARGS__)
}

#endif