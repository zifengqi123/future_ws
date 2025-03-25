#ifndef __FUTURE_DEF_H__
#define __FUTURE_DEF_H__

/**
 * @brief SDK返回值定义
 */
#define SDK_OK                              (0)     /**< 成功 */
#define SDK_ERROR                           (-1)    /**< 失败 */
#define SDK_PARAMERR                        (-2)    /**< 参数错误 */
#define SDK_ESC                             (-120)  /**< 取消退出 */
#define SDK_TIMEOUT                         (-121)  /**< 超时 */

/**
 * @brief 磁条卡错误码定义
 */ 
#define ERROR_MAGNETIC_OTHER                (-1101)  // 失败
#define ERROR_MAGNETIC_PARAMERR             (-1102)  // 参数错误
#define ERROR_MAGNETIC_NOCARD               (-1103)  // 未刷卡

/**
 * @brief IC卡错误码定义
 */
#define ERROR_IC_OTHER                      (-1201)  // 失败
#define ERROR_IC_PARAMERR                   (-1202)  // 参数错误
#define ERROR_IC_NOCARD                     (-1203)  // 卡不在座子上
#define ERROR_IC_NOPOWER                    (-1204)  // 卡未上电
#define ERROR_IC_TIMEOUT                    (-1205)  // 超时错误,或无响应(L1 TIME OUT ERROR)
#define ERROR_IC_PROTOCOL                   (-1206)  // 协议错误（L1 PROTOCOL ERROR）
#define ERROR_IC_TRANSFER                   (-1207)  // 传输错误

/**
 * @brief 非接卡错误码定义
 */
#define ERROR_ICC_OTHER                      (-1301)  // 失败
#define ERROR_ICC_PARAMERR                   (-1302)  // 参数错误
#define ERROR_ICC_NOCARD                     (-1303)  // 未搜寻到卡片
#define ERROR_ICC_MULTICARD         (-1304)  // 感应区内卡片过多（多卡冲突）
#define ERROR_ICC_TIMEOUT           (-1305)  // 超时错误(L1 TIME OUT ERROR)
#define ERROR_ICC_PROTOCOL          (-1306)  // 协议错误(L1 PROTOCOL ERROR)
#define ERROR_ICC_TRANSFER          (-1307)  // 传输错误,任何其他错误

/**
 * @brief 打印机错误码定义
 */
#define ERROR_PRINTER_OTHER             (-1401)  // 失败， 或内存不足
#define ERROR_PRINTER_PARAMERR          (-1402)  // 参数错误
#define ERROR_PRINTER_PAPER_LOW         (-1403)  // 打印机缺纸
#define ERROR_PRINTER_TEMP_HIGH         (-1404)  // 温度过高
#define ERROR_PRINTER_DEVICE_FAULT      (-1405)  // 打印机设备故障

/**
 * @brief 密码键盘错误码定义
 */
#define ERROR_KEYPAD_OTHER             (-1501)  // 其它错误
#define ERROR_KEYPAD_PARAMERR          (-1502)  // 参数错误
#define ERROR_KEYPAD_CONFIRM           (-1503)  // 持卡人直接按确认（无密码）
#define ERROR_KEYPAD_CANCEL            (-1504)  // 用户取消输入PIN
#define ERROR_KEYPAD_TIMEOUT           (-1505)  // 超时退出
#define ERROR_KEYPAD_PINOVERLIMIT      (-1506)  // 输入 PIN 一小时不能超过 120 次
#define ERROR_KEYPAD_KEYREPEAT         (-1507)  // 密钥值重复
#define ERROR_KEYPAD_INVALIDKEYINDEX   (-1508)  // 无效密钥索引
#define ERROR_KEYPAD_NOKEY             (-1509)  // 未设置密钥
#define ERROR_KEYPAD_MAINKEYERR        (-1510)  // 主密钥校验错
#define ERROR_KEYPAD_PINKEYERR         (-1511)  // PIN密钥校验错
#define ERROR_KEYPAD_TDKEYERR          (-1512)  // TD密钥校验错
#define ERROR_KEYPAD_MACKEYERR         (-1513)  // MAC密钥校验错
#define ERROR_KEYPAD_INITFAIL           (-1520)  // 密码键盘初始化失败
#define ERROR_KEYPAD_RANDOMNUMFAIL      (-1521)  // 获取随机数失败
#define ERROR_KEYPAD_TERMINAL_DESTROYED (-1522)  // 终端已自毁
#define ERROR_KEYPAD_KEYTRIGGERDISABLE  (-1523)  // 密钥触发禁用





