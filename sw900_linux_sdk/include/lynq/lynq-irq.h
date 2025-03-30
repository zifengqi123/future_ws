#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif


#include <linux/ioctl.h>
#include <linux/types.h>

struct sc_irq_info {
	unsigned int	line;
	unsigned int	type;
};

#define SC_IRQ_IOC_MAGIC            'I'

/*ioctl cmd usd by device*/
#define SC_IRQ_INSTALL              _IOW(SC_IRQ_IOC_MAGIC, 1, char *)
#define SC_IRQ_SET_TYPE             _IOW(SC_IRQ_IOC_MAGIC, 2, char *)
#define SC_IRQ_SET_WAKE             _IOW(SC_IRQ_IOC_MAGIC, 3, char *)
#define SC_IRQ_GET_WAKE             _IOW(SC_IRQ_IOC_MAGIC, 4, char *)
#define SC_IRQ_UNINSTALL            _IOW(SC_IRQ_IOC_MAGIC, 5, char *)

#define SC_IRQ_GET_STATUS           _IOW(SC_IRQ_IOC_MAGIC, 6, char *)
#define SC_IRQ_CLEAR_STATUS         _IOW(SC_IRQ_IOC_MAGIC, 7, char *)

#define SC_IRQ_DEV                  "/dev/sc_irq"



/*----------------------------------------------------------*/
struct sc_pm_info {
	unsigned int	sleep_time;		/* ms */
	unsigned int	wake_event;
};

#define SC_PM_WL_EVENT_EXT0         ((unsigned int)1 << 0)
#define SC_PM_WL_EVENT_EXT1         ((unsigned int)1 << 1) 
#define SC_PM_WL_EVENT_EXT2         ((unsigned int)1 << 2)
#define SC_PM_WL_EVENT_EXT3         ((unsigned int)1 << 3)
#define SC_PM_WL_EVENT_EXT4         ((unsigned int)1 << 4)
#define SC_PM_WL_EVENT_EXT5         ((unsigned int)1 << 5)
#define SC_PM_WL_EVENT_EXT6         ((unsigned int)1 << 6)
#define SC_PM_WL_EVENT_EXT7         ((unsigned int)1 << 7)

#define SC_PM_IOC_MAGIC             'P'

/*ioctl cmd usd by device*/
#define SC_PM_WL_SET                _IOW(SC_PM_IOC_MAGIC, 1, char *)
#define SC_PM_WL_CLEAR              _IOW(SC_PM_IOC_MAGIC, 2, char *)
#define SC_PM_WL_GET                _IOW(SC_PM_IOC_MAGIC, 3, char *)

#define SC_PM_DEV                   "/dev/sc_pm"


typedef void (*irq_handler)(void);

#define SC_LIBIRQ_EXT0						(0)
#define SC_LIBIRQ_EXT1						(1)
#define SC_LIBIRQ_EXT2						(2)
#define SC_LIBIRQ_EXT3						(3)
#define SC_LIBIRQ_EXT4						(4)
#define SC_LIBIRQ_EXT5						(5)
#define SC_LIBIRQ_EXT6						(6)
#define SC_LIBIRQ_EXT7						(7)
#define SC_LIBIRQ_EXT8						(8)
#define SC_LIBIRQ_EXT9						(9)
#define SC_LIBIRQ_EXT10						(10)
#define SC_LIBIRQ_EXT11						(11)
#define SC_LIBIRQ_EXT12						(12)
#define SC_LIBIRQ_EXT13						(13)
#define SC_LIBIRQ_EXT14						(14)
#define SC_LIBIRQ_EXT15						(15)
#define SC_LIBIRQ_MAX						(200)

#define SC_LIBIRQ_TYPE_RISING				(0)
#define SC_LIBIRQ_TYPE_FALLING				(1)
#define SC_LIBIRQ_TYPE_LEVEL_HIGH			(2)
#define SC_LIBIRQ_TYPE_LEVEL_LOW			(3)
#define SC_LIBIRQ_TYPE_MAX					(4)

/*
 *  Add a handler for an interrupt line.
 *
 *  line      		:  The interrupt line
 *  handler         :  Function to be called when the IRQ occurs.
 *  trig_type		:  rising edge or fallling edge
 *
 *  return 0 if succeed, others failed
 */
int sc_irq_install(unsigned int line, irq_handler handler, int trig_type);

/*
 *  free an interrupt allocated with sc_irq_install.
 *
 *  line      		:  The interrupt line
 *
 *  return 0 if succeed, others failed
 */
int sc_irq_uninstall(unsigned int line);

/*
 *  set the irq trigger type for an irq.
 *
 *  line      		:  The interrupt line
 *  trig_type		:  rising edge or fallling edge
 *
 *  return 0 if succeed, others failed
 */
int sc_irq_set_type(unsigned int line, int trig_type);

/*
 *  get the irq trigger type for an irq.
 *
 *  line      		:  The interrupt line
 *  trig_type		:  edge or level type
 *
 *  return 0 if succeed, others failed
 */
int sc_irq_get_type(unsigned int line, int *trig_type);

/*
 *  control irq power management wakeup.
 *
 *  line      		:  The interrupt line
 *  en				:  enable/disable power management wakeup
 *
 *  return 0 if succeed, others failed
 */
int sc_irq_set_wake(unsigned int line, int en);

/*
 *  get the irq awake status for an irq.
 *
 *  line      		:  The interrupt line
 *  en				:  enable/disable power management wakeup
 *
 *  return 0 if succeed, others failed
 */
int sc_irq_get_wake(unsigned int line, int *en);

typedef enum 
{
    Rising_edge_trigger = 0,//：上升沿触发
    Falling_edge_trigger = 1,//：下降沿触发
    High_level_trigger = 2,//：高电平触发
    Low_level_trigger = 3,//：低电平触发

}trig_type_e;


//int line_gpio[15]={-1,48,49,50,51,52,53,54,119,128,129,-1,131,-1,125};//this is the line match gpio
int line_gpio[4]={21, 22, 23 ,24};//this is the line match gpio

typedef void (*irq_handler)(void);

int lynq_irq_install(int line, irq_handler irq_handler, trig_type_e trig_type);

int lynq_irq_uninstall(int line);

int lynq_irq_set_type(int line, int trig_type);

int lynq_irq_get_type(int line);

int lynq_irq_set_wake(int line, int en);

int lynq_irq_get_wake(int line);


#ifdef __cplusplus
}
#endif
