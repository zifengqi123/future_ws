#pragma once

#include "lvgl/lvgl.h"

using namespace std;


namespace future_gui
{

#define NOTICE_VIEW_WIDTH       (320)
#define STATUS_VIEW_HIGHT       (24)
#define NOTICE_VIEW_HIGHT       (240 - STATUS_VIEW_HIGHT)

#define NOTICE_VIEW_BG_COLOR  0x00A0FF

typedef struct ui_notice
{
    lv_obj_t* parent;
    lv_obj_t* obj;
    lv_obj_t* title;
    lv_obj_t* title_txt;
    lv_obj_t* label;

    int width;
    int hight;

    int x;
    int y;

}ui_notice_t;

class notice_ui
{
public:
    notice_ui(lv_obj_t *parent);
    ~notice_ui();

    void notice_init(lv_obj_t *parent, int x, int y, int width, int hight);
    void notice_del();

    void notice_show(const char* title, const char* text);

    ui_notice_t _notice_ui;
};

}
