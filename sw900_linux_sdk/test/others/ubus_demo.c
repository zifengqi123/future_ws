#include <unistd.h>

#include <libubox/blobmsg_json.h>
#include "libubus.h"
#include "mbtk_audio.h"

void receive_call_result_data(struct ubus_request *req, int type, struct blob_attr *msg)
{
    printf("receive_call_result_data()\n");
	if (!msg)
		return;

	printf("len - %d,data - %s\n", msg->id_len, msg->data);
}


int main(int argc, char *argv[])
{
#if 1
    if(argc != 3) {
        printf("ubus_demo switch/mode 0/1/-2/2\n");
        return -1;
    }
    char *type = NULL;
    int value = atoi(argv[2]);
    if(!strcmp(argv[1], "switch")) {
        type = "switch_pcm";
        if(value != 0 && value != 1) {
            printf("ubus_demo switch/mode 0/1/-2/2\n");
            return -1;
        }
    } else if(!strcmp(argv[1], "mode")) {
        type = "audio_mode_set";
        if(value != -2 && value != 2) {
            printf("ubus_demo switch/mode 0/1/-2/2\n");
            return -1;
        }
    } else {
        printf("ubus_demo switch/mode 0/1/-2/2\n");
        return -1;
    }

    static struct ubus_context *ctx;
    ctx = ubus_connect(NULL);
	if (!ctx) {
		printf("Failed to connect to ubus\n");
		return -1;
	}

    static struct blob_buf b;
    uint32_t id;
	int ret;
    ret = ubus_lookup_id(ctx, "audio_if", &id);
	if (ret) {
        printf("ubus_lookup_id() fail.\n");
		return ret;
	}



    blob_buf_init(&b, 0);
    blobmsg_add_u32(&b, "param0", value);
	if((ret = ubus_invoke(ctx, id, type, b.head, NULL/*receive_call_result_data*/, NULL, 0)) != UBUS_STATUS_OK) {
        printf("ubus_invoke fail:%d.\n", ret);
	} else {
        printf("ubus_invoke success.\n");
	}
#else

    int handler = 0;
    mbtk_audio_ubus_client_init(&handler, NULL);

    mbtk_audio_switch_pcm(1);
    mbtk_audio_mode_set(2);

#endif

    return 0;
}
