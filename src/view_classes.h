#pragma once
  
//#define CLASSES_LIST_LENGTH 10
#define NUM_MENU_SECTIONS 1
  
Window *s_main_window;

void view_classes_menu_load(Window *window);
void view_classes_menu_unload(Window *window);

//
SimpleMenuLayer *simple_menu_layer3;
static SimpleMenuSection menu_sections[NUM_MENU_SECTIONS];

void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context);
uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context);