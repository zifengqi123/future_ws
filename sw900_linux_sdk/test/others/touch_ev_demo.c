#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <linux/input.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <time.h>
#include <pthread.h>
#include <sys/poll.h>
#include <dirent.h>
#include <stdbool.h>

#ifndef TRUE
#define TRUE   1   /* Boolean true value. */
#endif

#ifndef true
#define true   1   /* Boolean true value. */
#endif

#ifndef FALSE
#define FALSE  0   /* Boolean false value. */
#endif

#ifndef false
#define false  0   /* Boolean false value. */
#endif


#ifndef NULL
#define NULL  0
#endif


#ifndef null
#define null  0
#endif

#define LOGI printf
#define LOGE printf

/**
 * Compiler-digit : 16
 * char : 1     (%c)
 * char* : 2
 * short int : 2
 * int : 2      (%d)
 * unsigned int : 2  (%u)
 * float : 4    (%f)
 * double : 8   (%f)
 * long : 4
 * unsigned long : 4
 * long long : 8
 * unsigned long long : 8
 *
 *
 * Compiler-digit : 32
 * char : 1
 * char* : 4
 * short int : 2
 * int : 4
 * unsigned int : 4
 * float : 4
 * double : 8
 * long : 4
 * unsigned long : 4
 * long long : 8
 * unsigned long long : 8
 *
 *
 * Compiler-digit : 64
 * char : 1
 * char* : 8
 * short int : 2
 * int : 4
 * unsigned int : 4
 * float : 4
 * double : 8
 * long : 8
 * unsigned long : 8
 * long long : 8
 * unsigned long long : 8
 */
typedef unsigned char boolean; /* Boolean value type. */
// typedef unsigned char bool; /* Boolean value type. */
typedef unsigned long long uint64; /* Unsigned 64 bit value */
typedef unsigned long long uint64_t; /* Unsigned 64 bit value */
typedef unsigned int uint32; /* Unsigned 32 bit value */
typedef unsigned int uint32_t; /* Unsigned 32 bit value */
typedef unsigned short uint16; /* Unsigned 16 bit value */
typedef unsigned short uint16_t;
typedef unsigned char uint8; /* Unsigned 8  bit value */
typedef unsigned char uint8_t;
typedef signed long long int64; /* Signed 64 bit value */
typedef signed long long sint64; /* Signed 64 bit value */
typedef signed int int32; /* Signed 32 bit value */
typedef signed int sint32; /* Signed 32 bit value */
typedef signed short int16; /* Signed 16 bit value */
typedef signed short sint16; /* Signed 16 bit value */
typedef signed char int8; /* Signed 8  bit value */
typedef signed char sint8; /* Signed 8  bit value */
typedef unsigned char byte; /* byte type */

//#include "mbtk_type.h"
//#include "mbtk_log.h"

typedef int (*ev_callback)(int fd, uint32_t epevents, void *data);

typedef enum {
    ACTION_DOWN,
    ACTION_MOVE,
    ACTION_UP,
    ACTION_CANCEL
} touch_event_action_enum;

typedef struct {
    touch_event_action_enum action;
    int x;
    int y;
} touch_event;

static int move_x;
static touch_event event_pre;
static bool action_down_get = false;
static bool action_touched = false;

#define MAX_DEVICES 16
#define MAX_MISC_FDS 16

#define BITS_PER_LONG (sizeof(unsigned long) * 8)
#define BITS_TO_LONGS(x) (((x) + BITS_PER_LONG - 1) / BITS_PER_LONG)

#define test_bit(bit, array) \
    ((array)[(bit)/BITS_PER_LONG] & (1 << ((bit) % BITS_PER_LONG)))

struct fd_info {
    ev_callback cb;
    void *data;
};

static struct pollfd ev_fds[MAX_DEVICES + MAX_MISC_FDS];
static struct fd_info ev_fdinfo[MAX_DEVICES + MAX_MISC_FDS];

static unsigned ev_count = 0;
static unsigned ev_dev_count = 0;
static unsigned ev_misc_count = 0;

int ev_get_input(int fd, short revents, struct input_event *ev);
bool event_process(struct input_event ev);

int ev_cb(int fd, uint32_t epevents, void *data)
{
    struct input_event ev;

    int retval = ev_get_input(fd, epevents, &ev);
    if(retval < 0) return -1;

    if(!event_process(ev)) return 0;

    return 0;
}

