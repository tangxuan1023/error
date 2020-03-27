#pragma once

#if defined _WIN32 || defined __CYGWIN__
#define GLOBAL_SYMBOL_EXPORT __declspec(dllexport)
#define GLOBAL_SYMBOL_IMPORT __declspec(dllimport)
#define LOCAL_SYMBOL
#else
#if defined(__GNUC__) && (__GNUC__ >= 4) && !defined(__APPLE__)
#define GLOBAL_SYMBOL_EXPORT __attribute__ ((visibility ("default")))
#define GLOBAL_SYMBOL_IMPORT __attribute__ ((visibility ("default")))
#define LOCAL_SYMBOL         __attribute__ ((visibility ("hidden")))
#else
#define GLOBAL_SYMBOL_EXPORT
#define GLOBAL_SYMBOL_IMPORT
#define LOCAL_SYMBOL
#endif
#endif

#if !defined(PUBLIC_LIB_API)
#ifdef LIB_BUILDING_SHARED /* defined if we are building the shared (instead of using it) */
#define PUBLIC_LIB_API GLOBAL_SYMBOL_EXPORT
#else  /* using shared lib */
#define PUBLIC_LIB_API GLOBAL_SYMBOL_IMPORT
#endif /* LIB_BUILDING_SHARED */
#define PRIVATE_LIB_LOCAL LOCAL_SYMBOL
#endif /* PUBLIC_LIB_API */
