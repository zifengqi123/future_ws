#include "notice_ui.h"

namespace future_gui
{

notice_ui::notice_ui(lv_obj_t *parent)
{
    _notice_ui.parent = parent;
    notice_init(_notice_ui.parent, 0, NOTICE_STATUS_VIEW_HIGHT, NOTICE_VIEW_WIDTH, NOTICE_VIEW_HIGHT);
}

notice_ui::~notice_ui()
{
}

void notice_ui::notice_init(lv_obj_t *parent, int x, int y, int width, int hight)
{
    _notice_ui.parent = parent;
    _notice_ui.x = x;
    _notice_ui.y = y;
    _notice_ui.width = width;
    _notice_ui.hight = hight;


    _notice_ui.obj = lv_label_create(_notice_ui.parent);
    lv_obj_set_size(_notice_ui.obj, _notice_ui.width, _notice_ui.hight);
    lv_obj_set_pos(_notice_ui.obj, _notice_ui.x, _notice_ui.y);


    lv_obj_set_style_outline_width(_notice_ui.obj, 0, 0);
    lv_obj_set_style_outline_pad(_notice_ui.obj, 0, 0);
    lv_obj_set_style_border_width(_notice_ui.obj, 0, 0);
    lv_obj_set_style_radius(_notice_ui.obj, 0, 0);
    lv_obj_set_scrollbar_mode(_notice_ui.obj, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(_notice_ui.obj, "");

    _notice_ui.title = lv_obj_create(_notice_ui.obj);
    lv_obj_set_size(_notice_ui.title, _notice_ui.width, 36);
    // lv_obj_set_pos(_ui_notice.title, _ui_notice.x, _ui_notice.y);
    lv_obj_set_pos(_notice_ui.title, 0, 0);

    lv_obj_set_style_outline_width(_notice_ui.title, 0, 0);
    lv_obj_set_style_outline_pad(_notice_ui.title, 0, 0);
    lv_obj_set_style_border_width(_notice_ui.title, 0, 0);
    lv_obj_set_style_radius(_notice_ui.title, 0, 0);
    lv_obj_set_scrollbar_mode(_notice_ui.title, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_bg_color(_notice_ui.title, lv_color_hex(NOTICE_VIEW_BG_COLOR), 0);

    _notice_ui.title_txt = lv_label_create(_notice_ui.title);
    lv_obj_align(_notice_ui.title_txt, LV_ALIGN_CENTER, 0, 0);

    // lv_obj_set_style_text_font(_ui_notice.title_txt, &lv_font_simsun_16_cjk, 0);

    // lv_obj_set_style_base_dir(_ui_notice.title_txt, LV_BASE_DIR_RTL, 0);
    lv_obj_set_style_text_font(_notice_ui.title_txt, &lv_font_dejavu_16_persian_hebrew, 0);


    _notice_ui.label = lv_label_create(_notice_ui.obj);
	lv_label_set_long_mode(_notice_ui.label, LV_LABEL_LONG_WRAP);
	lv_obj_set_scrollbar_mode(_notice_ui.label, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_border_width(_notice_ui.label, 0, 0);
    lv_obj_set_style_radius(_notice_ui.label, 0, 0);

    lv_obj_set_style_base_dir(_notice_ui.label, LV_BASE_DIR_AUTO, 0);
    lv_obj_set_style_text_font(_notice_ui.label, &lv_font_montserrat_18, 0);
    lv_obj_align(_notice_ui.label, LV_ALIGN_CENTER, 0, 0);
}

void notice_ui::notice_del()
{
    if(_notice_ui.obj) lv_obj_del(_notice_ui.obj);
    if(_notice_ui.label) lv_obj_del(_notice_ui.label);
    if(_notice_ui.title) lv_obj_del(_notice_ui.title);
    if(_notice_ui.title_txt) lv_obj_del(_notice_ui.title_txt);
}

void notice_ui::notice_show(const char* title, const char* text)
{
    lv_label_set_text(_notice_ui.title_txt, title);
    lv_label_set_text(_notice_ui.label, text);
}
}
