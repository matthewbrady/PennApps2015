#pragma once
  
#define DAY_LIST_LENGTH 5
#define NUM_MENU_SECTIONS 1
Window *s_main_window;
  
void date_select_menu_load(Window *window);
void date_select_menu_unload(Window *window);

SimpleMenuItem second_menu_items[DAY_LIST_LENGTH];
SimpleMenuLayer *simple_menu_layer;
static void menu_select_callback(int index, void *ctx);
static SimpleMenuSection menu_sections[NUM_MENU_SECTIONS];
