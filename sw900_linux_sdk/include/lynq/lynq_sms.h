#ifndef LYNQ_SMS_H
#define LYNQ_SMS_H 1
#ifdef __cplusplus
extern "C" {
#endif

#define MIN_MSM_PARAM_NUM 4
#define MIN_IMS_MSM_PARAM_NUM 6
#define MIN_WRITE_MSM_PARAM_NUM 5
#define MSG_MAX_LEN 1024
#define TELEPHONE_NUM_LEN 64
#define STORAGE_SMS_MAX_SIZE 128
#define SMSC_MAX_LEN 22
#define SMS_NUM_MAX 255

typedef enum
{
    SMS_ERR = -1,
    SMS_OK = 0,
    SMS_KNONW = 1,
    SMS_UNKNONW = 2
}status_SMS;

int lynq_sms_init(int uToken);
int lynq_sms_deinit(void);
int lynq_send_sms(char telephony_num[TELEPHONE_NUM_LEN], int charset, char *msg, int msglen);
int lynq_read_sms(int index,int *status,int *charset,char smsc[SMSC_MAX_LEN],int *smscLen,int *smslen,char message[MSG_MAX_LEN],char teleNum[TELEPHONE_NUM_LEN],int *numLen,int *current,int *total);
int lynq_get_smsc_address(char service_num[SMSC_MAX_LEN]);
int lynq_set_smsc_address(const char* service_num);
int lynq_list_sms(char index_list[SMS_NUM_MAX]);
int lynq_delete_sms(int index);
int lynq_wait_receive_new_sms(int *handle);
#ifdef __cplusplus
}
#endif
#endif
