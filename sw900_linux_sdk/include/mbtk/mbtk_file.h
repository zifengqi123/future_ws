/*
*
*
* Author : lb
* Date   : 2022/3/4 16:44:14
*
*/
#ifndef _MBTK_FILE_H
#define _MBTK_FILE_H
#include "mbtk_type.h"

#ifdef MBTK_PLATFORM_LINUX
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#endif

#ifdef MBTK_PLATFORM_QCOMM
#include "fs_public.h"
#include "ds_profile.h"
#include "dsati.h"
#include "dsat_v.h"
#include "rex.h"
#include <stringl/stringl.h>
#include "mbtk_9205_prod.h"
#include "mbtk_socket_api.h"
#endif

#include "mbtk_log.h"

// Return TRUE if file exist,FLASE or not.
bool file_exist(const char *path);

/*
 * Return file descriptor if open file success, return -1 or not.
 *
 * flag : File open flag.
 *  O_RDONLY 以只读方式打开文件
 *  O_WRONLY 以只写方式打开文件
 *  O_RDWR 以可读写方式打开文件
 *      上述三种旗标是互斥
 *
 *  O_CREAT 若欲打开的文件不存在则自动建立该文件。
 *  O_EXCL 如果 O_CREAT 也被设置，此指令会去检查文件是否存在。文件若不存在则
 *      建立该文件，否则将导致打开文件错误。此外，若 O_CREAT 与 O_EXCL 同时设置，
 *      并且欲打开的文件为符号连接，则会打开文件失败。
 *  O_NOCTTY 如果欲打开的文件为终端机设备时，则不会将该终端机当成进程控制终端机。
 *  O_TRUNC 若文件存在并且以可写的方式打开时，此旗标会令文件长度清为 0，而原来存于该文件的资料也会消失。
 *  O_APPEND 当读写文件时会从文件尾开始移动，也就是所写入的数据会以附加的方式加入到文件后面。
 *  O_NONBLOCK 以不可阻断的方式打开文件，也就是无论有无数据读取或等待，都会立即返回进程之中。
 *  O_NDELAY 同 O_NONBLOCK。
 *  O_SYNC 以同步的方式打开文件。
 *  O_NOFOLLOW 如果参数 pathname 所指的文件为一符号连接，则会令打开文件失败。
 *  O_DIRECTORY 如果参数 pathname 所指的文件并非为一目录，则会令打开文件失败
 */
int file_open(const char *path, int flag);

// Return file size,or -1 if get file size fail.
int file_length(int fd);

// Read data of specified length.
int file_read(int fd, void *buf, int nbyte);

// Write data of specified length.
int file_write(int fd, void *buf, int nbyte);

// Close file.
int file_close(int fd);

#endif /* _MBTK_FILE_H */
