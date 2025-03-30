#ifndef LYNQ_LED_H
#define LYNQ_LED_H 1
#ifdef __cplusplus
extern "C" {
#endif

int lynq_set_netled_on(const int test_mode);
int lynq_set_statusled_on(const int test_mode);
#ifdef __cplusplus
}
#endif
#endif