/**
 * @brief 终端信息TAG
 */
#define INFO_MERCHANT_NAME      (0xA0) // 平台名称
#define INFO_MNT_VER            (0xA1) // mnt版本
#define INFO_HARDWARE_VER       (0xA2) // 硬件版本号(参数文件控制)
#define INFO_BOOT_VER           (0xA3) // BOOT版本号
#define INFO_APP_VER            (0xA4) // 应用版本
#define INFO_SN                 (0xA5) // SN
#define INFO_DATE               (0xA6) // 出厂日期
#define INFO_GSM_VER            (0xA7) // GPRS模块版本
#define INFO_SDK_VER            (0xA8) // SDK VER
#define INFO_IMEI               (0xA9) // IMEI
#define INFO_ICCID              (0xAA) // ICCID
#define INFO_IMSI               (0xAB) // IMSI
#define INFO_TERM_TYPE          (0xAC) // 终端类型
#define INFO_OPTION             (0xAD) // 硬件选配信息(TP/SCAN/WIFI)
#define INFO_EFLASH             (0xAE) // 外部FLAHS型号
#define INFO_CSN                (0xAF) // 客户CSN号
#define INFO_LCD                (0xB0)  // LCD类型
#define INFO_RES                (0xB1)  // 资源名称



/**
 * @brief 按键值定义, 中文注释
 */
#define KEY_0                   0x30            /**< 0 */
#define KEY_1                   0x31            /**< 1 */
#define KEY_2                   0x32            /**< 2 */
#define KEY_3                   0x33            /**< 3 */
#define KEY_4                   0x34            /**< 4 */
#define KEY_5                   0x35            /**< 5 */
#define KEY_6                   0x36            /**< 6 */
#define KEY_7                   0x37            /**< 7 */       
#define KEY_8                   0x38            /**< 8 */
#define KEY_9                   0x39            /**< 9 */
#define KEY_LETTER              0x07            /**< 字母 */
#define KEY_BACKSPACE           0x08            /**< 退格 */
#define KEY_CONFIRM             0x0D            /**< 确认 */
#define KEY_LEFT                0x12            /**< 向左 */
#define KEY_RIGHT               0x13            /**< 向右 */
#define KEY_MENU                0x14            /**< 菜单 */
#define KEY_FUNCTION            0x15            /**< 功能 */
#define KEY_PRINT_UP            0x19            /**< 打印向上走纸 */
#define KEY_CANCEL              0x1B            /**< 取消 */
#define KEY_UP                  0x26            /**< 向上 */
#define KEY_DOWN                0x28            /**< 向下 */
#define KEY_STAR                0x2A            /**< '*'键 */
#define KEY_CLEAR               0x2E            /**< 清除 */
#define KEY_INVALID             0xFF            /**< 无效按键 */
#define KEY_TIMEOUT             0x00            /**< 超时无按键 */
#define KEY_OTHER               0x00            /**< 其他 */

/**
 * @brief 命令字定义
 */
