#pragma once

#include "lvgl/lvgl.h"
#include <functional>
#include <vector>
#include <string>
#include "future_devices.h"

using namespace std;

namespace future_gui {

typedef struct ui_menu
{
    lv_obj_t* parent;
    lv_obj_t* title;
    lv_obj_t* title_txt;
    lv_obj_t* obj;
    lv_obj_t* list1;

    lv_obj_t* label_up;
    lv_obj_t* label_down;


    int width;
    int hight;

    int x;
    int y;

    int cnt_perv;
}ui_menu_t;

typedef struct menu_item
{
    int item;
    char name[32];
    std::function<void(int item)> callback;
}menu_item_t;

typedef struct menu_page
{
    int page;
    string title;
    vector<menu_item_t> items;
}menu_page_t;

typedef struct menu_config
{
    int page_cnt;
    vector<menu_page_t> pages;
}menu_config_t;

#define MENU_VIEW_WIDTH     (320)
#define MENU_STATUS_VIEW_HIGHT   (24)
#define MENU_VIEW_HEIGHT    (240 - MENU_STATUS_VIEW_HIGHT)

#define MENU_TITLE_BG_COLOR  0x00A0FF

typedef struct menu_event_param
{
    void* menu_ui;
    int key;
}menu_event_param_t;


class menu_ui {

public:
    menu_ui(lv_obj_t *parent, menu_config_t * config, std::function<void(void* data)> cancel_callback);
    ~menu_ui();

    void menu_show();
    

public:
    ui_menu_t _menu_ui;
    menu_config_t* _menu_config;
    int _current_page = 0;
    void menu_add_pageflag();

    uint32_t _keyboard_event_id;

    std::function<void(void* data)> _cancel_callback;

};

}
