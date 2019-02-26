#ifndef BGAME_LOG_H
#define BGAME_LOG_H

#define SPDLOG_TRACE_ON
//#define SPDLOG_DEBUG_ON
#define SPDLOG_ERROR_ON 
#define SPDLOG_INFO_ON

#include "spdlog/spdlog.h"

namespace {
    inline void initLogger(){
#if __ANDROID__
        auto android_logger = spdlog::android_logger("android");
#else
		auto android_logger = spdlog::stdout_color_mt("android");
#endif
        android_logger->critical("Use \"adb shell logcat\" to view this message.");
        spdlog::get("android")->critical("this is a critical");
        spdlog::get("android")->info("this is a info");
        spdlog::get("android")->trace("this is a trace");
        spdlog::get("android")->debug("this is a debug");
    }

    inline std::shared_ptr<spdlog::logger> getLogger(){
        return spdlog::get("android");
    }
}


#ifdef SPDLOG_INFO_ON
#  define SPDLOG_STR_H(x) #x
#  define SPDLOG_STR_HELPER(x) SPDLOG_STR_H(x)
#  ifdef _MSC_VER
#    define SPDLOG_INFO(logger, ...) logger->info("[ " __FILE__ "(" SPDLOG_STR_HELPER(__LINE__) ") ] " __VA_ARGS__)
#  else
#    define SPDLOG_INFO(logger, ...) logger->info("[ " __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) " ] " __VA_ARGS__)
#  endif
#else
#  define SPDLOG_INFO(logger, ...) (void)0
#endif

#ifdef SPDLOG_ERROR_ON
#  define SPDLOG_STR_H(x) #x
#  define SPDLOG_STR_HELPER(x) SPDLOG_STR_H(x)
#  ifdef _MSC_VER
#    define SPDLOG_ERROR(logger, ...) logger->critical("[ " __FILE__ "(" SPDLOG_STR_HELPER(__LINE__) ") ] " __VA_ARGS__)
#  else
#    define SPDLOG_ERROR(logger, ...) logger->critical("[ " __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) " ] " __VA_ARGS__)
#  endif
#else
#  define SPDLOG_ERROR(logger, ...) (void)0
#endif

 #endif //BGAME_LOG_H
