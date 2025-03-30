#!/bin/sh

STA_NET_CONF=/etc/wifi/sta_network.conf

function help()
{
	echo "mbtk_wifi.sh ap/sta start/stop"
}

function wifi_check()
{
	if [ ! -f /etc/wifi/aic8800_bsp.ko ];then
		echo "No found file: /etc/wifi/aic8800_bsp.ko"
		exit 1
	fi
	
	if [ ! -f /etc/wifi/aic8800_fdrv.ko ];then
		echo "No found file: /etc/wifi/aic8800_fdrv.ko"
		exit 1
	fi
	
	if [ ! -f /etc/wifi/hostapd.conf ];then
		echo "No found file: /etc/wifi/hostapd.conf"
		exit 1
	fi
	
	if [ ! -f /etc/wifi/wpa_supplicant.conf ];then
		echo "No found file: /etc/wifi/wpa_supplicant.conf"
		exit 1
	fi
	
	if [ ! -f $STA_NET_CONF ];then
		echo "No found file: $STA_NET_CONF"
		exit 1
	fi
}

function driver_insmod()
{
	cfg80211_insmod=`lsmod | grep cfg80211`
	if [ -z "$cfg80211_insmod" ];then
		modprobe cfg80211
	else
		echo "cfg80211 has insmod."
	fi
	
	aic8800_bsp_insmod=`lsmod | grep aic8800_bsp`
	if [ -z "$aic8800_bsp_insmod" ];then
		insmod /etc/wifi/aic8800_bsp.ko sdio_f0=0 sdio_clk=150000000
	else
		echo "aic8800_bsp has insmod."
	fi
	
	aic8800_fdrv_insmod=`lsmod | grep aic8800_fdrv`
	if [ -z "$aic8800_fdrv_insmod" ];then
		insmod /etc/wifi/aic8800_fdrv.ko
	else
		echo "aic8800_fdrv has insmod."
	fi
}

function driver_rmmod()
{
	aic8800_fdrv_insmod=`lsmod | grep aic8800_fdrv`
	if [ -z "$aic8800_fdrv_insmod" ];then
		echo "aic8800_fdrv not insmod."
	else
		rmmod aic8800_fdrv
	fi
	
	aic8800_bsp_insmod=`lsmod | grep aic8800_bsp`
	if [ -z "$aic8800_bsp_insmod" ];then
		echo "aic8800_bsp not insmod."
	else
		rmmod aic8800_bsp
	fi
	
	cfg80211_insmod=`lsmod | grep cfg80211`
	if [ -z "$cfg80211_insmod" ];then
		echo "cfg80211 not insmod."
	else
		rmmod cfg80211
	fi
}

function wifi_ap_start()
{
	hostapd /etc/wifi/hostapd.conf -B
	sleep 5s
	
	brctl addif br-lan wlan0
	iptables -t nat -A POSTROUTING -o ccinet0 -j MASQUERADE --random
}

function wifi_ap_stop()
{
	killall hostapd
}

function wifi_sta_start()
{
	wpa_supplicant -Dnl80211 -iwlan0 -c/etc/wifi/wpa_supplicant.conf -B
	sleep 5s
	
	SSID=`cat $STA_NET_CONF | grep -w 'SSID'   | cut -d '=' -f 2`
	PASSWORD=`cat $STA_NET_CONF | grep -w 'PASSWORD'   | cut -d '=' -f 2`
	prefix=\"
	subfix=\"
	str1=$prefix$SSID$subfix
	str2=$prefix$PASSWORD$subfix
	
	echo "SSID = $str1"
	echo "PSK = $str2"

	wpa_cli -p/var/run/wpa_supplicant remove_network 0
	wpa_cli -p/var/run/wpa_supplicant ap_scan 1
	wpa_cli -p/var/run/wpa_supplicant add_network
	wpa_cli -p/var/run/wpa_supplicant set_network 0 ssid $str1
	wpa_cli -p/var/run/wpa_supplicant set_network 0 key_mgmt WPA-PSK
	wpa_cli -p/var/run/wpa_supplicant set_network 0 psk $str2
	wpa_cli -p/var/run/wpa_supplicant select_network 0

	udhcpc -i wlan0
}

function wifi_sta_stop()
{
	killall wpa_supplicant
}

wifi_check

if [ -n "$2" ] ;then	#有参数
	case "$1" in
		ap)
			case "$2" in
				start)
					driver_insmod
					
					wifi_ap_start
					;;
				stop)
					wifi_ap_stop
					;;
				*)
					help
					;;
			esac
			;;
		sta)
			case "$2" in
				start)
					driver_insmod
					
					wifi_sta_start
					;;
				stop)
					wifi_sta_stop
					;;
				*)
					help
					;;
			esac
			;;
		*)
            help
			;;
	esac
else
	help
fi