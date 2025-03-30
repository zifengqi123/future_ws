#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <syslog.h>
#include <pthread.h>

#include <lynq/lynq-qser-wifi.h>

int main(int argc, char *argv[])
{
	if (argc == 2 )
    {
        if(strcmp(argv[1], "0") == 0){
            qser_wifi_enable(); //enable wifi
            qser_wifi_ap_stop(LYNQ_WIFI_AP_INDEX_AP0); //Disable ap mode for wlan0
            qser_wifi_disable(); //Turn off WiFi
        }
        else if(strcmp(argv[1], "1") == 0){
            char pw[65] = {0};
            sprintf(pw, "lynq123456");
            qser_wifi_enable(); //enable wifi
            qser_wifi_ap_ssid_set(LYNQ_WIFI_AP_INDEX_AP0,"hello"); //Set the ssid of wlan0 to hello
            qser_wifi_ap_max_sta_set(LYNQ_WIFI_AP_INDEX_AP0,22); //Example Set the maximum number of connections to 22
            qser_wifi_ap_channel_set(LYNQ_WIFI_AP_INDEX_AP0,"CN",13); //Set the country code to CN and channel to 13
            qser_wifi_ap_mode_set(LYNQ_WIFI_AP_INDEX_AP0,LYNQ_WIFI_MODE_80211BGN); //Set the working protocol mode of wlan0 to 80211BGN
            qser_wifi_ap_auth_set(LYNQ_WIFI_AP_INDEX_AP0,LYNQ_WIFI_AUTH_WPA2_PSK,pw); //Set the authentication of wlan0 to wpa2 and the password to lynq123456
            qser_wifi_ap_start(LYNQ_WIFI_AP_INDEX_AP0); //Set the ap mode of wlan0
        }
        else if(strcmp(argv[1], "2") == 0){
            char pw[65] = {0};
            char ssid[33] = {0};
            char country_code[12] = {0};
            int channel, max_sta_num;
            lynq_wifi_mode_type_e mode;
            lynq_wifi_auth_e auth_mode;
            lynq_wifi_bandwidth_type_e bandwidth;
            int ret = -1;
            qser_wifi_enable(); //enable wifi
            ret = qser_wifi_ap_ssid_get(LYNQ_WIFI_AP_INDEX_AP0,ssid); //Gets the ssid of wlan0
            printf("[lynq-wifi-demo]  ssid=%s ret = %d\n",ssid, ret);
            ret = qser_wifi_ap_max_sta_get(LYNQ_WIFI_AP_INDEX_AP0,&max_sta_num); //Gets the maximum sta number for wlan0
            printf("[lynq-wifi-demo]  max_sta_num = %d ret = %d\n",max_sta_num, ret);
            ret = qser_wifi_ap_mode_get(LYNQ_WIFI_AP_INDEX_AP0,&mode); //Gets the working protocol mode for wlan0
            printf("[lynq-wifi-demo]  mode = %d ret = %d\n",mode, ret);
            ret = qser_wifi_ap_auth_get(LYNQ_WIFI_AP_INDEX_AP0,&auth_mode,pw); //Get wlan0's password security authentication and password
            printf("[lynq-wifi-demo]  pw = %s auth_mode = %d ret = %d\n",pw, auth_mode, ret);
            ret = qser_wifi_ap_channel_get(LYNQ_WIFI_AP_INDEX_AP0,country_code,&channel); //Get the country code and channel of wlan0
            printf("[lynq-wifi-demo]  country_code = %s channel = %d ret = %d\n",country_code, channel, ret);
            ret = qser_wifi_ap_bandwidth_get(LYNQ_WIFI_AP_INDEX_AP0,&bandwidth); //Gets the bandwidth of wlan0
            printf("[lynq-wifi-demo]  bandwidth = %d ret = %d\n",bandwidth, ret);
        }
        else{
            printf("  [lynq-wifi-demo]Parameter error, please re-enter\n");
        }
    }
    else
    {
        printf("[lynq-wifi-demo]Please enter one of the parameters 0,1,2\n");
        printf("  [lynq-wifi-demo]2: View information such as ssid, password, and channel in ap mode\n");
        printf("  [lynq-wifi-demo]1: initializes wifi and enables and sets ap mode\n");
        printf("  [lynq-wifi-demo]0: indicates off.\n");
        return 0;
    }

	return 0;
}