#define CMD_SET_COMM_MODE                           0x11    /**< 设置通讯方式 */
#define CMD_GET_NETWORK_MODE                        0x12    /**< 获取网络制式 */
#define CMD_GET_SYSTEM_VERSION                      0x17    /**< 读取系统版本信息 */
#define CMD_GET_HARDWARE_SERIAL_NUMBER              0x18    /**< 获取硬件序列号 */
#define CMD_GET_DEVICE_MODEL                        0x19    /**< 获取设备型号 */
#define CMD_GET_CUSTOMER_SERIAL_NUMBER              0x1A    /**< 获取客户自定义序列号 */
#define CMD_SET_CLOCK                               0x1B    /**< 设置时钟 */
#define CMD_GET_CLOCK                               0x1C    /**< 获取时钟 */
#define CMD_BEEP                                    0x20    /**< 蜂鸣 */
#define CMD_BEEP_FREQUENCY                          0x21    /**< 蜂鸣器按指定的频率发声 */
#define CMD_LED_ON                                  0x22    /**< 点亮LED指示灯 */
#define CMD_LED_OFF                                 0x23    /**< 熄灭LED指示灯 */
#define CMD_LED_FLASH                               0x24    /**< LED指示灯闪烁 */
#define CMD_SYSTEM_RESET                            0x25    /**< 系统复位 */
#define CMD_SYSTEM_SHUTDOWN                         0x26    /**< 系统关机 */
#define CMD_SET_TERMINAL_SERIAL_NUMBER              0x28    /**< 设置终端序列号 */
#define CMD_GET_CHIP_SERIAL_NUMBER                  0x29    /**< 获取芯片序列号 */
#define CMD_ENABLE_SLEEP_MODE                       0x2A    /**< 使能下位机立即进入休眠模式 */
#define CMD_ENTER_BOOT_OR_QUERY_STATE               0x2F    /**< 下位机进入BOOT或查询状态 */
#define CMD_OPEN_KEYBOARD                           0x30    /**< 打开键盘 */
#define CMD_CLOSE_KEYBOARD                          0x31    /**< 关闭键盘 */
#define CMD_READ_KEYBOARD_INPUT                     0x32    /**< 读取键盘输入 */
#define CMD_CLEAR_KEYBOARD_CACHE                    0x33    /**< 清除按键缓存 */
#define CMD_SET_KEYBOARD_CALLBACK                   0x34    /**< 设置按键回调函数 */
#define CMD_SET_KEYBOARD_SOUND                      0x35    /**< 设置打开关闭按键音 */
#define CMD_SET_KEYBOARD_BACKLIGHT                  0x36    /**< 设置按键背光 */
#define CMD_OPEN_TP                                 0x3A    /**< 打开TP */
#define CMD_CLOSE_TP                                0x3B    /**< 关闭TP */
#define CMD_CHECK_TP_SUPPORT                        0x3C    /**< 检查是否支持TP */
#define CMD_GET_TOUCH_COORDINATE                    0x3E    /**< 获取触控坐标值 */
#define CMD_SET_TOUCH_PARAMETER                     0x3F    /**< 设置触控参数 */
#define CMD_OPEN_MAGNETIC_STRIPE_CARD               0x40    /**< 打开磁条卡设备 */
#define CMD_CLOSE_MAGNETIC_STRIPE_CARD              0x41    /**< 关闭磁条卡设备 */
#define CMD_CHECK_MAGNETIC_STRIPE_CARD              0x42    /**< 检查是否有刷磁条卡 */
#define CMD_READ_MAGNETIC_STRIPE_CARD_DATA          0x43    /**< 读取磁条卡数据 */
#define CMD_CLEAR_MAGNETIC_STRIPE_CARD_DATA         0x44    /**< 清除磁卡数据缓冲 */
#define CMD_FORMAT_MAGNETIC_STRIPE_CARD_DATA        0x45    /**< 格式化磁道信息 */
#define CMD_OPEN_IC_CARD_MODULE                     0x4A    /**< 打开IC卡模块 */
#define CMD_CLOSE_IC_CARD_MODULE                    0x4B    /**< 关闭IC卡模块 */
#define CMD_CHECK_IC_STATUS                         0x4C    /**< 检查ICC状态 */
#define CMD_IC_CARD_RESET                           0x4D    /**< IC卡上电复位 */
#define CMD_IC_CARD_MODULE_POWER_OFF                0x4E    /**< IC卡模块下电 */
#define CMD_IC_CARD_SEND_APDU_COMMAND               0x4F    /**< IC卡发送APDU命令 */
#define CMD_ICC_OPEN_MODULE                         0x50    /**< 打开非接模块 */
#define CMD_ICC_CLOSE_MODULE                        0x51    /**< 关闭非接模块 */
#define CMD_ICC_SEARCH_CARD_ACTIVATION              0x52    /**< 寻卡激活 */
#define CMD_ICC_SEND_APDU_COMMAND                   0x53    /**< 发送APDU命令 */
#define CMD_MIFARE_CARD_AUTHENTICATION              0x54    /**< Mifare卡认证 */
#define CMD_MIFARE_CARD_OPERATION                   0x55    /**< Mifare卡操作 */
#define CMD_SEARCH_CARD_START                       0x46    /**< 单独寻卡报文 */
#define CMD_SEARCH_CARD_RESULT                      0x47    /**< 发送寻卡结果报文 */
#define CMD_SEARCH_CARD_STOP                        0x48    /**< 结束寻卡 */
#define CMD_SCAN_OPEN                               0x5A    /**< 打开扫码 */
#define CMD_SCAN_CLOSE                              0x5B    /**< 关闭扫码 */
#define CMD_READ_SCAN_DATA                          0x5C    /**< 读取扫码数据 */
#define CMD_CHECK_PRINTER_SUPPORT                   0x61    /**< 检查是否支持打印机 */
#define CMD_PRINTER_OPEN                            0x62    /**< 打开打印机 */
#define CMD_PRINTER_CLOSE                           0x63    /**< 关闭打印机 */
#define CMD_QUERY_PRINTER_STATUS                    0x64    /**< 查询打印机状态 */
#define CMD_SET_PRINTER_GRAY                         0x65    /**< 设置打印灰度 */
#define CMD_PAPER_FEED                               0x6B    /**< 走纸 */
#define CMD_PRINT_BITMAP_DATA                        0x6C    /**< 打印位图数据 */
#define CMD_KEYPAD_OPEN_PASSWORD                     0x70    /**< 打开密码键盘 */
#define CMD_KEYPAD_CLOSE_PASSWORD                    0x71    /**< 关闭密码键盘 */
#define CMD_KEYPAD_GET_RANDOM_NUMBER                 0x72    /**< 获取随机数 */
#define CMD_KEYPAD_UPDATE_MASTER_KEY                 0x73    /**< 更新主密钥 */
#define CMD_KEYPAD_UPDATE_WORK_KEY                   0x74    /**< 更新工作密钥 */
#define CMD_KEYPAD_ENCRYPT_DATA                       0x75    /**< 加密数据 */
#define CMD_KEYPAD_ENCRYPT_MAGNETIC_STRIPE_DATA     0x76    /**< 加密磁道数据 */
#define CMD_KEYPAD_CALCULATE_MAC                    0x77    /**< 计算MAC */
#define CMD_KEYPAD_INPUT_ONLINE_PIN                 0x78    /**< 输入联机PIN */
#define CMD_KEYPAD_GEN_RSA_KEY_PAIR_OUTPUT_PUBLIC_KEY 0x7B   /**< 生成RSA密钥对输出公钥 */
#define CMD_KEYPAD_ENCRYPT_RSA_PRIVATE_KEY          0x7C    /**< 使用已生成RSA私钥加密 */
#define CMD_KEYPAD_ENCRYPT_HARDWARE_SERIAL_NUMBER   0x7D    /**< 硬件序列号加密 */
#define CMD_KEYPAD_CHECK_TRIGGER_STATUS             0x84    /**< 查看触发状态 */
#define CMD_KEYPAD_SET_TRIGGER_STATUS               0x85    /**< 设置触发状态 */
#define CMD_KEYPAD_RELEASE_TRIGGER                  0x86    /**< 解触发 */
#define CMD_KEYPAD_CHECK_KEY                        0x87    /**< 密钥检查 */
#define CMD_KEYPAD_SM3_HASH                         0x95    /**< SM3哈希算法 */
#define CMD_KEYPAD_DES_ENCRYPT_DECRYPT              0x96    /**< DES加解密算法 */
#define CMD_KEYPAD_AES_ENCRYPT_DECRYPT              0x97    /**< AES加解密算法 */
#define CMD_KEYPAD_SM4_ENCRYPT_DECRYPT              0x98    /**< SM4加解密算法 */
#define CMD_KEYPAD_SM2_ENCRYPT_DECRYPT              0x99    /**< SM2加解密算法 */
#define CMD_KEYPAD_SM2_SIGN                         0x9A    /**< SM2签名算法 */
#define CMD_KEYPAD_SM2_VERIFY                       0x9B    /**< SM2验签算法 */
#define CMD_FILE_DOWNLOAD_START                     0xAA    /**< 文件下载启动 */
#define CMD_FILE_DOWNLOAD                           0xAB    /**< 文件下载 */
#define CMD_INTERNAL_AUTH                           0xA0    /**< 内部认证 */
#define CMD_EXTERNAL_AUTH_DOWNLOAD_HARDWARE_SERIAL  0xA1    /**< 外部认证后硬件序列号下载 */
#define CMD_EXTERNAL_AUTH_UNLOCK                    0xA7    /**< 外部认证后解锁 */
#define CMD_EXTERNAL_AUTH_ENCRYPT_CHIP_ID           0xA8    /**< 外部认证后加密芯片ID */
#define CMD_EXTERNAL_AUTH_RESET_BOOT                0xA9    /**< 外部认证后重置Boot */

