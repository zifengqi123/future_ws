#pragma once

#include "lvgl/lvgl.h"

namespace future_gui
{

typedef struct ui_input
{
    lv_obj_t* parent;
    lv_obj_t* obj;
    lv_obj_t* title;
    lv_obj_t* input;

    int width;
    int hight;

    int x;
    int y;

}ui_input_t;

#define INPUT_VIEW_WIDTH    (320)
#define INPUT_STATUS_VIEW_HIGHT   (24)
#define INPUT_VIEW_HIGHT    (240 - INPUT_STATUS_VIEW_HIGHT)

class input_ui
{
public:
    input_ui(lv_obj_t *parent);
    ~input_ui();

    void ui_input_init(lv_obj_t *parent, int x, int y, int width, int hight);
    void ui_input_del();

    void ui_input_show(char * title, char* msg);

private:
    ui_input_t _ui_input;
};

}
