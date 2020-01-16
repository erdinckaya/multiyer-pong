#pragma once


#include <spdlog/spdlog.h>
#include <thread>
#include <sstream>


#define THREAD_ID(str) \
	std::string str;\
	do {	\
		std::thread::id this_id = std::this_thread::get_id(); \
		std::ostringstream stream; \
		stream << this_id; \
		str = stream.str(); \
	}while(0)

#define CHECK_ERROR(test, message) \
	do { \
	    if((test)) { \
	        spdlog::critical("{0}", message); \
	    } \
	} while(0)


#define CHECK_ERROR_EXIT(test, message) \
	do { \
	    if((test)) { \
	        spdlog::critical("{0}", message); \
	        exit(1); \
	    } \
	} while(0)

#define LOG_ERROR(message) \
	do { \
	    spdlog::critical("{0}", message); \
	} while(0)

#define CLAMP(val, l, r) \
	do { \
	    val = std::min(std::max((val), (l)), (r)); \
	} while(0)

#define RANDOM_IN_RANGE(val, max, min) \
	auto val = 0;\
	do { \
		auto range = (max) - (min) + 1; \
		val = rand() % range + min; \
	} while(0)


#ifdef CLIENT
#define DEBUG_CLIENT(log_type, ...) \
	do { \
	    spdlog::log_type(__VA_ARGS__); \
	} while(0)
#endif

#ifdef SERVER
#define DEBUG_SERVER(log_type, ...) \
	do { \
	    spdlog::log_type(__VA_ARGS__); \
	} while(0)
#endif


#ifdef CLIENT
#define DEBUG_CLIENT_COND(log_type, cond, ...) \
	do { \
	    if (cond) \
			spdlog::log_type(__VA_ARGS__); \
	} while(0)
#endif

#ifdef SERVER
#define DEBUG_SERVER_COND(log_type, cond, ...) \
	do { \
	    if (cond) \
			spdlog::log_type(__VA_ARGS__); \
	} while(0)
#endif

#define DEBUG_COND(cond, ...) \
	do { \
	    if (cond) \
			spdlog::debug(__VA_ARGS__); \
	} while(0)

#define DEBUG_LOG(...) \
	do { \
	    spdlog::debug(__VA_ARGS__); \
	} while(0)


#define DEG2RAD(d) ((((d) * PI)) / 180)

#define RAD2DEG(r) ((((r) * 180)) / PI)

#define BOOL_TO_STR(b) std::string(b ? "true" : "false")