int ev_init(void *data)
{
    DIR *dir;
    struct dirent *de;
    int fd;

    dir = opendir("/dev/input");
    if(dir != 0) {
        while((de = readdir(dir))) {
            unsigned long ev_bits[BITS_TO_LONGS(EV_MAX)];

//            fprintf(stderr,"/dev/input/%s\n", de->d_name);
            if(strncmp(de->d_name,"event",5)) continue;
            fd = openat(dirfd(dir), de->d_name, O_RDONLY);
            if(fd < 0) continue;

            /* read the evbits of the input device */
            if (ioctl(fd, EVIOCGBIT(0, sizeof(ev_bits)), ev_bits) < 0) {
                close(fd);
                continue;
            }

            /* TODO: add ability to specify event masks. For now, just assume
             * that only EV_KEY and EV_REL event types are ever needed. */
            if (!test_bit(EV_KEY, ev_bits) && !test_bit(EV_REL, ev_bits)) {
                close(fd);
                continue;
            }

            ev_fds[ev_count].fd = fd;
            ev_fds[ev_count].events = POLLIN;
            ev_fdinfo[ev_count].cb = ev_cb;
            ev_fdinfo[ev_count].data = data;
            ev_count++;
            ev_dev_count++;
            if(ev_dev_count == MAX_DEVICES) break;
        }
    }

    return 0;
}

void ev_exit(void)
{
    while (ev_count > 0) {
        close(ev_fds[--ev_count].fd);
    }
    ev_misc_count = 0;
    ev_dev_count = 0;
}

int ev_wait(int timeout)
{
    int r;

    r = poll(ev_fds, ev_count, timeout);
    if (r <= 0)
        return -1;
    return 0;
}

void ev_dispatch(void)
{
    unsigned n;
    int ret;

    for (n = 0; n < ev_count; n++) {
        ev_callback cb = ev_fdinfo[n].cb;
        if (cb && (ev_fds[n].revents & ev_fds[n].events))
            cb(ev_fds[n].fd, ev_fds[n].revents, ev_fdinfo[n].data);
    }
}

int ev_get_input(int fd, short revents, struct input_event *ev)
{
    int r;

    if (revents & POLLIN) {
        r = read(fd, ev, sizeof(*ev));
        if (r == sizeof(*ev))
            return 0;
    }
    return -1;
}

bool event_process(struct input_event ev)
{
    LOGI("Event:%d,%d,%d\n", ev.type, ev.code, ev.value);

//    if(ev.type != EV_KEY){
//        return false;
//    }

    bool is_touch = true;
    // Touch Down/Up
    if(ev.type == EV_KEY && ev.code == BTN_TOUCH) {
        if(!!ev.value) { // Down
            action_down_get = true;
            action_touched = true;
        }
        else // UP
        {
            action_down_get = false;
            action_touched = false;
        }
    } else if(ev.type == EV_ABS) { // Touch move
        if(!action_touched) // No down
            return false;
    } else if(ev.type != EV_KEY) {
        return false;
    } else {
        is_touch = false;
    }

    if (!is_touch && ev.value < 2){ // 2 is long press events
        int down = !!ev.value;
        if (down){
            LOGI("LongPress : DOWN.");
            //if(!l->onKeyDown(ev.type, ev.code)) return false;
        }else{
            //if(!l->onKeyUp(ev.type, ev.code)) return false;
            LOGI("LongPress : UP.");
        }
    } else if (is_touch) {
        touch_event m_event;
        if(ev.type == EV_ABS) { // Move
            if(ev.code == KEY_SLASH) { // X
                move_x = ev.value;
            } else if(ev.code == KEY_RIGHTSHIFT) { // Y
                if(action_down_get)
                {
                    m_event.action = ACTION_DOWN;
                    action_down_get = false;
                } else {
                    m_event.action = ACTION_MOVE;
                }
                m_event.x = move_x;
                m_event.y = ev.value;

                if(event_pre.x != m_event.x
                    || event_pre.y != m_event.y)
                {
                    event_pre.x = m_event.x;
                    event_pre.y = m_event.y;
                #ifdef MBTK_TP_RESIZE_SUPPORT
                    point_resize(getScreenWidth(),
                        getScreenHeight(),
                        mStatusBar->isVisibility() ? mStatusBar->getHeight() : 0,
                        &m_event);
                #endif
                    LOGI("Window onTouchEvent action:%d (%d,%d) -> (%d,%d)",
                        m_event.action, event_pre.x, event_pre.y, m_event.x, m_event.y);

                }
            } else {
                // Do nothing
            }
        } else if(!action_down_get){ // UP
            m_event.action = ACTION_UP;
            m_event.x = event_pre.x;
            m_event.y = event_pre.y;

        #ifdef MBTK_TP_RESIZE_SUPPORT
            point_resize(getScreenWidth(),
                getScreenHeight(),
                mStatusBar->isVisibility() ? mStatusBar->getHeight() : 0,
                &m_event);
        #endif

            LOGI("Window onTouchEvent action:%d (%d,%d) -> (%d,%d)",
                m_event.action, event_pre.x, event_pre.y, m_event.x, m_event.y);

        } else {
            // Do nothing
        }
    } else {
        // Do nothing
    }

    //invalidate();

    return true;
}

int main(int argc, char *argv[])
{
    mbtk_log_init(NULL, "MBTK_EVENT");

    if(ev_init(NULL)) {
        LOGE("ev_init() fail.");
        return -1;
    }

    LOGI("event getting...");
    while(1) {
        if(!ev_wait(-1))
            ev_dispatch();
    }

    LOGI("exit!!!");
    return 0;
}
