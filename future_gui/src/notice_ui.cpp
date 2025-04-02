#include "notice_ui.h"
#include "config_gui.h"

extern "C" {

ui_notice_t _notice_ui;

void notice_ui_init(lv_obj_t *parent)
{
    _notice_ui.parent = parent;
    _notice_ui.x_start = 0;
    _notice_ui.y_start = STATUS_VIEW_HIGHT;
    _notice_ui.width = NOTICE_VIEW_WIDTH;
    _notice_ui.hight = NOTICE_VIEW_HIGHT;


    _notice_ui.obj = lv_obj_create(_notice_ui.parent);
    lv_obj_set_size(_notice_ui.obj, _notice_ui.width, _notice_ui.hight);
    lv_obj_set_pos(_notice_ui.obj, _notice_ui.x_start, _notice_ui.y_start);

    lv_obj_set_style_radius(_notice_ui.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(_notice_ui.obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(_notice_ui.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(_notice_ui.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(_notice_ui.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(_notice_ui.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(_notice_ui.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(_notice_ui.obj, lv_color_hex(MENU_ITEM_UNSELECT_BG_COLOR), LV_PART_MAIN|LV_STATE_DEFAULT);


    _notice_ui.label = lv_label_create(_notice_ui.obj);
    lv_obj_set_width(_notice_ui.label, _notice_ui.width - MENU_ITEM_TEXT_SIZE*2);
	lv_label_set_long_mode(_notice_ui.label, LV_LABEL_LONG_WRAP);
	lv_obj_set_scrollbar_mode(_notice_ui.label, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(_notice_ui.label, LV_ALIGN_CENTER, 0, -12);


    // lv_obj_set_style_text_align(_notice_ui.label, MENU_ITEM_TEXT_ALIGN, 0);
    lv_obj_set_style_text_align(_notice_ui.label, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_set_style_base_dir(_notice_ui.label, MENU_ITEM_TEXT_DIRSTYLE, 0);
    lv_obj_set_style_text_font(_notice_ui.label, MENU_ITEM_TEXT_FONT, 0);

}

void notice_ui_del()
{
    if(_notice_ui.obj) lv_obj_del(_notice_ui.obj);
    if(_notice_ui.label) lv_obj_del(_notice_ui.label);
}

void notice_ui_show(const char* msg)
{
    lv_label_set_text(_notice_ui.label, msg);

}


}
