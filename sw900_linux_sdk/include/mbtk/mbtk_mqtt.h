#ifndef MBTK_MQTT_H
#define MBTK_MQTT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mqtt/MQTTClient.h"
#include "mqtt/MQTTLinux.h"
enum  iot_ctrl_status_t
{
	IOT_STATUS_LOGIN,
	IOT_STATUS_CONNECT,
	IOT_STATUS_DROP,
};
typedef void  (*pMessageArrived_Fun)(void*,int len);

#define MQTT_TOPIC_SIZE     (128)		//订阅和发布主题长度
#define MQTT_BUF_SIZE       (8 * 1024) 	//接收后发送缓冲区大小
#define MQTT_CONFIG_SIZE    256
#define MBTK_IMQTT_USER_NAME_LEN   (100)
#define MBTK_IMQTT_CLIENT_ID_LEN   (160)
#define MBTK_IMQTT_PASSWORD_LEN    (160)
#define MBTK_ITOPIC_PATH_MAX_LEN (MBTK_IMQTT_CLIENT_ID_LEN+40) //160+40

#define MQTT_HOST "***.***.***.***"		//ip地址
#define MQTT_PORT 61613					//端口号
#define MQTT_USER "admin"				//用户名
#define MQTT_PASS "password"			//密码
#define MQTT_CLIENT_ID "17849359"		//客户端标识

typedef struct {
    Network* network;
    Client Client;
    char sub_topic[MQTT_TOPIC_SIZE];		//存放订阅主题
    char pub_topic[MQTT_TOPIC_SIZE];		//存放发布主题
    char mqtt_buffer[MQTT_BUF_SIZE];		//发送缓冲区
    char mqtt_read_buffer[MQTT_BUF_SIZE];	//接收缓冲区

    unsigned char willFlag;
    MQTTPacket_willOptions will;
    char will_topic[MQTT_TOPIC_SIZE];		//存放遗嘱主题

    pMessageArrived_Fun DataArrived_Cb;

    char mqtt_host[MQTT_CONFIG_SIZE] ;
    char mqtt_client_id[MQTT_CONFIG_SIZE];
    char mqtt_user[MQTT_CONFIG_SIZE];
    char mqtt_pass[MQTT_CONFIG_SIZE];
    int mqtt_port;
    int keepAliveInterval;
    int mqtt_version;
}Cloud_MQTT_t;

typedef struct{
    enum iot_ctrl_status_t iotstatus;
    char model[5];
    char company[32];
} iot_device_info_t;//主题结构体


struct opts_struct {
    char    *clientid;
    int     nodelimiter;
    char    *delimiter;
    enum    QoS qos;
    char    *username;
    char    *password;
    char    *host;
    int     port;
    int     showtopics;
}; //结构体

typedef struct mbtk_mqtt_device_session{
    char product_key[MQTT_CONFIG_SIZE];
    char product_secret[MQTT_CONFIG_SIZE];
    char device_name[MQTT_CONFIG_SIZE];
    char device_secret[MQTT_CONFIG_SIZE];
    int host;
    int port;
} mbtk_mqtt_device_session_t;

void mqtt_module_init(void);
int mqtt_data_write(char *pbuf, int len, char retain);

void iot_mqtt_init(Cloud_MQTT_t *piot_mqtt,char *host,int port ,char *clientid,char *user,char *pass,int keepAliveInterval,int version,char *sub_topic,char *pub_topic,pMessageArrived_Fun mqtt_data_rx_cb);
int mqtt_will_msg_set(Cloud_MQTT_t *piot_mqtt, char *pbuf, int len);
int mqtt_device_connect(Cloud_MQTT_t *piot_mqtt);
int mqtt_device_disconnect(Cloud_MQTT_t *piot_mqtt);
int mbtk_aliyun_mqtt_one_type_one_secret_unregin_device_connect(Cloud_MQTT_t *piot_mqtt);
void mbtk_aliyun_mqtt_one_type_one_secret_unregin_set_info_init(Cloud_MQTT_t *piot_mqtt,mbtk_mqtt_device_session_t *device,int keepAliveInterval,int version,pMessageArrived_Fun mqtt_data_rx_cb);
int mbtk_aliyun_mqtt_one_type_one_secret_unregin_get_regin_info(char *clientId, char *deviceToken);
void iot_aliyun_mqtt_one_type_one_secret_unregin_connect_init(Cloud_MQTT_t *piot_mqtt,mbtk_mqtt_device_session_t *device, char *clientId, char *deviceToken,
         int keepAliveInterval,int version,char *sub_topic,char *pub_topic,pMessageArrived_Fun mqtt_data_rx_cb);
void mqtt_data_rx_cb(void *pbuf, int len);
void *cloud_mqtt_thread(void *arg);
int mbtk_MQTTSubscribe(Client* c, const char* topicFilter, enum QoS qos, messageHandler messageHandler);
int mbtk_MQTTUnsubscribe(Client* c, const char* topicFilter);
int mbtk_MQTTPublish(char *pbuf, int len, char retain,Client* c,const char* pub_topic,enum QoS qos,char dup);
#define mDEBUG(fmt, ...)  printf("%s[%s](%d):" fmt,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif

