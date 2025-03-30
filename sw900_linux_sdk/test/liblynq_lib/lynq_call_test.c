#include "lynq-qser-voice.h"

#include "mbtk_type.h"
#include "mbtk_info_api.h"
#include "mbtk_audio.h"

#define MAX_LEN 15

#if 0
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdint.h>

//#include"lynq-qser-voice-demo.h"

typedef struct
{
    int  cmdIdx;
    char *funcName;
} st_api_test_case;
    
//for server test
st_api_test_case at_api_testcases[] = 
{
    {0,   "print_help"},
    {1,   "qser_voice_call_start"},
    {2,   "qser_voice_call_end"},
    {3,   "qser_voice_call_answer"},
    {4,   "qser_voice_set_speech_volume"},
    {5,   "qser_voice_get_speech_volume"},
    {-1,    NULL}
};



typedef uint32_t voice_client_handle_type;

static pthread_t s_lynq_voice_tid = -1;


int (*qser_voice_call_client_init)(voice_client_handle_type *ph_voice);
int (*qser_voice_call_client_deinit)(voice_client_handle_type );
int (*qser_voice_call_addstatehandler)(voice_client_handle_type  h_voice,
                                       QSER_VoiceCall_StateHandlerFunc_t  handlerPtr,
                                       void *contextPtr);

int (*qser_voice_call_removestatehandle)(voice_client_handle_type );
int (*qser_voice_call_start)(voice_client_handle_type  h_voice,
                             E_QSER_VCALL_ID_T   simId, 
                             char *phone_number, int *call_id); 

int (*qser_voice_call_end)(voice_client_handle_type ,int );
int (*qser_voice_call_answer)(voice_client_handle_type ,int  );
int (*qser_voice_set_speech_volume)(const int volume);
int (*qser_voice_get_speech_volume)(int *volume);

void *dlHandle_call = NULL;

static void yk_voice_call_cb_func(int                     call_id,
                                  char*                   phone_num,
                                  qser_voice_call_state_t state,
                                  void                    *contextPtr)
{
    char *call_state[] = {"INCOMING", "DIALING", "ALERTING", "ACTIVE", "HOLDING", "END", "WAITING"};

    printf("######### Call id=%d, PhoneNum:%s, event=%s!  ######\n", call_id, phone_num, call_state[state]);
}

void print_help(void)
{
    int i;
    printf("Supported test cases:\n");
    for(i = 0; ; i++)
    {
        if(at_api_testcases[i].cmdIdx == -1)
        {
            break;
        }
        printf("%d:\t%s\n", at_api_testcases[i].cmdIdx, at_api_testcases[i].funcName);
    }
}

int main(int argc, char const *argv[])
{
    int cmdIdx = 0;
    int ret    = 0;
    int  voice_call_id = 0;
    voice_client_handle_type    h_voice     = 0;
    
    //const char *lynqLibPath_Call = "/lib/liblynq-qser-voice.so";
    const char *lynqLibPath_Call = "/lib/liblynq_lib.so";
    dlHandle_call = dlopen(lynqLibPath_Call, RTLD_NOW);
    if (dlHandle_call == NULL) 
    {
        printf("dlopen dlHandle_call failed: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    
    qser_voice_call_client_init = (int(*)(voice_client_handle_type *ph_voice))dlsym(dlHandle_call, "qser_voice_call_client_init");
    if(qser_voice_call_client_init == NULL) 
    {
        printf("qser_voice_call_client_init not defined or exported in %s\n", lynqLibPath_Call);
        return -1;
    }

    qser_voice_call_addstatehandler = (int(*)(voice_client_handle_type          h_voice,
                                            QSER_VoiceCall_StateHandlerFunc_t    handlerPtr,
                                            void  *contextPtr))dlsym(dlHandle_call,"qser_voice_call_addstatehandler");
    if(qser_voice_call_addstatehandler == NULL) 
    {
        printf("qser_voice_call_addstatehandler not defined or exported in %s\n", lynqLibPath_Call);
        return -1;
    }
    
    qser_voice_call_answer = (int(*)(voice_client_handle_type,int ))dlsym(dlHandle_call,"qser_voice_call_answer");
    if(qser_voice_call_answer == NULL) 
    {
        printf("qser_voice_call_answer not defined or exported in %s\n", lynqLibPath_Call);
        return -1;
    }

    qser_voice_call_start = (int(*)(voice_client_handle_type  h_voice,E_QSER_VCALL_ID_T   simId, 
                                    char *phone_number, int *call_id))dlsym(dlHandle_call,"qser_voice_call_start");
    if(qser_voice_call_start == NULL) 
    {
        printf("qser_voice_call_start not defined or exported in %s\n", lynqLibPath_Call);
        return -1;
    }

    qser_voice_call_end = (int(*)(voice_client_handle_type ,int))dlsym(dlHandle_call,"qser_voice_call_end");
    if(qser_voice_call_end == NULL) 
    {
        printf("qser_voice_call_end not defined or exported in %s\n", lynqLibPath_Call);
        return -1;
    }
    

    qser_voice_call_client_deinit = (int (*)(voice_client_handle_type h_voice))dlsym(dlHandle_call,"qser_voice_call_client_deinit");
    if(qser_voice_call_client_deinit == NULL) 
    {
        printf("qser_voice_call_client_deinit  not defined or exported in %s\n", lynqLibPath_Call);
        return -1;
    }

    qser_voice_call_removestatehandle = (int (*)(voice_client_handle_type))dlsym(dlHandle_call,"qser_voice_call_removestatehandle");
    if(qser_voice_call_removestatehandle == NULL) 
    {
        printf("qser_voice_call_removestatehandle not defined or exported in %s\n", lynqLibPath_Call);
        return -1;
    }

    qser_voice_set_speech_volume = (int (*)(const int ))dlsym(dlHandle_call,"qser_voice_set_speech_volume");
    if(qser_voice_set_speech_volume == NULL)    
    {
            printf("qser_voice_set_speech_volume not defined or exported in %s\n", lynqLibPath_Call);
            return -1;
    }

    qser_voice_get_speech_volume = (int (*)(int* ))dlsym(dlHandle_call,"qser_voice_get_speech_volume");
    if(qser_voice_get_speech_volume == NULL)    
    {
            printf("qser_voice_get_speech_volume not defined or exported in %s\n", lynqLibPath_Call);
            return -1;
    }
    
    ret = qser_voice_call_client_init(&h_voice);
    if(ret != 0 )
    {
        printf("qser_voice_call_client_init FAIL\n");
        return -1;
    }

    ret = qser_voice_call_addstatehandler(h_voice, yk_voice_call_cb_func, &voice_call_id);
    if(ret != 0)
    {
        printf("qser_voice_call_addstatehandler FAIL\n");
        return -1;
    }


    print_help();
    while(1)
    {
        printf("\nplease input cmd index(-1 exit): ");
        scanf("%d", &cmdIdx);
        if(cmdIdx == -1)
        {
            break;
        }

        switch(cmdIdx)
        {
            //"print_help
            case 0:
                print_help();
                break;

            //"qser_voice_call_start"
            case 1:
            {
                char    PhoneNum[32] = {0}; 

                printf("please input dest phone number: \n");
                scanf("%s", PhoneNum);
            
                ret = qser_voice_call_start(h_voice, E_QSER_VCALL_EXTERNAL_SLOT_1, PhoneNum, &voice_call_id);
                printf("qser_voice_call_start ret = %d, with voice_call_id=%d\n", ret, voice_call_id);
                break;
            }
    
            //"qser_voice_call_end"
            case 2:
            {
                int call_id = -1;
                printf("please input end call id: \n");
                scanf("%d", &call_id);
                ret = qser_voice_call_end(h_voice, call_id);
                printf(" ret = %d\n", ret);
                break;
            }

            //"qser_voice_call_answer"
            case 3:
            {
                int call_id = -1;
                printf(" please input answer call id\n");
                scanf("%d", &call_id);
                ret = qser_voice_call_answer(h_voice, call_id);
                printf(" ret = %d\n", ret);
                break;
            }
            case 4:
            {
                int volume = 0;
                printf("Please set speech volume:0-5 level\n");
                scanf("%d",&volume);
                ret = qser_voice_set_speech_volume(volume);
                printf("ret is %d\n",ret);
                break;

            }

            case 5:
            {
                int volume = -1;
                printf("Enter get speech volume\n");
                ret = qser_voice_get_speech_volume(&volume);
                printf("ret is %d,get volume is %d\n",ret,volume);
                break;

            }
            default:
                print_help();
                break;
        }
            
    }
    ret = qser_voice_call_removestatehandle(h_voice);
    if(ret != 0 && ret != 1)
    {
        printf("qser_voice_call_removestatehandle FAIL!!!\n");
        return -1;
    }
    printf("qser_voice_call_removestatehandle ret = %d\n", ret);
    
    
    ret = qser_voice_call_client_deinit(h_voice);
    if(ret != 0)
    {
        printf("qser_voice_call_client_deinit FAIL\n");
        return -1;
    }
    printf("qser_voice_call_client_deinit ret = %d, with h_voice=%d\n", ret, h_voice);
    
   return 0;
}


#else

static void yk_voice_call_cb_func(int                     call_id,
                                  char*                   phone_num,
                                  qser_voice_call_state_t state,
                                  void                    *contextPtr)
{
    char *call_state[] = {"INCOMING", "DIALING", "ALERTING", "ACTIVE", "HOLDING", "END", "WAITING"};

    printf("######### Call id=%d, PhoneNum:%s, event=%s!  ######\n", call_id, phone_num, call_state[state]);
}

int main(int argc, char *argv[])
{
    char operator[MAX_LEN];
    int opt;
    int lv_voll = 0;
    voice_client_handle_type handle = -1;
    int  voice_call_id = 0;
    mbtk_log_init("radio", "CALL_TEST");
    
    printf("=========call main=========\n"
        "\t0 exit\n"
        "\t1 call init\n"
        "\t2 call add register handle\n"
        "\t3 call start\n"
        "\t4 call end\n"
        "\t5 call answer\n"
        "\t6 call remove register handle\n"
        "\t7 call set volume level 0-5\n"
        "\t8 call get volume\n"
        "\t9 call dtmf\n"
        "\t10 call deinit\n"
        "operator: >> \n");
    
    while(1)
    {
        fgets(operator, sizeof(operator), stdin);
        fflush(stdin);
        opt = atoi(operator);
        switch (opt)
        {
        case 0:
            printf("main exit\n");
            return 0;
        case 1:
            qser_voice_call_client_init(&handle);
            printf("test>>: handle = %d\n",handle);
            break;
        case 2:
            qser_voice_call_addstatehandler(handle, yk_voice_call_cb_func, &voice_call_id);
            break;
        case 3:
            printf("input phone number:\n");
            memset(operator, 0x0, MAX_LEN);
            fgets(operator, MAX_LEN, stdin);
            fflush(stdin);
            printf("number:%s\n", operator);
            qser_voice_call_start(handle, E_QSER_VCALL_EXTERNAL_SLOT_1, operator, &voice_call_id);//被叫电话
            break;
        case 4:
            qser_voice_call_end(handle, voice_call_id);
            break;
        case 5:
            qser_voice_call_answer(handle, voice_call_id);
            break;
        case 6:
            qser_voice_call_removestatehandle(handle);
            break;
        case 7:
            {
                printf("Pleas set volume level(0-5)>>>>\n");
                memset(operator, 0x00, sizeof(operator));
                fgets(operator, sizeof(operator), stdin);
                fflush(stdin);
                //def level 3
                lv_voll = atoi(operator);
                if (lv_voll >= 0 && lv_voll <= 5)
                    qser_voice_set_speech_volume(lv_voll);
                else
                    printf("set volume level(0-5) ERR\n");
            }
            break;
        case 8:
            qser_voice_get_speech_volume(&lv_voll);
            printf("volume level = %d\n",lv_voll);
            break;
        case 9:
            {
                char inputChar;
                
                printf("Enter set dtmf\n");
                scanf(" %c", &inputChar);
                printf("inputChar is %c\n", inputChar);
                
                if (qser_voice_set_dtmf(inputChar) != 0) 
                {
                    printf("qser set voice dtmf failed\n");
                }
            }
            break;
        case 10:
            qser_voice_call_client_deinit(handle);
            break;
        default:
            break;
        }

    }

    return 0;
}

#endif

