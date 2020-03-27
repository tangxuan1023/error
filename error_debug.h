#pragma once
#include "export_settings.h"

#ifndef DEBUG_TAG
#define DEBUG_TAG "ERR_DEBUG"
#endif

#define FILE_FUNC_LINE __FILE__, __func__, __LINE__

#define UNDEFPTR ((void *) undefined_object)
extern const char undefined_object[];

#ifdef __cplusplus
extern "C" {
#endif

typedef enum err_dbg_e
{
    ERR_FATAL,
    ERR_ERROR,
    ERR_WARN,
    ERR_INFO,
    ERR_DEBUG,
    ERR_VERBOSE,
    ERR_MAX
} e_dbg_lvl_t;

typedef void(*err_cb_f)(void* user_data, e_dbg_lvl_t, const char *, ...);

PUBLIC_LIB_API void err_msg(e_dbg_lvl_t lvl, const char *filepath, const char *funcname, long lineno, const char *fmt, ...);
PUBLIC_LIB_API void err_set_callback(err_cb_f cb, void* user_data);
PUBLIC_LIB_API int  err_set_logfile(const char *path);
PUBLIC_LIB_API int  err_set_debug_level(int level);

#define LOGF(...)                                                \
    do                                                               \
    {                                                                \
        err_msg(ERR_FATAL, FILE_FUNC_LINE, __VA_ARGS__); \
        exit(EXIT_FAILURE);                                          \
    } while (0)

#if 1
#define LOGE(...) err_msg(ERR_ERROR, FILE_FUNC_LINE, __VA_ARGS__)
#define LOGW(...) err_msg(ERR_WARN, FILE_FUNC_LINE, __VA_ARGS__)
#define LOGI(...) err_msg(ERR_INFO, FILE_FUNC_LINE, __VA_ARGS__)
#define LOGD(...) err_msg(ERR_DEBUG, FILE_FUNC_LINE, __VA_ARGS__)
#define LOGV(...) err_msg(ERR_VERBOSE, FILE_FUNC_LINE, __VA_ARGS__)
#else
#define LOGE(...)
#define LOGW(...)
#define LOGI(...)
#define LOGD(...)
#define LOGV(...)
#endif

#ifdef __cplusplus
}
#endif