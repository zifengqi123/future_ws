#ifndef __LIBLOG_H__
#define __LIBLOG_H__

#include <unistd.h>
#include "mbtk_log.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    LOG_UNSET = -1,
    LOG_VERBOSE = 8,
    LOG_ERROR = 3, 
    LOG_WARNING = 4,
    LOG_INFO = 6,
    LOG_DEBUG = 7, 
    LOG_LEVEL_MAX = 9
}log_level_enum;

void lynq_log_global_output(log_level_enum Level,const char *format,...);
void lynq_log_configuration_init(const char *log_name);
const char* lynq_read_log_version();

//log level api
int lynq_set_log_level(const char * module_name, log_level_enum level);
int lynq_get_log_level(const char * module_name, log_level_enum *level);
int lynq_set_special_log_level(const char * exe_name, const char * module_name, log_level_enum level);
int lynq_get_special_log_level(const char * exe_name, const char * module_name, log_level_enum *level);
int lynq_notify_recalc_log_level(pid_t pid);

#ifdef __cplusplus
}
#endif


#endif  //__LOG_H__
