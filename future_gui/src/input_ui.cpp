#include "input_ui.h"

namespace future_gui
{

input_ui::input_ui(lv_obj_t *parent)
{
    ui_input_init(parent, 0, INPUT_STATUS_VIEW_HIGHT, INPUT_VIEW_WIDTH, INPUT_VIEW_HIGHT);
}

input_ui::~input_ui()
{
    ui_input_del();
}

void input_ui::ui_input_init(lv_obj_t *parent, int x, int y, int width, int hight)
{
    _ui_input.parent = parent;
    _ui_input.x = x;
    _ui_input.y = y;
    _ui_input.width = width;
    _ui_input.hight = hight;


    _ui_input.obj = lv_label_create(_ui_input.parent);
    lv_obj_set_size(_ui_input.obj, _ui_input.width, _ui_input.hight);
    lv_obj_set_pos(_ui_input.obj, _ui_input.x, _ui_input.y);


    lv_obj_set_style_outline_width(_ui_input.obj, 0, 0);
    lv_obj_set_style_outline_pad(_ui_input.obj, 0, 0);
    lv_obj_set_style_border_width(_ui_input.obj, 0, 0);
    lv_obj_set_style_radius(_ui_input.obj, 0, 0);
    lv_obj_set_scrollbar_mode(_ui_input.obj, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(_ui_input.obj, "");

    _ui_input.title = lv_label_create(_ui_input.obj);
    lv_obj_set_size(_ui_input.title, _ui_input.width, 24);
    lv_obj_set_pos(_ui_input.title, 24, 24);
    lv_obj_set_style_text_font(_ui_input.title, &lv_font_montserrat_14, 0);
    // lv_obj_set_style_text_color(_ui_input.title, lv_color_hex(0x0000AA), 0);


    _ui_input.input = lv_textarea_create(_ui_input.obj);
    lv_textarea_set_one_line(_ui_input.input, true);
    lv_obj_align(_ui_input.input, LV_ALIGN_TOP_MID, _ui_input.x, _ui_input.y + 36);

        lv_obj_add_state(_ui_input.input, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/

}

void input_ui::ui_input_del()
{
    lv_obj_del(_ui_input.obj);
}

void input_ui::ui_input_show(char * title, char* msg)
{
    lv_label_set_text(_ui_input.title, title);
    lv_textarea_set_text(_ui_input.input, msg);

    lv_obj_set_style_base_dir(_ui_input.input, LV_BASE_DIR_AUTO, 0);
    lv_obj_set_style_text_font(_ui_input.input, &lv_font_montserrat_14, 0);
}



}
