#include "menu_ui.h"
#include "stdio.h"

static ui_menu_t _menu_ui;


void menu_ui_init(lv_obj_t *parent)
{
    _menu_ui.parent = parent;

    _menu_ui.x_start = 0;
    _menu_ui.y_start = STATUS_VIEW_HIGHT;
    _menu_ui.width = MENU_VIEW_WIDTH;
    _menu_ui.hight = MENU_VIEW_HEIGHT;

    _menu_ui.obj = lv_obj_create(_menu_ui.parent);
    lv_obj_set_size(_menu_ui.obj, _menu_ui.width, _menu_ui.hight);
    lv_obj_set_pos(_menu_ui.obj, _menu_ui.x_start, _menu_ui.y_start);

    lv_obj_set_style_radius(_menu_ui.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(_menu_ui.obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(_menu_ui.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(_menu_ui.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(_menu_ui.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(_menu_ui.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(_menu_ui.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(_menu_ui.obj, lv_color_hex(MENU_ITEM_UNSELECT_BG_COLOR), LV_PART_MAIN|LV_STATE_DEFAULT);



    for (int i = 0; i < MENU_ITEMS_CNT; i++)
    {
        _menu_ui.items[i].item_lines = lv_obj_create(_menu_ui.obj);
        lv_obj_set_size(_menu_ui.items[i].item_lines, _menu_ui.width, MENU_ITEM_TEXT_HEIGHT);
        lv_obj_set_pos(_menu_ui.items[i].item_lines, _menu_ui.x_start, i*MENU_ITEM_TEXT_HEIGHT);
        lv_obj_set_scrollbar_mode(_menu_ui.items[i].item_lines, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_style_radius(_menu_ui.items[i].item_lines, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(_menu_ui.items[i].item_lines, 1, LV_PART_MAIN|LV_STATE_DEFAULT);

        _menu_ui.items[i].item_text = lv_label_create(_menu_ui.items[i].item_lines);
        lv_obj_set_size(_menu_ui.items[i].item_text, _menu_ui.width - MENU_ITEM_TEXT_SIZE, MENU_ITEM_TEXT_SIZE);
        lv_obj_set_style_base_dir(_menu_ui.items[i].item_text, MENU_ITEM_TEXT_DIRSTYLE, 0);
#if LANGUAGE_AL
        lv_obj_align(_menu_ui.items[i].item_text, LV_ALIGN_RIGHT_MID, -3, (MENU_ITEM_TEXT_HEIGHT - MENU_ITEM_TEXT_SIZE)/2);
#else
        lv_obj_align(_menu_ui.items[i].item_text, LV_ALIGN_LEFT_MID, 3, (MENU_ITEM_TEXT_HEIGHT - MENU_ITEM_TEXT_SIZE)/2);
#endif
        lv_obj_set_style_text_align(_menu_ui.items[i].item_text, MENU_ITEM_TEXT_ALIGN, 0);
        lv_obj_set_style_text_font(_menu_ui.items[i].item_text, MENU_ITEM_TEXT_FONT, 0);
        lv_obj_set_scrollbar_mode(_menu_ui.items[i].item_text, LV_SCROLLBAR_MODE_OFF);

        _menu_ui.items[i].is_selected = false;

    }

    _menu_ui.label_up = lv_label_create(_menu_ui.obj);
    lv_obj_set_size(_menu_ui.label_up, MENU_ITEM_TEXT_SIZE, MENU_ITEM_TEXT_SIZE);

    lv_label_set_text(_menu_ui.label_up, LV_SYMBOL_UP);
    lv_obj_set_style_text_font(_menu_ui.label_up, MENU_ITEM_TEXT_FONT, 0);

    _menu_ui.label_down = lv_label_create(_menu_ui.obj);
    lv_obj_set_size(_menu_ui.label_down, MENU_ITEM_TEXT_SIZE, MENU_ITEM_TEXT_SIZE);
    lv_label_set_text(_menu_ui.label_down, LV_SYMBOL_DOWN);
    lv_obj_set_style_text_font(_menu_ui.label_down, MENU_ITEM_TEXT_FONT, 0);

#if LANGUAGE_AL
    lv_obj_align(_menu_ui.label_up, LV_ALIGN_TOP_LEFT, 10, 0);
    lv_obj_align(_menu_ui.label_down, LV_ALIGN_BOTTOM_LEFT, 10, 0);

#else
    lv_obj_align(_menu_ui.label_up, LV_ALIGN_TOP_RIGHT, -10, 0);
    lv_obj_align(_menu_ui.label_down, LV_ALIGN_BOTTOM_RIGHT, -10, 0);
#endif

    menu_ui_show_pageflag(false, false);

}

void menu_ui_del()
{
    menu_ui_hide();

    for (int i = 0; i < MENU_ITEMS_CNT; i++)
    {
        if(_menu_ui.items[i].item_lines) lv_obj_del(_menu_ui.items[i].item_lines);
        if(_menu_ui.items[i].item_text) lv_obj_del(_menu_ui.items[i].item_text);

    }

    if(_menu_ui.label_up) lv_obj_del(_menu_ui.label_up);
    if(_menu_ui.label_down) lv_obj_del(_menu_ui.label_down);

    if(_menu_ui.obj) lv_obj_del(_menu_ui.obj);

}

void menu_ui_show_pageflag(bool has_up, bool has_down)
{
    if (has_up)
    {
        lv_obj_clear_flag(_menu_ui.label_up, LV_OBJ_FLAG_HIDDEN);
    }
    else {
        lv_obj_add_flag(_menu_ui.label_up, LV_OBJ_FLAG_HIDDEN);
    }

    if (has_down)
    {
        lv_obj_clear_flag(_menu_ui.label_down, LV_OBJ_FLAG_HIDDEN);
    }
    else {
        lv_obj_add_flag(_menu_ui.label_down, LV_OBJ_FLAG_HIDDEN);
    }
}

void menu_ui_show(char items[6][64], int cnt)
{
    for (int i = 0; i < MENU_ITEMS_CNT; i++)
    {
        if(i < cnt) {
            sprintf(_menu_ui.items[i].str_text, "%s", items[i]);
            lv_label_set_text(_menu_ui.items[i].item_text, _menu_ui.items[i].str_text);

            lv_obj_clear_flag(_menu_ui.items[i].item_lines, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(_menu_ui.items[i].item_text, LV_OBJ_FLAG_HIDDEN);
        }
        else {
            lv_obj_add_flag(_menu_ui.items[i].item_lines, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(_menu_ui.items[i].item_text, LV_OBJ_FLAG_HIDDEN);
        }

    }
}

void menu_ui_hide()
{
    for (int i = 0; i < MENU_ITEMS_CNT; i++)
    {
        lv_obj_add_flag(_menu_ui.items[i].item_lines, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(_menu_ui.items[i].item_text, LV_OBJ_FLAG_HIDDEN);
    }
    lv_obj_add_flag(_menu_ui.label_up, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(_menu_ui.label_down, LV_OBJ_FLAG_HIDDEN);
}

void menu_ui_set_select(int idex)
{
    //select one only
    for (int i = 0; i < MENU_ITEMS_CNT; i++)
    {
        if (i+1 == idex)
        {
            _menu_ui.items[i].is_selected = true;
            lv_obj_set_style_bg_color(_menu_ui.items[i].item_lines, lv_color_hex(MENU_ITEM_SELECT_BG_COLOR), LV_PART_MAIN|LV_STATE_DEFAULT);

        }
        else {
            _menu_ui.items[i].is_selected = false;
            lv_obj_set_style_bg_color(_menu_ui.items[i].item_lines, lv_color_hex(MENU_ITEM_UNSELECT_BG_COLOR), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
    }
}
