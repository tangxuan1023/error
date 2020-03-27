#include "error_debug.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#if defined(__ANDROID__)
#include <android/log.h>
static void
err_logcat_cb(void* user_data, err_lvl_t level, const char *fmt, ...);
#endif
static void err_logfp_cb(void *user_data, e_dbg_lvl_t lvl, const char *fmt, ...);
static const char *get_basename(const char *path);
static FILE *err_get_logfp(void);
static void err_set_logfp(FILE *stream);

#if defined(__ANDROID__)
static err_cb_f err_cb = err_logcat_cb;
#else
static err_cb_f err_cb = err_logfp_cb;
#endif
static void* err_user_data;

static FILE*  logfp = NULL;
static int    logfp_disabled = false;
static int    _debug_level = 1;

const char undefined_object[] = "UNDEFINED";

const char *get_basename(const char *path)
{
	const char *result;

#if defined(_WIN32) || defined(__CYGWIN__)
	result = strrchr(path, '\\');
#else
	result = strrchr(path, '/');
#endif

	return (result == NULL ? path : result + 1);
}

void err_msg(e_dbg_lvl_t lvl, const char *filepath, const char *funcname, long lineno, const char *fmt, ...)
{
	static const char *err_prefix[ERR_MAX] = {
		"FATAL", "ERROR", 
		"WARN", "INFO", 
		"DEBUG", "VERBOSE"
	};

    char msg[1024];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

    if (filepath && funcname) {
        const char *fname = get_basename(filepath);
    	err_cb(err_user_data, lvl, "%s: %s:%ld /%s/: %s", err_prefix[lvl], fname, lineno, funcname, msg);
    } 
    else {
        err_cb(err_user_data, lvl, "%s", msg);
    }
}

int err_set_logfile(const char *path)
{
	FILE *newfp, *oldfp;

	if ((newfp = fopen(path, "a")) == NULL)
		return -1;
	oldfp = err_get_logfp();
	err_set_logfp(newfp);
	if (oldfp != NULL && oldfp != stdout && oldfp != stderr)
		fclose(oldfp);
	return 0;
}

void err_set_logfp(FILE *stream)
{
	if (stream == NULL) {
		logfp_disabled = true;
		logfp = NULL;
		return;
	}
	logfp_disabled = false;
	logfp = stream;
	return;
}

FILE *err_get_logfp(void)
{
	if (logfp_disabled)
		return NULL;
	if (logfp == NULL)
		return stderr;

	return logfp;
}

void err_set_callback(err_cb_f cb, void* user_data)
{
	err_cb = cb;
	err_user_data = user_data;
}

int err_set_debug_level(int level)
{
	int prev = _debug_level;
	_debug_level = level;
	return prev;
}

#if defined(__ANDROID__)
static void
err_logcat_cb(void *user_data, err_lvl_t lvl, const char *fmt, ...)
{
	if (lvl > _debug_level) {
		return;
	}

	static const int android_level[ERR_MAX] = { 
		ANDROID_LOG_ERROR,
		ANDROID_LOG_ERROR,
		ANDROID_LOG_WARN,
		ANDROID_LOG_INFO,
		ANDROID_LOG_DEBUG,
		ANDROID_LOG_VERBOSE,
	};

	va_list ap;
	va_start(ap, fmt);
	__android_log_vprint(android_level[lvl], DEBUG_TAG, fmt, ap);
	va_end(ap);
}
#else
void err_logfp_cb(void *user_data, e_dbg_lvl_t lvl, const char *fmt, ...)
{
	if (lvl > _debug_level) {
		return;
	}

	va_list ap;
	FILE *fp = err_get_logfp();

	if (!fp)
		return;

	va_start(ap, fmt);
	vfprintf(fp, fmt, ap);
	va_end(ap);
}
#endif