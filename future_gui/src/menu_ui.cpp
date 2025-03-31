#include "menu_ui.h"

namespace future_gui {

static void menu_timer_cb(lv_timer_t * timer)
{
    menu_ui * menu = (menu_ui *)lv_timer_get_user_data(timer);

    if(menu->_menu_key_click) {
        int key = 0;
        menu->_menu_key_click(key);

        if(key == LV_KEY_UP) {
            if(menu->_current_page > 0) {
                menu->_current_page--;
            }
        } else if(key == LV_KEY_DOWN) {
            if(menu->_current_page < menu->_menu_config->page_cnt - 1) {
                menu->_current_page++;
            }
        }

    }

}

menu_ui::menu_ui(lv_obj_t *parent, menu_config_t * config)
{
    _menu_ui.parent = parent;
    _menu_config = config;
    _current_page = 0;

    _menu_ui.x = 0;
    _menu_ui.y = STATUS_VIEW_HIGHT;
    _menu_ui.width = MENU_VIEW_WIDTH;
    _menu_ui.hight = MENU_VIEW_HEIGHT;

    _menu_ui.title = lv_obj_create(_menu_ui.parent);
    lv_obj_set_size(_menu_ui.title, _menu_ui.width, 36);
    lv_obj_set_pos(_menu_ui.title, 0, 24);

    lv_obj_set_style_outline_width(_menu_ui.title, 0, 0);
    lv_obj_set_style_outline_pad(_menu_ui.title, 0, 0);
    lv_obj_set_style_border_width(_menu_ui.title, 0, 0);
    lv_obj_set_style_radius(_menu_ui.title, 0, 0);
    lv_obj_set_scrollbar_mode(_menu_ui.title, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_bg_color(_menu_ui.title, lv_color_hex(MENU_TITLE_BG_COLOR), 0);

    _menu_ui.title_txt = lv_label_create(_menu_ui.title);
    lv_obj_align(_menu_ui.title_txt, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_style_text_font(_menu_ui.title_txt, &lv_font_montserrat_20, 0);


    _menu_ui.obj = lv_obj_create(_menu_ui.parent);
    lv_obj_set_size(_menu_ui.obj, _menu_ui.width, 180);
    lv_obj_set_pos(_menu_ui.obj, _menu_ui.x, _menu_ui.y+36);


    lv_obj_set_style_outline_width(_menu_ui.obj, 0, 0);
    lv_obj_set_style_outline_pad(_menu_ui.obj, 0, 0);
    lv_obj_set_style_border_width(_menu_ui.obj, 0, 0);
    lv_obj_set_style_radius(_menu_ui.obj, 0, 0);
    lv_obj_set_scrollbar_mode(_menu_ui.obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_snap_x(_menu_ui.obj, LV_SCROLL_SNAP_NONE);

    /*Create a list*/
    _menu_ui.list1 = lv_list_create(_menu_ui.obj);
    lv_obj_set_size(_menu_ui.list1, _menu_ui.width, 180);
    lv_obj_center(_menu_ui.list1);

    lv_obj_set_style_radius(_menu_ui.list1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    lv_obj_set_scrollbar_mode(_menu_ui.list1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(_menu_ui.list1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	lv_obj_set_style_pad_top(_menu_ui.list1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(_menu_ui.list1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(_menu_ui.list1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(_menu_ui.list1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);


    _menu_ui.label_up = lv_label_create(_menu_ui.obj);
    lv_obj_set_size(_menu_ui.label_up, 14, 14);
    lv_obj_align(_menu_ui.label_up, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_label_set_text(_menu_ui.label_up, LV_SYMBOL_UP);
    lv_obj_set_style_text_font(_menu_ui.label_up, &lv_font_montserrat_14, 0);

    _menu_ui.label_down = lv_label_create(_menu_ui.obj);
    lv_obj_set_size(_menu_ui.label_down, 14, 14);
    lv_obj_align(_menu_ui.label_down, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_label_set_text(_menu_ui.label_down, LV_SYMBOL_DOWN);
    lv_obj_set_style_text_font(_menu_ui.label_down, &lv_font_montserrat_14, 0);

    menu_show();

    _menu_timer = lv_timer_create(menu_timer_cb, 100, this);
    lv_timer_ready(_menu_timer);
}

menu_ui::~menu_ui()
{
    if(_menu_ui.title) lv_obj_del(_menu_ui.title);
    if(_menu_ui.title_txt) lv_obj_del(_menu_ui.title_txt);
    if(_menu_ui.obj) lv_obj_del(_menu_ui.obj);
    if(_menu_ui.list1) lv_obj_del(_menu_ui.list1);
    if(_menu_ui.label_up) lv_obj_del(_menu_ui.label_up);
    if(_menu_ui.label_down) lv_obj_del(_menu_ui.label_down);

    lv_timer_del(_menu_timer);
}

void menu_ui::set_menu_key_click(std::function<void(int key)> callback)
{
    _menu_key_click = callback;
}

void menu_ui::menu_show()
{
    int page = _current_page;

    lv_obj_t * btn;

    if(page == 0) {
        lv_obj_add_flag(_menu_ui.label_up, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(_menu_ui.label_up, LV_OBJ_FLAG_HIDDEN);
    }

    if(page == _menu_config->page_cnt - 1) {
        lv_obj_add_flag(_menu_ui.label_down, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(_menu_ui.label_down, LV_OBJ_FLAG_HIDDEN);
    }

    lv_label_set_text(_menu_ui.title_txt, _menu_config->pages[page].title.c_str());
    // set_label_txt_center(_ui_menu.title_txt);

    for (size_t i = 0; i < _menu_config->pages[page].items.size(); i++)
    {
        char name[64];
        snprintf(name, sizeof(name), "%d. %s", i+1, _menu_config->pages[page].items[i].name);
        btn = lv_list_add_btn(_menu_ui.list1, NULL, name);
        lv_obj_set_size(btn, _menu_ui.width, 36);
        lv_obj_set_style_text_font(btn, &lv_font_montserrat_24, 0);
    }
}



}
