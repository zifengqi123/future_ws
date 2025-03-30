#ifndef MBTK_LOG_INCLUDE
#define MBTK_LOG_INCLUDE
#include <string.h>

#ifndef MBTK_ANDROID_LOG
//#define MBTK_ANDROID_LOG
#endif

#ifdef MBTK_ANDROID_LOG

#ifndef LOG_TAG
#define LOG_TAG "mbtk_log"
#endif

// Print ALOGV log.
#ifndef LOG_NDEBUG
//#define LOG_NDEBUG 0
#endif

#include <cutils/log.h>

#define LOGV(fmt, arg...) ALOGV("Line-%d: " fmt,__LINE__,##arg)
#define LOGI(fmt, arg...) ALOGI("Line-%d: " fmt,__LINE__,##arg)
#define LOGD(fmt, arg...) ALOGD("Line-%d: " fmt,__LINE__,##arg)
#define LOGW(fmt, arg...) ALOGW("Line-%d: " fmt,__LINE__,##arg)
#define LOGE(fmt, arg...) ALOGE("Line-%d: " fmt,__LINE__,##arg)

#else /* MBTK_ANDROID_LOG */

//#include <log.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef UNUSED
#define UNUSED(a)    (void)(a)
#endif

//#include "pxa_dbg.h"

#ifdef __cplusplus
extern "C" {
#endif
void mbtk_log(int level, const char *format, ...);
void mbtk_log_init(char *path, char *tag);
void log_hex(const char *tag, const void *data, int data_len);
#ifdef __cplusplus
}
#endif

#ifndef LOG_ERR_LEVEL
#define LOG_ERR_LEVEL  3      /* error conditions */
#endif
#ifndef LOG_WARN_LEVEL
#define LOG_WARN_LEVEL 4   /* warning conditions */
#endif
#ifndef LOG_INFO_LEVEL
#define LOG_INFO_LEVEL 6      /* informational */
#endif
#ifndef LOG_DEBUG_LEVEL
#define LOG_DEBUG_LEVEL 7     /* debug-level messages */
#endif
#ifndef LOG_VERBOSE_LEVEL
#define LOG_VERBOSE_LEVEL 8
#endif

#define LOGV(fmt, args ...) \
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

#define LOGI(fmt, args...) \
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

#define LOGD(fmt, args...) \
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
        mbtk_log(LOG_DEBUG_LEVEL, "%s#%s: " fmt, ptr_1001 + 1, line_1001, ##args); \
    } while(0)

#define LOGW(fmt, args...) \
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

#define LOGE(fmt, args...) \
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
#endif

#define MBTK_SOURCE_INFO_PRINT(name) \
    do{ \
        LOGD("%s : %s(%s)", name, MBTK_BUILD_GIT, MBTK_BUILD_TIME); \
        printf("%s : %s(%s)\n", name, MBTK_BUILD_GIT, MBTK_BUILD_TIME); \
    } while(0)


void mbtk_debug_open(const char *log_file, bool thread_support);

void ql_lib_info_print();
void mbtk_tcpip_lib_info_print();
void mbtk_net_lib_info_print();
void mbtk_mqtt_lib_info_print();
void mbtk_http_lib_info_print();
void mbtk_gnss_lib_info_print();
void mbtk_ftp_lib_info_print();
void mbtk_fota_lib_info_print();
void mbtk_factory_lib_info_print();
void mbtk_coap_lib_info_print();
void mbtk_audio_lib_info_print();
void lynq_lib_info_print();
void mbtk_ril_lib_info_print();
void mbtk_lib_info_print();

#endif /* MBTK_LOG_INCLUDE */
