#include <pebble.h>
#include "time_select_menu.h"
  
static TextLayer *s_date_layer;
Window *time_select_menu;
  
void time_select_window_load(Window *window) {
  
  //Sets up the colors and content of the layer
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  text_layer_set_text(s_date_layer, "Made it!");
  
  //Sets up the font for the layer
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  
  //Adds the layer to the window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
}

void time_select_window_unload(Window *window) {
  text_layer_destroy(s_date_layer); //Destroys the layer to free up memory
  window_destroy(time_select_window);
}