/**
 * @brief 算法模式
 */
#define SDK_MATH_MODE_ECB  (0x00)            /**< ECB模式 */
#define SDK_MATH_MODE_CBC  (0x01)            /**< CBC模式 */
#define SDK_MATH_MODE_CFB  (0x02)            /**< CFB模式 */
#define SDK_MATH_MODE_OFB  (0x03)            /**< OFB模式 */

/**
 * @brief 加解密模式
 */
#define SDK_MATH_DECRYPT   (0x00)            /**< 解密 */
#define SDK_MATH_ENCRYPT   (0x01)            /**< 加密 */

/**
 * @brief 解锁模式
 */
#define SDK_UNLOCK_MODE_UNLOCK   (0x01)        /**< 解锁 */
#define SDK_UNLOCK_MODE_LOCK     (0x00)        /**< 锁定 */

/**
 * @brief 写入模式
 */
#define SDK_PED_DECRYPT  (0x00)              /**< 解密 */
#define SDK_PED_ENCRYPT  (0x01)              /**< 加密 */
#define SDK_PED_PLAINTEXT  (0x02)            /**< 明文 */
#define SDK_PED_DES  (0x00)                  /**< DES */
#define SDK_PED_SM4  (0x80)                  /**< SM4 */
#define SDK_PED_AES  (0x40)                  /**< AES */
#define SDK_PED_XOR  (0xC0)                  /**< XOR */

