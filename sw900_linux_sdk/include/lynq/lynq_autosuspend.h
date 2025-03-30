#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <dlfcn.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif


struct time_info_t
{
    long sleep_start_time;
    long wakeup_time;
};

int lynq_autosleep_enable(void);
int lynq_autosleep_disable(void);
int lynq_wait_wakeup_event(long *sleep_start_time, long * wakeup_time);

#ifdef MOBILETEK_TARGET_PLATFORM_T106
int lynq_set_lpmode(int lp_mode);
int release_wake_lock(char *name);
int acquire_wake_lock(int lock, char *name);
#endif

#ifdef __cplusplus
}
#endif
