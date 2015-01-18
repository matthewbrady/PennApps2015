#pragma once
  
//#define CLASSES_LIST_LENGTH 10
#define NUM_MENU_SECTIONS 1
  
Window *s_main_window;

void view_classes_menu_load(Window *window);
void view_classes_menu_unload(Window *window);

//
SimpleMenuLayer *simple_menu_layer3;
static SimpleMenuSection menu_sections[NUM_MENU_SECTIONS];