/**
 * @brief 打印对齐方式
 */
#define SDK_PRINT_ALIGN_LEFT  (0x01)          /**< 左对齐 */    
#define SDK_PRINT_ALIGN_RIGHT  (0x02)          /**< 右对齐 */
#define SDK_PRINT_ALIGN_CENTER  (0x04)          /**< 居中对齐 */

/**
 * @brief 寻卡方式
 */
#define SDK_SWIPE_CARD_HAND     (0x01)          /**< 手动寻卡 */
#define SDK_SWIPE_CARD_MAG      (0x02)          /**< 磁条卡寻卡 */
#define SDK_SWIPE_CARD_ICC      (0x04)          /**< IC卡寻卡 */
#define SDK_SWIPE_CARD_RF       (0x08)          /**< 非接卡寻卡 */

/**
 * @brief 非接卡类型
 */
#define SDK_ICC_TYPE_A  (0x01)                /**< A卡 */
#define SDK_ICC_TYPE_B  (0x02)                /**< B卡 */
#define SDK_ICC_TYPE_M1  (0x04)                /**< M1卡 */
#define SDK_ICC_TYPE_C  (0x08)                /**< C卡 */

/**
 * @brief 卡类型
 */
#define SDK_CARD_TYPE_CPU  (0x00)              /**< CPU卡 */
#define SDK_CARD_TYPE_AT24CXX  (0x01)          /**< AT24CXX */
#define SDK_CARD_TYPE_MEMORY  (0x02)          /**< 内存卡 */

