#ifndef _LYNQ_UCI_H_
#define _LYNQ_UCI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <uci.h>

#define LYNQ_UCI_MAX_LEN  128
#define LYNQ_UCI_SUCCESS 0
#define LYNQ_UCI_ERROR 1
#define LYNQ_UCI_FILE "lynq_uci"
#define LYNQ_UCI_RO_FILE "lynq_uci_ro"

//static struct uci_context * ctx = NULL;

int lynq_del(char *option);
#if UCI_SUPPORT
    static int uci_get_value(struct uci_option *o, char *out_buf);
#endif //uci_support
int lynq_uci_get(const char *arg, char *out_buf);

int lynq_uci_set(const char *arg);

int lynq_add_section(char *section_type, char *section);

int lynq_set_value(char *section, char *key, char *value);

int lynq_get_value(char *file, char *section, char *key, char *tmp);


int lynq_load_config(char *filename);

#ifdef __cplusplus
}
#endif
#endif
