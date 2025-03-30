#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dlfcn.h>
#include <errno.h>
#include <sys/un.h>
#include <ctype.h>
#include <unistd.h>
#include <lynq/lynq-qser-data.h>

#define MAX_COMMAND_LEN 10


void evt_cb(qser_data_call_state_s *state);
int qser_init_data();
int qser_deinit_data();
int data_demo_auto_test(char *apn_name1, char *apn_name2);

qser_data_call_state_s state = {0};

void delete_enter(char data[])
{
    char *find = strchr(data, '\n');
    if(find)
        *find = '\0';
    return ;
}
void printf_help(void)
{
    printf("*******************************User Guide*************************************************\n");
    printf("usage: lynq-qser-data-demo <apn_name> <apn_type>\n");
    printf("Example: lynq-qser-data-demo cmwap lynq_apn1\n");
    printf("*******************************************************************************************\n");
    return;
}

/**
 * @brief qser data call demo entry function.
 *
 * @detail This example will take a complete data process
 *         and show you how to call the API in liblynq-qser-data.
 */
int main(int argc, char *argv[])
{
    int i, ret;

    printf("[%s] [%d] entry !\n", __FUNCTION__, __LINE__);
    printf("[DATA_DEMO]lynq-qser-data-demo entry !\n");

	if(argc != 3)
    {
        printf_help();
        exit(EXIT_FAILURE);
    }

	//do initialization
    ret = qser_data_call_init(evt_cb);
    if(0 != ret)
    {
        printf("initial failed !\n");
	    return -1;
    }

	data_demo_auto_test(argv[1], argv[2]);
	qser_data_call_destroy();

	return 0;
}

