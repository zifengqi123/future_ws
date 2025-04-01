#include "menu_ui.h"

namespace future_gui {

static void event_handler_key_click(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    menu_event_param_t * param = (menu_event_param_t *)lv_event_get_param(e);
    menu_ui * _menu_t = ((menu_ui*)param->menu_ui);

    if(code == (lv_event_code_t)(_menu_t->_keyboard_event_id)) {
        if(param->key == KEY_DOWN) {
            if(_menu_t->_current_page < _menu_t->_menu_config->page_cnt - 1) {
                _menu_t->_current_page++;
                _menu_t->menu_show();
            }
        } else if(param->key == KEY_UP) {
            if(_menu_t->_current_page > 0) {
                _menu_t->_current_page--;
                _menu_t->menu_show();
            }
        } else if(param->key == KEY_1) {
            if(_menu_t->_menu_config->pages[_menu_t->_current_page].items.size()
                > 0) {
                _menu_t->_menu_config->pages[_menu_t->_current_page].items[0].callback(param->key);
            }
        } else if(param->key == KEY_2) {
            if(_menu_t->_menu_config->pages[_menu_t->_current_page].items.size()
                > 1) {
                _menu_t->_menu_config->pages[_menu_t->_current_page].items[1].callback(param->key);
            }
        } else if(param->key == KEY_3) {
            if(_menu_t->_menu_config->pages[_menu_t->_current_page].items.size()
                > 2) {
                _menu_t->_menu_config->pages[_menu_t->_current_page].items[2].callback(param->key);
            }
        } else if(param->key == KEY_4) {
            if(_menu_t->_menu_config->pages[_menu_t->_current_page].items.size()
                > 3) {
                _menu_t->_menu_config->pages[_menu_t->_current_page].items[3].callback(param->key);
            }
        } else if(param->key == KEY_5) {
            if(_menu_t->_menu_config->pages[_menu_t->_current_page].items.size()
                > 4) {
                _menu_t->_menu_config->pages[_menu_t->_current_page].items[4].callback(param->key);
            }
        } else if (param->key == KEY_CANCEL)
        {
            if (_menu_t->_current_page != 0)
            {
                _menu_t->_current_page = 0;
                _menu_t->menu_show();
            }
            else {  
                if (_menu_t->_cancel_callback)
                {
                    _menu_t->_cancel_callback(_menu_t);
                }
            }
        }
        
    }
}

menu_ui::menu_ui(lv_obj_t *parent, menu_config_t * config, std::function<void(void* data)> cancel_callback)
{
    _menu_ui.parent = parent;
    _menu_config = config;
    _current_page = 0;
    _cancel_callback = cancel_callback;

    _menu_ui.x = 0;
    _menu_ui.y = MENU_STATUS_VIEW_HIGHT;
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

    
    _keyboard_event_id = lv_event_register_id();
    lv_obj_add_event_cb(_menu_ui.parent, event_handler_key_click, (lv_event_code_t)_keyboard_event_id, NULL);

    future::future_devices::getInstance().keyboard_open([this](uint8_t key) {
        printf("on key %d\n",  key);
        
        menu_event_param_t param;
        param.menu_ui = this;
        param.key = key;
        lv_obj_send_event(_menu_ui.parent, (lv_event_code_t)_keyboard_event_id, &param);
    });

    future::future_devices::getInstance().keyboard_set_sound(SDK_KEYBOARD_SOUND_OFF);
    future::future_devices::getInstance().keyboard_set_backlight(SDK_KEYBOARD_BACKLIGHT_ON);
    // future::future_devices::getInstance().keyboard_set_sound(SDK_KEYBOARD_SOUND_ON);

    menu_show();
}

menu_ui::~menu_ui()
{
    future::future_devices::getInstance().keyboard_set_backlight(SDK_KEYBOARD_BACKLIGHT_OFF);
    // future::future_devices::getInstance().keyboard_set_sound(SDK_KEYBOARD_SOUND_OFF);
    future::future_devices::getInstance().keyboard_close();

    if(_menu_ui.title) lv_obj_del(_menu_ui.title);
    if(_menu_ui.title_txt) lv_obj_del(_menu_ui.title_txt);
    if(_menu_ui.obj) lv_obj_del(_menu_ui.obj);
    if(_menu_ui.list1) lv_obj_del(_menu_ui.list1);
    if(_menu_ui.label_up) lv_obj_del(_menu_ui.label_up);
    if(_menu_ui.label_down) lv_obj_del(_menu_ui.label_down);
    
}

void menu_ui::menu_add_pageflag()
{
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

}

void menu_ui::menu_show()
{
    int page = _current_page;

    lv_obj_t * btn;

    


    lv_label_set_text(_menu_ui.title_txt, _menu_config->pages[page].title.c_str());
    // set_label_txt_center(_ui_menu.title_txt);

    if(_menu_ui.list1) lv_obj_del(_menu_ui.list1);

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
    

    for (size_t i = 0; i < _menu_config->pages[page].items.size(); i++)
    {
        char name[64];
        snprintf(name, sizeof(name), "%ld. %s", i+1, _menu_config->pages[page].items[i].name);
        btn = lv_list_add_btn(_menu_ui.list1, NULL, name);
        lv_obj_set_size(btn, _menu_ui.width, 36);
        lv_obj_set_style_text_font(btn, &lv_font_montserrat_24, 0);
    }
    
    menu_add_pageflag();
    if (page == 0)
    {
        lv_obj_add_flag(_menu_ui.label_up, LV_OBJ_FLAG_HIDDEN);

        if (_menu_config->page_cnt > 1)
        {
            lv_obj_clear_flag(_menu_ui.label_down, LV_OBJ_FLAG_HIDDEN);
        }
        else {
            lv_obj_add_flag(_menu_ui.label_down, LV_OBJ_FLAG_HIDDEN);
        }
    }
    else if(page == _menu_config->page_cnt - 1) {
        lv_obj_add_flag(_menu_ui.label_down, LV_OBJ_FLAG_HIDDEN);

        if (_menu_config->page_cnt > 1)
        {
            lv_obj_clear_flag(_menu_ui.label_up, LV_OBJ_FLAG_HIDDEN);
        }
        else {
            lv_obj_add_flag(_menu_ui.label_up, LV_OBJ_FLAG_HIDDEN);
        }
    }
    else {
        lv_obj_clear_flag(_menu_ui.label_up, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(_menu_ui.label_down, LV_OBJ_FLAG_HIDDEN);
    }
}



}