/**
 * @brief 卡座号
 */
#define SDK_CARD_SEAT_STANDARD  (0x00)          /**< 标准卡座 */
#define SDK_CARD_SEAT_SAM1  (0x01)
#define SDK_CARD_SEAT_SAM2  (0x02)
#define SDK_CARD_SEAT_SAM3  (0x03)

/**
 * @brief 键盘背光状态
 */
#define SDK_KEYBOARD_BACKLIGHT_OFF  (0x00)        /**< 关闭 */
#define SDK_KEYBOARD_BACKLIGHT_ON  (0x01)


/**
 * @brief 键盘按键音状态
 */
#define SDK_KEYBOARD_SOUND_OFF  (0x00)          /**< 关闭 */
#define SDK_KEYBOARD_SOUND_ON  (0x01)           /**< 开启 */

/**
 * @brief 低功耗状态唤醒类型
 */
#define SDK_SLEEP_MODE_WAKEUP_TYPE_ANY_KEY  (0x01)  /**< 任意按键 */
#define SDK_SLEEP_MODE_WAKEUP_TYPE_IO  (0x02)      /**< IO口 */
#define SDK_SLEEP_MODE_WAKEUP_TYPE_IC  (0x04)      /**< IC卡 */
#define SDK_SLEEP_MODE_WAKEUP_TYPE_MAG  (0x08)      /**< 磁条卡 */
#define SDK_SLEEP_MODE_WAKEUP_TYPE_ICC  (0x10)      /**< 非接卡 */
#define SDK_SLEEP_MODE_WAKEUP_TYPE_TP  (0x20)

/**
 * @brief 指示灯 ID,按位控制
 */
#define SDK_LED_ID_BLUE  (0x01)              /**< 蓝色 */
#define SDK_LED_ID_YELLOW  (0x02)
#define SDK_LED_ID_GREEN  (0x04)
#define SDK_LED_ID_RED  (0x08)
#define SDK_LED_ID_ALL  (0x0F)

/**
 * @brief 蜂鸣器鸣叫类型
 */
#define SDK_BEEP_NORMAL  (0x00)              /**< 正常 */
#define SDK_BEEP_ABNORMAL  (0x01)            /**< 异常 */


/**
 * @brief 版本类型
 */
#define SDK_VERSION_TYPE_HARDWARE  (0x00)    /**< 硬件(内部版本号) */
#define SDK_VERSION_TYPE_BOOT  (0x01)        /**< Boot 版本号 */
#define SDK_VERSION_TYPE_KERNEL  (0x02)      /**< 内核版本号 */
#define SDK_VERSION_TYPE_SYSTEM  (0x03)      /**< 系统版本号 */
#define SDK_VERSION_TYPE_LIBRARY  (0x04)     /**< 底层库版本 */
#define SDK_VERSION_TYPE_SDK  (0x05)         /**< SDK 版本号 */

/**
 * @brief 通讯模式
 */
#define SDK_COMMUNICATION_MODE_NONE  (0x00)    /**< 不需要检测通讯模块 */
#define SDK_COMMUNICATION_MODE_GPRS  (0x01)    /**< GPRS,4G 等 */
#define SDK_COMMUNICATION_MODE_WIFI  (0x02)    /**< WIFI */
#define SDK_COMMUNICATION_MODE_BLUETOOTH  (0x04)    /**< 蓝牙 */





#endif