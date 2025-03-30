/*
* mbtk_device.h
*
* MBTK device_info partition data header.
*
* Author : lb
* Date   : 2023/12/30 11:36:35
*/
#ifndef _MBTK_DEVICE_H
#define _MBTK_DEVICE_H
//#include "mbtk_type.h"

#define MBTK_DEVICE_INFO_PARTITION_NAME "device_info"
#define MBTK_DEVICE_INFO_PARTITION_TAG 0x87878787
#define MBTK_DEVICE_INFO_CURR_VERSION 1

#define MBTK_DEV_INFO_PARTITION_ADDR 0x02A20000

// 78,147,482,134742231
#define MBTK_BAND_ALL_GSM_DEFAULT 78             // GSM : ASR default.
#define MBTK_BAND_ALL_WCDMA_DEFAULT 147          // WCDMA : ASR default.
#define MBTK_BAND_ALL_TDLTE_DEFAULT 482          // TDD-LTE : ASR default.
#define MBTK_BAND_ALL_FDDLTE_DEFAULT 134742231   // FDD-LTE : ASR default.
#define MBTK_BAND_ALL_EXT_LTE_DEFAULT 0          // EXT-LTE
#define MBTK_BAND_CN_GSM_DEFAULT 11             // GSM : B3/B8 (PGSM 900/DCS GSM 1800/EGSM 900)
#define MBTK_BAND_CN_WCDMA_DEFAULT 145          // WCDMA : B1/B5/B8
#define MBTK_BAND_CN_TDLTE_DEFAULT 482          // TDD-LTE : B34/B38/B39/B40/B41
#define MBTK_BAND_CN_FDDLTE_DEFAULT 149         // FDD-LTE : B1/B3/B5/B8
#define MBTK_BAND_CN_EXT_LTE_DEFAULT 0          // EXT-LTE
#define MBTK_BAND_EU_GSM_DEFAULT 11             // GSM : B3/B8 (PGSM 900/DCS GSM 1800/EGSM 900)
#define MBTK_BAND_EU_WCDMA_DEFAULT 145          // WCDMA : B1/B5/B8
#define MBTK_BAND_EU_TDLTE_DEFAULT 416          // TDD-LTE : B38/B40/B41
#define MBTK_BAND_EU_FDDLTE_DEFAULT 134742229   // FDD-LTE : B1/B3/B5/B7/B8/B20/B28
#define MBTK_BAND_EU_EXT_LTE_DEFAULT 0          // EXT-LTE
#define MBTK_BAND_SA_GSM_DEFAULT 79             // GSM : B2/B3/B5/B8(GSM 850/PGSM 900/EGSM 900/DCS GSM 1800/PCS GSM 1900)
#define MBTK_BAND_SA_WCDMA_DEFAULT 155          // WCDMA : B1/B2/B4/B5/B8
#define MBTK_BAND_SA_TDLTE_DEFAULT 128          // TDD-LTE : B40
#define MBTK_BAND_SA_FDDLTE_DEFAULT 134217951  // FDD-LTE : B1/B2/B3/B4/B5/B7/B8/B28
#define MBTK_BAND_SA_EXT_LTE_DEFAULT 2          // EXT-LTE : B66

#define MBTK_DEVICE_INFO_ITEM_STR_BASIC "BASIC"
#define MBTK_DEVICE_INFO_ITEM_STR_FOTA "FOTA"
#define MBTK_DEVICE_INFO_ITEM_STR_MODEM "MODEM"
#define MBTK_DEVICE_INFO_ITEM_STR_LOG "LOG"

typedef enum {
    MBTK_DEVICE_INFO_ITEM_BASIC = 0,
    MBTK_DEVICE_INFO_ITEM_FOTA,
    MBTK_DEVICE_INFO_ITEM_MODEM,
    MBTK_DEVICE_INFO_ITEM_LOG,

    MBTK_DEVICE_INFO_ITEM_NUM
} mbtk_device_info_item_enum;

typedef struct {
    mbtk_device_info_item_enum item;
    uint32 addr;
} mbtk_device_info_item_header_t;

typedef struct {
    uint32 tag;
    uint32 version;             // Default : 0x01
    uint32 item_count;
    mbtk_device_info_item_header_t item_header[MBTK_DEVICE_INFO_ITEM_NUM];
} mbtk_device_info_header_t;

typedef enum {
    MBTK_REBOOT_FLAG_NORMAL = 0,
    MBTK_REBOOT_FLAG_DOWNLOAD
} mbtk_device_info_reboot_flag_enum;

typedef struct {
    uint8 name[16];
    uint32 version;             // Default : 0x01
    uint8 project[16];          // T108 / L508_X6
    uint8 project_cust[16];     // T108_C1 / L508_X6_C1  (Refer to: Custom_Model in blf file.)
    uint32 ab_support;          // 1 for ab
    mbtk_device_info_reboot_flag_enum reboot_flag;
    uint8 revision_out[48];     // L508_X6v01.01b04.00
    uint8 revision_in[64];
    uint8 build_time[64];
} mbtk_device_info_basic_t;

typedef struct {
    uint8 name[16];
    uint32 version;             // Default : 0x01
    uint32 state;               //
} mbtk_device_info_fota_t;

typedef enum {
    MBTK_MODEM_BAND_AREA_ALL,
    MBTK_MODEM_BAND_AREA_CN,
    MBTK_MODEM_BAND_AREA_EU,
    MBTK_MODEM_BAND_AREA_SA
} mbtk_modem_band_area_enum;

typedef struct {
    uint8 name[16];
    uint32 version;             // Default : 0x01
    mbtk_modem_band_area_enum band_area;
    uint32 band_gsm;
    uint32 band_wcdma;
    uint32 band_tdlte;
    uint32 band_fddlte;
    uint32 band_lte_ext;
} mbtk_device_info_modem_t;

typedef struct {
    uint8 name[16];
    uint32 version;             // Default : 0x01
    uint32 state;               //
} mbtk_device_info_log_t;


/*
* MBTK_DEVICE_INFO_ITEM_BASIC - mbtk_device_info_basic_t
* MBTK_DEVICE_INFO_ITEM_FOTA - mbtk_device_info_fota_t
* MBTK_DEVICE_INFO_ITEM_MODEM - mbtk_device_info_modem_t
* MBTK_DEVICE_INFO_ITEM_LOG - mbtk_device_info_log_t
*/
int mbtk_dev_info_read(mbtk_device_info_item_enum item_type, void *item_ptr, int item_size);
int mbtk_dev_info_write(mbtk_device_info_item_enum item_type, void *item_ptr, int item_size);

#endif /* _MBTK_DEVICE_H */
