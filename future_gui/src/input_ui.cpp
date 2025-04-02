#include "input_ui.h"

extern "C" {

static ui_input_t _ui_input;


void input_ui_init(lv_obj_t *parent)
{
    _ui_input.parent = parent;
    _ui_input.x_start = 0;
    _ui_input.y_start = STATUS_VIEW_HIGHT;
    _ui_input.width = INPUT_VIEW_WIDTH;
    _ui_input.hight = INPUT_VIEW_HIGHT;


    _ui_input.obj = lv_obj_create(_ui_input.parent);
    lv_obj_set_size(_ui_input.obj, _ui_input.width, _ui_input.hight);
    lv_obj_set_pos(_ui_input.obj, _ui_input.x_start, _ui_input.y_start);

    lv_obj_set_style_radius(_ui_input.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(_ui_input.obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(_ui_input.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(_ui_input.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(_ui_input.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(_ui_input.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(_ui_input.obj, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(_ui_input.obj, lv_color_hex(MENU_ITEM_UNSELECT_BG_COLOR), LV_PART_MAIN|LV_STATE_DEFAULT);


    lv_obj_set_style_outline_width(_ui_input.obj, 0, 0);
    lv_obj_set_style_outline_pad(_ui_input.obj, 0, 0);
    lv_obj_set_style_border_width(_ui_input.obj, 0, 0);
    lv_obj_set_style_radius(_ui_input.obj, 0, 0);
    lv_obj_set_scrollbar_mode(_ui_input.obj, LV_SCROLLBAR_MODE_OFF);



    _ui_input.title = lv_label_create(_ui_input.obj);
    lv_obj_set_size(_ui_input.title, _ui_input.width - MENU_ITEM_TEXT_SIZE*2, MENU_ITEM_TEXT_SIZE);
#if LANGUAGE_AL
    lv_obj_align(_ui_input.title, LV_ALIGN_RIGHT_MID, -MENU_ITEM_TEXT_SIZE, -MENU_ITEM_TEXT_HEIGHT - 10);
#else
    lv_obj_align(_ui_input.title, LV_ALIGN_LEFT_MID, MENU_ITEM_TEXT_SIZE, -MENU_ITEM_TEXT_HEIGHT - 10);
#endif
    lv_obj_set_style_text_align(_ui_input.title, MENU_ITEM_TEXT_ALIGN, 0);
    lv_obj_set_style_base_dir(_ui_input.title, MENU_ITEM_TEXT_DIRSTYLE, 0);
    lv_obj_set_style_text_font(_ui_input.title, MENU_ITEM_TEXT_FONT, 0);

    _ui_input.input = lv_textarea_create(_ui_input.obj);
    lv_obj_set_size(_ui_input.input, _ui_input.width - MENU_ITEM_TEXT_SIZE*2, MENU_ITEM_TEXT_HEIGHT);
    lv_textarea_set_one_line(_ui_input.input, true);

    lv_obj_add_state(_ui_input.input, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/
    lv_obj_set_style_base_dir(_ui_input.input, MENU_ITEM_TEXT_DIRSTYLE, 0);

#if LANGUAGE_AL
    lv_obj_align(_ui_input.input, LV_ALIGN_RIGHT_MID, -MENU_ITEM_TEXT_SIZE, 0);
#else
    lv_obj_align(_ui_input.input, LV_ALIGN_LEFT_MID, MENU_ITEM_TEXT_SIZE, 0);
#endif
    lv_obj_set_style_text_align(_ui_input.input, MENU_ITEM_TEXT_ALIGN, 0);
    lv_obj_set_style_text_font(_ui_input.input, MENU_ITEM_TEXT_FONT, 0);


}

void input_ui_del()
{
    if(_ui_input.title) lv_obj_del(_ui_input.title);
    if(_ui_input.input) lv_obj_del(_ui_input.input);

    if(_ui_input.obj) lv_obj_del(_ui_input.obj);
}

void input_ui_show(char * title, char* msg)
{
    lv_label_set_text(_ui_input.title, title);
    lv_textarea_set_text(_ui_input.input, msg);
}

}
