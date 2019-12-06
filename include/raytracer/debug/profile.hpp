#ifndef RAYTRACER_DEBUG_PROFILE_H
#define RAYTRACER_DEBUG_PROFILE_H

#ifdef RAYTRACER_EASY_PROFILER
// clang-format off
#include <easy/profiler.h>
#include <easy/arbitrary_value.h>
// clang-format on

#define RT_PROFILE_FUNCTION(...) EASY_FUNCTION(__VA_ARGS__);
#define RT_PROFILE_BLOCK(...) EASY_BLOCK(__VA_ARGS__);
#define RT_PROFILE_BLOCK_END() EASY_END_BLOCK;
#define RT_PROFILE_VALUE(...) EASY_VALUE(__VA_ARGS__);
#define RT_PROFILE_ARRAY(...) EASY_ARRAY(__VA_ARGS__);
#define RT_PROFILE_START() EASY_PROFILER_ENABLE;
#define RT_PROFILE_SAVE_FILE(...) ::profiler::dumpBlocksToFile(__VA_ARGS__);
#else
#define RT_PROFILE_FUNCTION(...)
#define RT_PROFILE_BLOCK(...)
#define RT_PROFILE_BLOCK_END()
#define RT_PROFILE_VALUE(...)
#define RT_PROFILE_ARRAY(...)
#define RT_PROFILE_START()
#define RT_PROFILE_SAVE_FILE(...)
#endif // RAYTRACER_EASY_PROFILER

#endif // RAYTRACER_DEBUG_PROFILE_H
