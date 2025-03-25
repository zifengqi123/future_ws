#include "future_devices.h"
#include <stdio.h>


// export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

int main(int argc, char *argv[])
{
    printf("hello world\n");


    future::future_devices& _devices = future::future_devices::getInstance();

    int ret = 0;
    ret = _devices.set_communication_mode(0);
    printf("set_communication_mode ret: %d\n", ret);

    uint8_t mode = 0;
    ret = _devices.get_network_mode(mode);
    printf("get_network_mode ret: %d, mode: %d\n", ret, mode);

    std::vector<uint8_t> version;
    ret = _devices.get_system_version(0x00, version);
    printf("get_system_version ret: %d\n", ret);
    printf("version: %s\n", version);

    ret = _devices.get_hardware_serial_number(version);
    printf("get_hardware_serial_number ret: %d\n", ret);
    printf("serial_number: %s\n", version);

    // ret = _devices.get_device_model(0);
    // printf("get_device_model ret: %d\n", ret);

    // ret = _devices.get_customer_serial_number(0);
    // printf("get_customer_serial_number ret: %d\n", ret);

    std::vector<uint8_t> time = {0x20,0x25,0x03,0x23,0x17,0x50};
    ret = _devices.set_clock(time);
    printf("set_clock ret: %d\n", ret);

    ret = _devices.get_clock(time);
    printf("get_clock ret: %d\n", ret);
    printf("time: %d-%d-%d %d:%d:%d\n", time[0], time[1], time[2], time[3], time[4], time[5]);

    ret = _devices.beep(0);
    printf("beep ret: %d\n", ret);

    // ret = _devices.beep_frequency(1000, 1000);
    // printf("beep_frequency ret: %d\n", ret);
    
    ret = _devices.led_on(0x0F);
    printf("led_on ret: %d\n", ret);

    // ret = _devices.led_off(0x0F);
    // printf("led_off ret: %d\n", ret);

    // ret = _devices.led_flash(0x0F, 1000);
    // printf("led_flash ret: %d\n", ret);

    // ret = _devices.system_reset();
    // printf("system_reset ret: %d\n", ret);
    // ret = _devices.system_shutdown();
    // printf("system_shutdown ret: %d\n", ret);
    // ret = _devices.set_terminal_serial_number(0);
    // printf("set_terminal_serial_number ret: %d\n", ret);
    // ret = _devices.get_chip_serial_number(0);
    // printf("get_chip_serial_number ret: %d\n", ret);
    // ret = _devices.set_enable_sleep_mode(0);
    // printf("set_enable_sleep_mode ret: %d\n", ret);
    // ret = _devices.get_enter_boot_state(0);
    // printf("get_enter_boot_state ret: %d\n", ret);
    // ret = _devices.open_keyboard();
    // printf("open_keyboard ret: %d\n", ret);
    // ret = _devices.close_keyboard();
    // printf("close_keyboard ret: %d\n", ret);
    // ret = _devices.clear_keyboard_cache();
    // printf("clear_keyboard_cache ret: %d\n", ret);
    // ret = _devices.set_keyboard_sound(0);
    // printf("set_keyboard_sound ret: %d\n", ret);
    // ret = _devices.set_keyboard_backlight(0);
    // printf("set_keyboard_backlight ret: %d\n", ret);
    // ret = _devices.tp_open(0x00);
    // printf("tp_open ret: %d\n", ret);
    // ret = _devices.tp_close();
    // printf("tp_close ret: %d\n", ret);
    // ret = _devices.tp_check_support();
    // printf("tp_check_support ret: %d\n", ret);
    // ret = _devices.tp_get_touch_coordinate(0, 0); 
    // printf("tp_get_touch_coordinate ret: %d\n", ret);
    // ret = _devices.tp_set_parameter(0);
    // printf("set_touch_parameter ret: %d\n", ret);
    // ret = _devices.open_magnetic_stripe_card();
    // printf("open_magnetic_stripe_card ret: %d\n", ret);
    // ret = close_magnetic_stripe_card();
    // printf("close_magnetic_stripe_card ret: %d\n", ret);
    // ret = check_magnetic_stripe_card();
    // // ret = read_magnetic_stripe_card_data(0, 0, 0, 0, 0, 0);
    // printf("read_magnetic_stripe_card_data ret: %d\n", ret);
    // ret = clear_magnetic_stripe_card_data();
    // printf("clear_magnetic_stripe_card_data ret: %d\n", ret);
    // // ret = format_magnetic_stripe_card_data(0, 0, 0, 0, 0, 0);
    // printf("format_magnetic_stripe_card_data ret: %d\n", ret);
    // ret = open_ic_card_module(0, 0);
    // printf("open_ic_card_module ret: %d\n", ret);
    // ret = close_ic_card_module(0, 0);
    // printf("close_ic_card_module ret: %d\n", ret);
    // ret = check_icc_status(0, 0);
    // printf("check_icc_status ret: %d\n", ret);
    // ret = ic_card_reset(0, 0);
    // printf("ic_card_reset ret: %d\n", ret);
    // ret = ic_card_module_power_off(0, 0); 
    // printf("ic_card_module_power_off ret: %d\n", ret);
    // ret = icc_open_module();
    // printf("icc_open_module ret: %d\n", ret);
    // ret = icc_close_module();
    // printf("icc_close_module ret: %d\n", ret);
    // ret = icc_search_card_activation();
    // printf("icc_search_card_activation ret: %d\n", ret);
    // ret = icc_read_card_data();
    // printf("icc_read_card_data ret: %d\n", ret);
    // ret = icc_send_apdu_command(0, 0);
    // printf("icc_send_apdu_command ret: %d\n", ret);
    // ret = mifare_card_authentication();
    // printf("mifare_card_authentication ret: %d\n", ret);
    // ret = mifare_card_operation();
    // printf("mifare_card_operation ret: %d\n", ret);
    // ret = single_search_card_message();
    // printf("single_search_card_message ret: %d\n", ret);
    // ret = send_search_card_result_message();
    // printf("send_search_card_result_message ret: %d\n", ret);
    // ret = end_search_card();
    // printf("end_search_card ret: %d\n", ret);
    // ret = scan_open();
    // printf("scan_open ret: %d\n", ret);
    // ret = scan_close();
    // printf("scan_close ret: %d\n", ret);
    // ret = read_scan_data();           
    // printf("read_scan_data ret: %d\n", ret);
    // ret = check_printer_support();
    // printf("check_printer_support ret: %d\n", ret);
    // ret = printer_open();
    // printf("printer_open ret: %d\n", ret);
    // ret = printer_close();
    // printf("printer_close ret: %d\n", ret);
    // ret = query_printer_status();
    // printf("query_printer_status ret: %d\n", ret);
    // ret = set_printer_gray(); 
    // printf("set_printer_gray ret: %d\n", ret);
    // ret = paper_feed();
    // printf("paper_feed ret: %d\n", ret);
    // ret = print_bitmap_data();
    // printf("print_bitmap_data ret: %d\n", ret);
    // ret = keyboard_open_password();
    // printf("keyboard_open_password ret: %d\n", ret);
    // ret = keyboard_close_password();
    // printf("keyboard_close_password ret: %d\n", ret);
    // ret = keyboard_get_random_number();
    // printf("keyboard_get_random_number ret: %d\n", ret);
    // ret = keyboard_update_master_key();
    // printf("keyboard_update_master_key ret: %d\n", ret);
    // ret = keyboard_update_work_key();
    // printf("keyboard_update_work_key ret: %d\n", ret);
    // ret = keyboard_encrypt_data();
    // printf("keyboard_encrypt_data ret: %d\n", ret);
    // ret = keyboard_encrypt_magnetic_stripe_data();
    // printf("keyboard_encrypt_magnetic_stripe_data ret: %d\n", ret);
    // ret = keyboard_calculate_mac();
    // printf("keyboard_calculate_mac ret: %d\n", ret);
    // ret = keyboard_input_online_pin();
    // printf("keyboard_input_online_pin ret: %d\n", ret);
    // ret = keyboard_generate_rsa_key_pair_output_public_key(); 
    // printf("keyboard_generate_rsa_key_pair_output_public_key ret: %d\n", ret);
    // ret = keyboard_encrypt_rsa_private_key();
    // printf("keyboard_encrypt_rsa_private_key ret: %d\n", ret);
    // ret = keyboard_encrypt_hardware_serial_number();
    // printf("keyboard_encrypt_hardware_serial_number ret: %d\n", ret);
    // ret = keyboard_check_trigger_status();
    // printf("keyboard_check_trigger_status ret: %d\n", ret);
    // ret = keyboard_set_trigger_status();
    // printf("keyboard_set_trigger_status ret: %d\n", ret);
    // ret = keyboard_release_trigger();
    // printf("keyboard_release_trigger ret: %d\n", ret);
    // ret = keyboard_check_key();
    // printf("keyboard_check_key ret: %d\n", ret);
    // ret = sm3_hash_algorithm();
    // printf("sm3_hash_algorithm ret: %d\n", ret);
    // ret = des_encrypt_decrypt_algorithm();
    // printf("des_encrypt_decrypt_algorithm ret: %d\n", ret);
    // ret = aes_encrypt_decrypt_algorithm();
    // printf("aes_encrypt_decrypt_algorithm ret: %d\n", ret);
    // ret = sm4_encrypt_decrypt_algorithm();
    // printf("sm4_encrypt_decrypt_algorithm ret: %d\n", ret);
    // ret = sm2_signature_algorithm();
    // printf("sm2_signature_algorithm ret: %d\n", ret);
    // ret = sm2_verify_algorithm(); 
    // ret = file_download_start_download();
    // printf("file_download_start_download ret: %d\n", ret);
    // ret = file_download();
    // printf("file_download ret: %d\n", ret);
    // ret = internal_authentication();
    // printf("internal_authentication ret: %d\n", ret);
    // ret = external_authentication_hardware_serial_number_download();
    // printf("external_authentication_hardware_serial_number_download ret: %d\n", ret);
    // ret = external_authentication_unlock(0);
    // printf("external_authentication_unlock ret: %d\n", ret);
    // ret = external_authentication_encrypted_chip_id();
    // printf("external_authentication_encrypted_chip_id ret: %d\n", ret);
    // ret = external_authentication_reset_boot();
    


    return 0;
}