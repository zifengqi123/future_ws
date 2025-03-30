#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LOCK_NUM 128

#define E_READ -2
#define E_WRITE -3
#define E_TABLE_FULL -4
#define E_LOCK_EXIST -5
#define E_INPUT_ERROR -6

typedef enum 
{
    E_QL_LPM_FALLING = 0, /* Falling, Means wakeupin falling to wakeup the module, or wakeupout falling to wakeup mcu
. */
    E_QL_LPM_RISING = 1, /* Rising, Means  wakeupin rising to wakeup the module,  or wakeupout rising to wakeup  mcu. 
*/
}qser_lpm_edge_t;

typedef int qser_lpm_pin_t;


typedef struct{
    qser_lpm_pin_t wakeupin_pin;
    qser_lpm_edge_t wakeupin_edge;
}qser_lpm_wakeupin_data_t;

typedef struct{
    qser_lpm_pin_t wakeupout_pin;
    qser_lpm_edge_t wakeupout_edge;
}qser_lpm_wakeupout_data_t;


typedef void (*qser_lpm_Handler_t)
(
    qser_lpm_edge_t lpm_edge
);

typedef struct{
    qser_lpm_wakeupin_data_t wakeupin;
    qser_lpm_wakeupout_data_t wakeupout;
}qser_pm_cfg_t;


typedef struct
{
    char lock_name[MAX_LOCK_NUM][64];
    int lock_pid[MAX_LOCK_NUM];
} LOCK_TABLE;

int read_lock_table(void);
int qser_lpm_init(qser_lpm_Handler_t qser_lpm_handler, qser_pm_cfg_t *qser_lpm_cfg);
int qser_lpm_deinit(void);
int qser_autosuspend_enable(char enable);
int qser_wakelock_create(const char *name, size_t len);
int qser_wakelock_lock(int fd);
int qser_wakelock_unlock(int fd);
int qser_wakelock_destroy(int fd);

int qser_whitelist_set(char *whitelist);
int qser_whitelist_get(char *whitelist);


#ifdef __cplusplus
}
#endif

