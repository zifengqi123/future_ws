#ifndef __LYNQ_DEFLOG_H__
#define __LYNQ_DEFLOG_H__
#include "liblog.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*LYNQ_WRITE_LOG_PTR)(log_level_enum level, const char *format, ...);
extern LYNQ_WRITE_LOG_PTR lynq_write_log;
LYNQ_WRITE_LOG_PTR lynq_log_function_init(const char *log_name);

int lynq_syslog_set_file_size(int value);
int lynq_syslog_get_file_size(void);
int lynq_syslog_set_file_rotate(int value);
int lynq_syslog_get_file_rotate(void);

#ifdef __cplusplus
}
#endif
#define lynq_log_verbose(fmt, args...)  \
    do{ \
        char *file_ptr_1001 = __FILE__; \
        char *ptr_1001 = file_ptr_1001 + strlen(file_ptr_1001) - 1;   \
        char line_1001[10] = {0}; \
        sprintf(line_1001, "%d", __LINE__); \
        while(ptr_1001 >= file_ptr_1001 && *ptr_1001){ \
            if(*ptr_1001 == '/') \
                 break; \
            ptr_1001--; \
        } \
        mbtk_log(LOG_VERBOSE_LEVEL, "%s#%s: " fmt, ptr_1001 + 1, line_1001, ##args); \
    } while(0)

#define lynq_log_error(fmt, args...)  \
    do{ \
		char *file_ptr_1001 = __FILE__; \
        char *ptr_1001 = file_ptr_1001 + strlen(file_ptr_1001) - 1;   \
        char line_1001[10] = {0}; \
        sprintf(line_1001, "%d", __LINE__); \
        while(ptr_1001 >= file_ptr_1001 && *ptr_1001){ \
            if(*ptr_1001 == '/') \
                 break; \
            ptr_1001--; \
        } \
        mbtk_log(LOG_ERR_LEVEL, "%s#%s: " fmt, ptr_1001 + 1, line_1001, ##args); \
    } while(0)

#define lynq_log_warning(fmt, args...)  \
    do{ \
        char *file_ptr_1001 = __FILE__; \
        char *ptr_1001 = file_ptr_1001 + strlen(file_ptr_1001) - 1;   \
        char line_1001[10] = {0}; \
        sprintf(line_1001, "%d", __LINE__); \
        while(ptr_1001 >= file_ptr_1001 && *ptr_1001){ \
            if(*ptr_1001 == '/') \
                 break; \
            ptr_1001--; \
        } \
        mbtk_log(LOG_WARN_LEVEL, "%s#%s: " fmt, ptr_1001 + 1, line_1001, ##args); \
    } while(0)

#define lynq_log_info(fmt, args...)  \
    do{ \
        char *file_ptr_1001 = __FILE__; \
        char *ptr_1001 = file_ptr_1001 + strlen(file_ptr_1001) - 1;   \
        char line_1001[10] = {0}; \
        sprintf(line_1001, "%d", __LINE__); \
        while(ptr_1001 >= file_ptr_1001 && *ptr_1001){ \
            if(*ptr_1001 == '/') \
                 break; \
            ptr_1001--; \
        } \
        mbtk_log(LOG_INFO_LEVEL, "%s#%s: " fmt, ptr_1001 + 1, line_1001, ##args); \
    } while(0)

#define lynq_log_debug(fmt, args...)  \
    do{ \
        char *file_ptr_1001 = __FILE__; \
        char *ptr_1001 = file_ptr_1001 + strlen(file_ptr_1001) - 1;   \
        char line_1001[10] = {0}; \
        sprintf(line_1001, "%d", __LINE__); \
        while(ptr_1001 >= file_ptr_1001 && *ptr_1001){ \
            if(*ptr_1001 == '/') \
                 break; \
            ptr_1001--; \
        } \
        mbtk_log(LOG_DEBUG_LEVEL, "%s#%s: " fmt "\n", ptr_1001 + 1, line_1001, ##args); \
    } while(0)

#define LYVERBLOG(fmt, args...)  \
    do{ \
        char *file_ptr_1001 = __FILE__; \
        char *ptr_1001 = file_ptr_1001 + strlen(file_ptr_1001) - 1;   \
        char line_1001[10] = {0}; \
        sprintf(line_1001, "%d", __LINE__); \
        while(ptr_1001 >= file_ptr_1001 && *ptr_1001){ \
            if(*ptr_1001 == '/') \
                 break; \
            ptr_1001--; \
        } \
        mbtk_log(LOG_VERBOSE_LEVEL, "%s#%s: " fmt, ptr_1001 + 1, line_1001, ##args); \
    } while(0)

#define LYERRLOG(fmt, args...)  \
    do{ \
		char *file_ptr_1001 = __FILE__; \
        char *ptr_1001 = file_ptr_1001 + strlen(file_ptr_1001) - 1;   \
        char line_1001[10] = {0}; \
        sprintf(line_1001, "%d", __LINE__); \
        while(ptr_1001 >= file_ptr_1001 && *ptr_1001){ \
            if(*ptr_1001 == '/') \
                 break; \
            ptr_1001--; \
        } \
        mbtk_log(LOG_ERR_LEVEL, "%s#%s: " fmt, ptr_1001 + 1, line_1001, ##args); \
    } while(0)

#define LYWARNLOG(fmt, args...)  \
    do{ \
        char *file_ptr_1001 = __FILE__; \
        char *ptr_1001 = file_ptr_1001 + strlen(file_ptr_1001) - 1;   \
        char line_1001[10] = {0}; \
        sprintf(line_1001, "%d", __LINE__); \
        while(ptr_1001 >= file_ptr_1001 && *ptr_1001){ \
            if(*ptr_1001 == '/') \
                 break; \
            ptr_1001--; \
        } \
        mbtk_log(LOG_WARN_LEVEL, "%s#%s: " fmt, ptr_1001 + 1, line_1001, ##args); \
    } while(0)

#define LYINFLOG(fmt, args...)  \
    do{ \
        char *file_ptr_1001 = __FILE__; \
        char *ptr_1001 = file_ptr_1001 + strlen(file_ptr_1001) - 1;   \
        char line_1001[10] = {0}; \
        sprintf(line_1001, "%d", __LINE__); \
        while(ptr_1001 >= file_ptr_1001 && *ptr_1001){ \
            if(*ptr_1001 == '/') \
                 break; \
            ptr_1001--; \
        } \
        mbtk_log(LOG_INFO_LEVEL, "%s#%s: " fmt, ptr_1001 + 1, line_1001, ##args); \
    } while(0)

#define LYDBGLOG(fmt, args...)  \
    do{ \
        char *file_ptr_1001 = __FILE__; \
        char *ptr_1001 = file_ptr_1001 + strlen(file_ptr_1001) - 1;   \
        char line_1001[10] = {0}; \
        sprintf(line_1001, "%d", __LINE__); \
        while(ptr_1001 >= file_ptr_1001 && *ptr_1001){ \
            if(*ptr_1001 == '/') \
                 break; \
            ptr_1001--; \
        } \
        mbtk_log(LOG_DEBUG_LEVEL, "%s#%s: " fmt "\n", ptr_1001 + 1, line_1001, ##args); \
    } while(0)
// #define lynq_log_verbose(...)  do { lynq_write_log(LOG_VERBOSE,__VA_ARGS__);} while (0)
// #define lynq_log_error(...)  do {lynq_write_log(LOG_ERROR,__VA_ARGS__);} while (0)
// #define lynq_log_warning(...)  do {lynq_write_log(LOG_WARNING,__VA_ARGS__);} while (0)
// #define lynq_log_info(...)  do {lynq_write_log(LOG_INFO,__VA_ARGS__);} while (0)
// #define lynq_log_debug(...)  do {lynq_write_log(LOG_DEBUG,__VA_ARGS__);} while (0)

// #define LYVERBLOG(...)  do {lynq_write_log(LOG_VERBOSE,__VA_ARGS__);} while (0)
// #define LYERRLOG(...)  do {lynq_write_log(LOG_ERROR,__VA_ARGS__);} while (0)
// #define LYWARNLOG(...)  do {lynq_write_log(LOG_WARNING,__VA_ARGS__);} while(0)
// #define LYINFLOG(...)  do {lynq_write_log(LOG_INFO,__VA_ARGS__);} while (0)
// #define LYDBGLOG(...)  do {lynq_write_log(LOG_DEBUG,__VA_ARGS__);} while (0)

// // just for comcompatibility, do nothing
// #define LYLOGSET(a) do{;}while(0)
// #define LYLOGEINIT(Y) do{;}while(0)

// #ifndef ALOGV
// #define ALOGV(...) do {lynq_write_log(LOG_VERBOSE,__VA_ARGS__);} while (0)
// #endif

// #ifndef ALOGE
// #define ALOGE(...) do {lynq_write_log(LOG_ERROR,__VA_ARGS__);} while (0)
// #endif

// #ifndef ALOGW
// #define ALOGW(...) do {lynq_write_log(LOG_WARNING,__VA_ARGS__);} while (0)
// #endif

// #ifndef ALOGI
// #define ALOGI(...) do {lynq_write_log(LOG_INFO,__VA_ARGS__);} while (0)
// #endif

// #ifndef ALOGD
// #define ALOGD(...) do {lynq_write_log(LOG_DEBUG,__VA_ARGS__);} while (0)
// #endif

#define DEFINE_LYNQ_LIB_LOG(tag) \
    LYNQ_WRITE_LOG_PTR __attribute__ ((visibility ("hidden"))) lynq_write_log = NULL; \
    void __attribute__((constructor)) tag##_init() \
    { \
        lynq_write_log = lynq_log_function_init(#tag); \
    }

#define DEFINE_LYNQ_EXE_LOG(tag) \
    LYNQ_WRITE_LOG_PTR __attribute__ ((visibility ("hidden"))) lynq_write_log = NULL; \
    void __attribute__((constructor)) tag##_init() \
    { \
        lynq_write_log = lynq_log_function_init(#tag); \
        lynq_log_configuration_init(#tag); \
    }

#endif  //__LYNQ_DEFLOG_H__
