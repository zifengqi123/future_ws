#include "status_ui.h"

namespace future_gui
{

static void update_status_timer_cb(lv_timer_t *timer)
{
    status_ui *ui = (status_ui *)lv_timer_get_user_data(timer);
    ui->update_status();
}

status_ui::status_ui(lv_obj_t *parent)
{

    _status_bar.parent = parent;
    _status_bar.obj = lv_obj_create(_status_bar.parent);

    lv_obj_set_pos(_status_bar.obj, 0, 0);
	lv_obj_set_size(_status_bar.obj, STATUS_VIEW_WIDTH, STATUS_VIEW_HIGHT);
    lv_obj_set_style_bg_color(_status_bar.obj, lv_color_hex(STATUS_BAR_BG_COLOR), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(_status_bar.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    lv_obj_set_scrollbar_mode(_status_bar.obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(_status_bar.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	lv_obj_set_style_pad_top(_status_bar.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(_status_bar.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(_status_bar.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(_status_bar.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    signal_bar_init(_status_bar.obj, 10, 0, 30, STATUS_BAR_HIGHT);
    wifi_bar_init(_status_bar.obj, 35, 0, 20, STATUS_BAR_HIGHT);
    print_bar_init(_status_bar.obj, 60, 0, 20, STATUS_BAR_HIGHT);

    battery_bar_init(_status_bar.obj, 220, 0, 30, STATUS_BAR_HIGHT);
    timebar_init(_status_bar.obj, 265, 0, 55, STATUS_BAR_HIGHT);


    pthread_mutex_init(&_update_mutex, NULL);
    _update_timer = lv_timer_create(update_status_timer_cb, 1000, this);
    lv_timer_ready(_update_timer);
}


status_ui::~status_ui()
{
    signal_bar_del();
    wifi_bar_del();
    print_bar_del();
    battery_bar_del();
    timebar_del();

    if(_status_bar.obj) lv_obj_del(_status_bar.obj);
    if(_update_timer) lv_timer_del(_update_timer);
}

void status_ui::set_update_callback(std::function<void(status_info_t & status_info)> callback)
{
    _update_callback = callback;
}

void status_ui::update_status()
{
    // printf("update_status\n");
    if(_update_callback) {
        _update_callback(_status_info);

        pthread_mutex_lock(&_update_mutex);
        battery_bar_show(_status_info.battery_per, _status_info.is_battery_charge);
        pthread_mutex_unlock(&_update_mutex);

        pthread_mutex_lock(&_update_mutex);
        print_bar_show(_status_info.is_print_err);
        pthread_mutex_unlock(&_update_mutex);

        pthread_mutex_lock(&_update_mutex);
        signal_bar_show(_status_info.signal_level, _status_info.signal_type.c_str());
        pthread_mutex_unlock(&_update_mutex);

        pthread_mutex_lock(&_update_mutex);
        timebar_show(_status_info.time_stamp);
        pthread_mutex_unlock(&_update_mutex);

        pthread_mutex_lock(&_update_mutex);
        wifi_bar_show(_status_info.has_wifi, _status_info.is_wifi_connect);
        pthread_mutex_unlock(&_update_mutex);
    }
    else {

        pthread_mutex_lock(&_update_mutex);
        battery_bar_show(t, (int)(t*10)%2);
        pthread_mutex_unlock(&_update_mutex);

        pthread_mutex_lock(&_update_mutex);
        print_bar_show((int)(t*10)%2);
        pthread_mutex_unlock(&_update_mutex);

        pthread_mutex_lock(&_update_mutex);
        wifi_bar_show(true, (int)(t*10)%2);
        pthread_mutex_unlock(&_update_mutex);

        //更新信号强度
        pthread_mutex_lock(&_update_mutex);
        signal_bar_show((((int)(t*10)) % 6), "4G");
        pthread_mutex_unlock(&_update_mutex);

        //更新时间
        pthread_mutex_lock(&_update_mutex);

        static char time_str[9]; // 用于存放时间字符串
        time_t now;
        struct tm *time_info;

        time(&now);
        timebar_show(now);
        pthread_mutex_unlock(&_update_mutex);


        t += 0.1;
        if(t > 1.1) {
            t = 0;
        }
    }

}

void status_ui::battery_bar_addflag()
{
    _battery_bar.battflag = lv_obj_create(_battery_bar.parent);


    lv_obj_set_style_bg_color(_battery_bar.battflag, lv_color_hex(0xFFFFFF), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(_battery_bar.battflag, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    lv_obj_set_scrollbar_mode(_battery_bar.battflag, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(_battery_bar.battflag, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	lv_obj_set_style_pad_top(_battery_bar.battflag, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(_battery_bar.battflag, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(_battery_bar.battflag, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(_battery_bar.battflag, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //设置宽高
    lv_obj_set_size(_battery_bar.battflag, 3, _battery_bar.hight-10);

    lv_obj_align_to(_battery_bar.battflag, _battery_bar.outline, LV_ALIGN_OUT_RIGHT_MID, 0, 0);


    // lv_obj_set_style_outline_width(_battery_bar.battflag, 0, 0);
    // lv_obj_set_style_outline_pad(_battery_bar.battflag, 0, 0);
    // lv_obj_set_style_border_width(_battery_bar.battflag, 0, 0);
    // lv_obj_set_style_bg_color(_battery_bar.battflag, lv_color_hex(0xB0B0B0), 0);

}

void status_ui::battery_bar_addcharge()
{
    _battery_bar.charge = lv_label_create(_battery_bar.parent);
    lv_obj_set_size(_battery_bar.charge, 8, _battery_bar.hight);
    lv_obj_align_to(_battery_bar.charge, _battery_bar.battflag, LV_ALIGN_OUT_RIGHT_MID, 4, 3);
    lv_obj_set_style_text_color(_battery_bar.charge, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(_battery_bar.charge, &lv_font_montserrat_12, 0);

}

void status_ui::battery_bar_init(lv_obj_t *parent, int x, int y, int width, int hight)
{
    _battery_bar.parent = parent;
    _battery_bar.x = x;
    _battery_bar.y = y;
    _battery_bar.width = width;
    _battery_bar.hight = hight;

    _battery_bar.outline = lv_obj_create(_battery_bar.parent);

    //设置border和pading
    lv_obj_set_style_border_width(_battery_bar.outline, 2, LV_PART_MAIN|LV_STATE_DEFAULT);

	lv_obj_set_style_pad_top(_battery_bar.outline, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(_battery_bar.outline, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(_battery_bar.outline, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(_battery_bar.outline, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //设置圆角
    lv_obj_set_style_radius(_battery_bar.outline, 2, 0);

    //关闭滚动条
    lv_obj_clear_flag(_battery_bar.outline, LV_OBJ_FLAG_SCROLLABLE);

    //设置宽高
    lv_obj_set_size(_battery_bar.outline, _battery_bar.width, _battery_bar.hight-4);

    //居中对齐
    lv_obj_align(_battery_bar.outline, LV_ALIGN_LEFT_MID, _battery_bar.x, _battery_bar.y);

    lv_obj_set_style_bg_color(_battery_bar.outline, lv_color_hex(0xD0D0D0), 0);

    //电池电量填充obj
    _battery_bar.pad = lv_obj_create(_battery_bar.outline);

    //设置outline
    lv_obj_set_style_outline_width(_battery_bar.pad, 0, 0);
    lv_obj_set_style_outline_pad(_battery_bar.pad, 0, 0);
    lv_obj_set_style_border_width(_battery_bar.pad, 0, 0);
    //设置背景色
    // lv_obj_set_style_bg_color(batt.pad, lv_color_hex(0xff0000), 0);

    //设置宽高
    lv_obj_set_size(_battery_bar.pad, _battery_bar.width, _battery_bar.hight - 4);
    lv_obj_set_style_border_width(_battery_bar.pad, 0, 0);

    //设置圆角
    lv_obj_set_style_radius(_battery_bar.pad, 2, 0);

    //居中对齐
    lv_obj_align(_battery_bar.pad, LV_ALIGN_LEFT_MID, 0, 0);

    //关闭滚动条
    lv_obj_clear_flag(_battery_bar.pad, LV_OBJ_FLAG_SCROLLABLE);

    //电池百分比
    _battery_bar.label = lv_label_create(_battery_bar.outline);
    lv_obj_align(_battery_bar.label, LV_ALIGN_CENTER, 0, 0);

    _battery_bar.text = lv_obj_get_child(lv_obj_get_parent(_battery_bar.pad), -1);
    lv_obj_set_size(_battery_bar.text, _battery_bar.width, _battery_bar.hight - 6);
    lv_obj_set_style_text_align(_battery_bar.text, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(_battery_bar.text, &lv_font_montserrat_12, 0);

    battery_bar_addflag();
    battery_bar_addcharge();
}

void status_ui::battery_bar_del()
{
    if(_battery_bar.outline) lv_obj_del(_battery_bar.outline);
    if(_battery_bar.battflag) lv_obj_del(_battery_bar.battflag);
    if(_battery_bar.pad) lv_obj_del(_battery_bar.pad);
    if(_battery_bar.label) lv_obj_del(_battery_bar.label);
    if(_battery_bar.text) lv_obj_del(_battery_bar.text);
    if(_battery_bar.charge) lv_obj_del(_battery_bar.charge);
}

void status_ui::battery_bar_show(float per, bool is_charge)
{
    if (per < 0.2)
    {
        lv_obj_set_style_bg_color(_battery_bar.pad, lv_color_hex(0xff0000), 0);
    }
    else if(per < 0.5)
    {
        lv_obj_set_style_bg_color(_battery_bar.pad, lv_color_hex(0xffff00), 0);
    }
    else
    {
        lv_obj_set_style_bg_color(_battery_bar.pad, lv_color_hex(0xff00), 0);
    }

    if(per > 0.97) {
        lv_obj_set_style_bg_color(_battery_bar.outline, lv_color_hex(0xff00), 0);
        lv_obj_set_style_bg_color(_battery_bar.battflag, lv_color_hex(0xff00), 0);
    }
    else {
        lv_obj_set_style_bg_color(_battery_bar.outline, lv_color_hex(0xD0D0D0), 0);
        lv_obj_set_style_bg_color(_battery_bar.battflag, lv_color_hex(0xFFFFFF), 0);
    }

    //修改电量颜色obj宽度
    lv_obj_set_width(_battery_bar.pad, (int32_t)(_battery_bar.width) * per);

    //修改电池百分比
    lv_obj_set_style_text_color(_battery_bar.text, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text_fmt(_battery_bar.text, "%d", (int32_t)(per*100));

    if(is_charge) {
        lv_label_set_text(_battery_bar.charge, LV_SYMBOL_CHARGE);
    }
    else{
        lv_label_set_text(_battery_bar.charge, "");
    }
}

void status_ui::print_bar_init(lv_obj_t *parent, int x, int y, int width, int hight)
{
    _print_bar.parent = parent;
    _print_bar.x = x;
    _print_bar.y = y;
    _print_bar.width = width;
    _print_bar.hight = hight;

    _print_bar.label_0 = lv_obj_create(_print_bar.parent);
    _print_bar.label_1 = lv_obj_create(_print_bar.parent);
    _print_bar.label_2 = lv_obj_create(_print_bar.parent);
    _print_bar.text = lv_label_create(_print_bar.parent);

    lv_obj_set_style_border_width(_print_bar.label_0, 1, 0);
    lv_obj_set_style_pad_all(_print_bar.label_0, 0, 0);

    lv_obj_set_style_border_width(_print_bar.label_1, 1, 0);
    lv_obj_set_style_pad_all(_print_bar.label_1, 0, 0);

    lv_obj_set_style_radius(_print_bar.label_0, 3, 0);
    lv_obj_set_style_radius(_print_bar.label_1, 3, 0);

    lv_obj_set_size(_print_bar.label_0, _print_bar.width - 6, 8);
    lv_obj_align(_print_bar.label_0, LV_ALIGN_TOP_LEFT, _print_bar.x + 3, _print_bar.y + 3);

    lv_obj_set_size(_print_bar.label_1, _print_bar.width, 12);
    lv_obj_align(_print_bar.label_1, LV_ALIGN_TOP_LEFT, _print_bar.x, _print_bar.y + 8);

    lv_obj_set_size(_print_bar.label_2, _print_bar.width - 6, 6);
    lv_obj_align(_print_bar.label_2, LV_ALIGN_TOP_LEFT, _print_bar.x + 3, _print_bar.y + 14);

    lv_obj_set_style_outline_width(_print_bar.label_0, 0, 0);
    lv_obj_set_style_outline_pad(_print_bar.label_0, 0, 0);

    lv_obj_set_style_outline_width(_print_bar.label_1, 0, 0);
    lv_obj_set_style_outline_pad(_print_bar.label_1, 0, 0);

    lv_obj_set_style_bg_color(_print_bar.label_0, lv_color_hex(0xf0f0f0), 0);
    lv_obj_set_style_bg_color(_print_bar.label_1, lv_color_hex(0xb0b0b0), 0);
    // lv_obj_set_style_bg_color(_print_bar.label_1, lv_color_hex(0xff0000), 0);
    lv_obj_set_style_bg_color(_print_bar.label_2, lv_color_hex(0xf0f0f0), 0);


    lv_obj_set_size(_print_bar.text, 12, 12);
    lv_obj_align(_print_bar.text, LV_ALIGN_TOP_LEFT, _print_bar.x + 8, _print_bar.y + 6);
    lv_obj_set_style_text_font(_print_bar.text, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(_print_bar.text, lv_color_hex(0xff0000), 0);
}

void status_ui::print_bar_del()
{
    if(_print_bar.label_0) lv_obj_del(_print_bar.label_0);
    if(_print_bar.label_1) lv_obj_del(_print_bar.label_1);
    if(_print_bar.label_2) lv_obj_del(_print_bar.label_2);
    if(_print_bar.text) lv_obj_del(_print_bar.text);
}

void status_ui::print_bar_show(bool is_err)
{
    if(is_err) {
        lv_label_set_text(_print_bar.text, LV_SYMBOL_CLOSE);
        // lv_obj_clear_flag(_print_bar.text, LV_OBJ_FLAG_HIDDEN);
    }
    else {
        lv_label_set_text(_print_bar.text, "");
        // lv_obj_add_flag(_print_bar.text, LV_OBJ_FLAG_HIDDEN);
    }
}

void status_ui::signal_bar_init(lv_obj_t *parent, int x, int y, int width, int hight)
{
    _signal_bar.parent = parent;
    _signal_bar.x = x;
    _signal_bar.y = y;
    _signal_bar.width = width;
    _signal_bar.hight = hight;

    _signal_bar.label_0 = lv_obj_create(_signal_bar.parent);
    _signal_bar.label_1 = lv_obj_create(_signal_bar.parent);
    _signal_bar.label_2 = lv_obj_create(_signal_bar.parent);
    _signal_bar.label_3 = lv_obj_create(_signal_bar.parent);
    _signal_bar.label_4 = lv_obj_create(_signal_bar.parent);

    lv_obj_clear_flag(_signal_bar.label_0, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(_signal_bar.label_1, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(_signal_bar.label_2, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(_signal_bar.label_3, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(_signal_bar.label_4, LV_OBJ_FLAG_SCROLLABLE);

    int signal_h = (_signal_bar.hight - 2) / 5;
    //设置宽高
    lv_obj_set_size(_signal_bar.label_0, 2, 1 * signal_h);
    lv_obj_set_size(_signal_bar.label_1, 2, 2 * signal_h);
    lv_obj_set_size(_signal_bar.label_2, 2, 3 * signal_h);
    lv_obj_set_size(_signal_bar.label_3, 2, 4 * signal_h);
    lv_obj_set_size(_signal_bar.label_4, 2, 5 * signal_h);

    // _signal_bar.lable_txt = lv_label_create(_signal_bar.parent);
	// lv_label_set_text(_signal_bar.lable_txt, "4G");
	// lv_label_set_long_mode(_signal_bar.lable_txt, LV_LABEL_LONG_WRAP);
	// lv_obj_set_pos(_signal_bar.lable_txt, _signal_bar.x, _signal_bar.y + 1);
	// lv_obj_set_size(_signal_bar.lable_txt, 16, 8);
	// lv_obj_set_scrollbar_mode(_signal_bar.lable_txt, LV_SCROLLBAR_MODE_OFF);
    // lv_obj_set_style_text_font(_signal_bar.lable_txt, &lv_font_montserrat_12, 0);

    int txp = _signal_bar.x;    // + 5;
    int ty = 4;

    lv_obj_set_pos(_signal_bar.label_0, txp, _signal_bar.y + 4 * signal_h + ty);
    lv_obj_set_pos(_signal_bar.label_1, txp + 4, _signal_bar.y + 3 * signal_h + ty);
    lv_obj_set_pos(_signal_bar.label_2, txp + 8, _signal_bar.y + 2 * signal_h + ty);
    lv_obj_set_pos(_signal_bar.label_3, txp + 12, _signal_bar.y + 1 * signal_h + ty);
    lv_obj_set_pos(_signal_bar.label_4, txp + 16, _signal_bar.y + 0 * signal_h + ty);


    lv_obj_set_style_outline_width(_signal_bar.label_0, 0, 0);
    lv_obj_set_style_outline_pad(_signal_bar.label_0, 0, 0);
    lv_obj_set_style_border_width(_signal_bar.label_0, 0, 0);

    lv_obj_set_style_outline_width(_signal_bar.label_1, 0, 0);
    lv_obj_set_style_outline_pad(_signal_bar.label_1, 0, 0);
    lv_obj_set_style_border_width(_signal_bar.label_1, 0, 0);

    lv_obj_set_style_outline_width(_signal_bar.label_2, 0, 0);
    lv_obj_set_style_outline_pad(_signal_bar.label_2, 0, 0);
    lv_obj_set_style_border_width(_signal_bar.label_2, 0, 0);

    lv_obj_set_style_outline_width(_signal_bar.label_3, 0, 0);
    lv_obj_set_style_outline_pad(_signal_bar.label_3, 0, 0);
    lv_obj_set_style_border_width(_signal_bar.label_3, 0, 0);

    lv_obj_set_style_outline_width(_signal_bar.label_4, 0, 0);
    lv_obj_set_style_outline_pad(_signal_bar.label_4, 0, 0);
    lv_obj_set_style_border_width(_signal_bar.label_4, 0, 0);


}

void status_ui::signal_bar_del()
{
    if(_signal_bar.label_0) lv_obj_del(_signal_bar.label_0);
    if(_signal_bar.label_1) lv_obj_del(_signal_bar.label_1);
    if(_signal_bar.label_2) lv_obj_del(_signal_bar.label_2);
    if(_signal_bar.label_3) lv_obj_del(_signal_bar.label_3);
    if(_signal_bar.label_4) lv_obj_del(_signal_bar.label_4);
    if(_signal_bar.lable_txt) lv_obj_del(_signal_bar.lable_txt);
}

void status_ui::signal_bar_show(int level, const char* signal)
{
    switch (level)
    {
    case 1:
        lv_obj_set_style_bg_color(_signal_bar.label_0, lv_color_hex(0xff0000), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_1, lv_color_hex(0xc0c0c0), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_2, lv_color_hex(0xc0c0c0), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_3, lv_color_hex(0xc0c0c0), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_4, lv_color_hex(0xc0c0c0), 0);

        break;
    case 2:
        lv_obj_set_style_bg_color(_signal_bar.label_0, lv_color_hex(0xff0000), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_1, lv_color_hex(0xff0000), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_2, lv_color_hex(0xc0c0c0), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_3, lv_color_hex(0xc0c0c0), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_4, lv_color_hex(0xc0c0c0), 0);
        break;
    case 3:
        lv_obj_set_style_bg_color(_signal_bar.label_0, lv_color_hex(0xffff00), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_1, lv_color_hex(0xffff00), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_2, lv_color_hex(0xffff00), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_3, lv_color_hex(0xc0c0c0), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_4, lv_color_hex(0xc0c0c0), 0);
        break;
    case 4:
        lv_obj_set_style_bg_color(_signal_bar.label_0, lv_color_hex(0x00ff00), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_1, lv_color_hex(0x00ff00), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_2, lv_color_hex(0x00ff00), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_3, lv_color_hex(0x00ff00), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_4, lv_color_hex(0xc0c0c0), 0);
        break;
    case 5:
        lv_obj_set_style_bg_color(_signal_bar.label_0, lv_color_hex(0x00ff00), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_1, lv_color_hex(0x00ff00), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_2, lv_color_hex(0x00ff00), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_3, lv_color_hex(0x00ff00), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_4, lv_color_hex(0x00ff00), 0);

        break;
    default:
        lv_obj_set_style_bg_color(_signal_bar.label_0, lv_color_hex(0xc0c0c0), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_1, lv_color_hex(0xc0c0c0), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_2, lv_color_hex(0xc0c0c0), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_3, lv_color_hex(0xc0c0c0), 0);
        lv_obj_set_style_bg_color(_signal_bar.label_4, lv_color_hex(0xc0c0c0), 0);

        break;
    }

    // if(level == 0) {
    //     lv_obj_set_style_text_color(_signal_bar.lable_txt, lv_color_hex(0xff0000), 0);
    //     lv_label_set_text(_signal_bar.lable_txt, LV_SYMBOL_CLOSE);
    // }
    // else {
    //     lv_obj_set_style_text_color(_signal_bar.lable_txt, lv_color_hex(0x000000), 0);
    //     lv_label_set_text(_signal_bar.lable_txt, signal);
    // }
}


void status_ui::timebar_init(lv_obj_t *parent, int x, int y, int width, int hight)
{
    _time_bar.parent = parent;
    _time_bar.x = x;
    _time_bar.y = y;
    _time_bar.width = width;
    _time_bar.hight = hight;

    _time_bar.obj = lv_label_create(_time_bar.parent);
    lv_obj_set_size(_time_bar.obj, _time_bar.width, _time_bar.hight);
    lv_obj_align(_time_bar.obj, LV_ALIGN_LEFT_MID, _time_bar.x, _time_bar.y);
    lv_label_set_text(_time_bar.obj, "");

    _time_bar.label = lv_label_create(_time_bar.obj);
	lv_label_set_text(_time_bar.label, "--:--");
	lv_label_set_long_mode(_time_bar.label, LV_LABEL_LONG_WRAP);
    lv_obj_align(_time_bar.label, LV_ALIGN_CENTER, 0, 0);
	// lv_obj_set_size(_time_bar.label, _time_bar.width, _time_bar.hight);
	lv_obj_set_scrollbar_mode(_time_bar.label, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_text_color(_time_bar.label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(_time_bar.label, &lv_font_montserrat_14, 0);
}

void status_ui::timebar_del()
{
    if(_time_bar.label) lv_obj_del(_time_bar.label);
}


void status_ui::timebar_show(time_t time_stamp)
{
    struct tm *timeinfo = localtime(&time_stamp);
    char time_str[10];
    strftime(time_str, sizeof(time_str), "%H:%M", timeinfo);
    lv_label_set_text(_time_bar.label, time_str);
}

void status_ui::wifi_bar_init(lv_obj_t *parent, int x, int y, int width, int hight)
{
    _wifi_bar.parent = parent;
    _wifi_bar.x = x;
    _wifi_bar.y = y;
    _wifi_bar.width = width;
    _wifi_bar.hight = hight;

    _wifi_bar.label = lv_label_create(_wifi_bar.parent);

    lv_label_set_text(_wifi_bar.label, LV_SYMBOL_WIFI);

	lv_label_set_long_mode(_wifi_bar.label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(_wifi_bar.label, _wifi_bar.x, _wifi_bar.y + 6);
	lv_obj_set_size(_wifi_bar.label, _wifi_bar.width, _wifi_bar.hight);
	lv_obj_set_scrollbar_mode(_wifi_bar.label, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_text_align(_wifi_bar.label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(_wifi_bar.label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(_wifi_bar.label, &lv_font_montserrat_12, 0);
}

void status_ui::wifi_bar_del()
{
    if(_wifi_bar.label) lv_obj_del(_wifi_bar.label);
}


void status_ui::wifi_bar_show(bool has_wifi, bool is_connect)
{
    if (has_wifi)
    {
        // lv_obj_clear_flag(_wifi_bar.label, LV_OBJ_FLAG_HIDDEN);
        if(is_connect) {
            lv_label_set_text(_wifi_bar.label, LV_SYMBOL_WIFI);
        }
        else {
            lv_label_set_text(_wifi_bar.label, "");
        }
    }
    else {
        // lv_obj_add_flag(_wifi_bar.label, LV_OBJ_FLAG_HIDDEN);
    }

}


}