//demo示例拨两路apn，一路使用默认apn配置拨号访问公网，另一路建议为私网配置
int data_demo_auto_test(char *apn_name1, char *apn_type)
{
	int ret = 0;
	char command[MAX_COMMAND_LEN] = {'\0'};
	bool apn_test1_need_insert = true;
	unsigned char profile_idx_char1;
	qser_apn_add_s apn_test1 = {QSER_APN_PDP_TYPE_IPV4V6, QSER_APN_AUTH_PROTO_DEFAULT, NULL, NULL, NULL, NULL};
	qser_data_call_s apn_normal_datacall;
	qser_data_call_s apn_test1_datacall;
    qser_data_call_error_e err = QSER_DATA_CALL_ERROR_NONE;

	memcpy(apn_test1.apn_type,apn_type,QSER_APN_NAME_SIZE);
	memcpy(apn_test1.apn_name,apn_name1,QSER_APN_NAME_SIZE);

	//1、检查apn是否存在
	qser_apn_info_list_s apn_list = {0};
    ret = qser_apn_get_list(&apn_list);
	if(ret != 0)
	{
		printf("\n get apn list error\n");
		return -1;
	}
    for(int i = 0; i < apn_list.cnt; i++)
    {
        printf("data_demo_auto_test: pdp_type=%d, auth_proto=%d, apn_name=%s, username=%s, password=%s, apn_type=%s\n"
            ,apn_list.apn[i].pdp_type, apn_list.apn[i].auth_proto, apn_list.apn[i].apn_name, apn_list.apn[i].username, apn_list.apn[i].password, apn_list.apn[i].apn_type);

		if( apn_list.apn[i].pdp_type == apn_test1.pdp_type
			&& apn_list.apn[i].auth_proto == apn_test1.auth_proto
			&& (strcmp(apn_list.apn[i].apn_name, apn_test1.apn_name) == 0)
			&& (strcmp(apn_list.apn[i].username, apn_test1.username) == 0)
			&& (strcmp(apn_list.apn[i].password, apn_test1.password) == 0)
			&& (strcmp(apn_list.apn[i].apn_type, apn_test1.apn_type) == 0)
			)
		{
			profile_idx_char1 = apn_list.apn[i].profile_idx;
			apn_test1_need_insert = false;
		}
	}
	//2、若不存在，插入apn
	if(apn_test1_need_insert)
	{
		ret = qser_apn_add(&apn_test1, &profile_idx_char1);
		if(ret != 0)
		{
			printf("\n add apn error\n");
			return -1;
		}
	}
	//3、拨号
	apn_normal_datacall.profile_idx = 0;//默认apn profile_idx
	apn_test1_datacall.profile_idx = profile_idx_char1;
	//拨号时实际使用的ip_name,userdata,password等配置以apn信息中的为准，不使用datacall中的配置
	ret = qser_data_call_start(&apn_normal_datacall, &err);//默认apn拨号
	if(ret != 0)
	{
		printf("\nERROR: setup data call fail!!!\n");
		return -1;
	}

	ret = qser_data_call_start(&apn_test1_datacall, &err);
	if(ret != 0)
	{
		printf("\nERROR: setup data call fail!!!\n");
		return -1;
	}

	printf("\n[DATA_DEMO]Enter --exit to exit data demo\n");
	while(1)
	{
		fgets(command , MAX_COMMAND_LEN, stdin);
		delete_enter(command);
		if(strcmp(command,"--exit") == 0)
		{
			break;
		}
		else
		{
			printf("\n[DATA_DEMO]Enter --exit to  deactive data call and exit data demo\n");
		}
	}

	//4、去激活
	//去激活时有效入参仅为profile_idx
	ret = qser_data_call_stop(apn_normal_datacall.profile_idx, apn_normal_datacall.ip_family, &err);
	if(ret < 0)
	{
		printf("\nERROR: deactive data call fail!!!\n");
		return -1;
	}
	ret = qser_data_call_stop(apn_test1_datacall.profile_idx, apn_test1_datacall.ip_family, &err);
	if(ret < 0)
	{
		printf("\nERROR: deactive data call fail!!!\n");
		return -1;
	}
	return 0;
}
void evt_cb(qser_data_call_state_s *state)
{
    char buf_ip[64] = {0};
    char buf_gateway[64] = {0};
    char buf_pri_dns[64] = {0};
    char buf_sec_dns[64] = {0};
    printf("DATA_DEMO_CALL_BACK: profile_idx=%d, name=%s, ip_family=%d, state=%d, error=%d\n"
        , state->profile_idx, state->name, state->ip_family, state->state, state->err);
    #if 1
    printf("DATA_DEMO_CALL_BACK: v4.ip=%s\n"
        , inet_ntoa(state->v4.ip));
    printf("DATA_DEMO_CALL_BACK: v4.gateway=%s\n"
        , inet_ntoa(state->v4.gateway));
    printf("DATA_DEMO_CALL_BACK: v4.pri_dns=%s\n"
        , inet_ntoa(state->v4.pri_dns));
    printf("DATA_DEMO_CALL_BACK: v4.sec_dns=%s\n"
        , inet_ntoa(state->v4.sec_dns));
    inet_ntop(AF_INET6, &(state->v6.ip), buf_ip, sizeof(buf_ip));
    inet_ntop(AF_INET6, &(state->v6.gateway), buf_gateway, sizeof(buf_gateway));
    inet_ntop(AF_INET6, &(state->v6.pri_dns), buf_pri_dns, sizeof(buf_pri_dns));
    inet_ntop(AF_INET6, &(state->v6.sec_dns), buf_sec_dns, sizeof(buf_sec_dns));
    printf("DATA_DEMO_CALL_BACK: v6.ip=%s\n", buf_ip);
    printf("DATA_DEMO_CALL_BACK: v6.gateway=%s\n", buf_gateway);
    printf("DATA_DEMO_CALL_BACK: v6.pri_dns=%s\n", buf_pri_dns);
    printf("DATA_DEMO_CALL_BACK: v6.sec_dns=%s\n", buf_sec_dns);
    #endif
	//后续对回调函数中返回值的处理建议放在其它线程中进行，避免阻塞在回调函数中影响后续消息上报
}
