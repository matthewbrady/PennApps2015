#include <pebble.h>
#include "time_select_menu.h"
  
static TextLayer *s_hour_layer;
static TextLayer *s_minute_layer;
Window *time_select_menu;
int hours = 12;
int minutes = 0;
char hour_buffer[128];
char min_buffer[128];
bool edit_hours = true;

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  snprintf(hour_buffer, 128, "%d", hours);
  
  text_layer_set_text(s_hour_layer, hour_buffer);
  if (edit_hours) {
    if (hours < 24) {
      hours++;
    }
    else if (hours == 24) {
      hours = 0;
    }
  }
  else {
    if (minutes < 60) {
      minutes++;
    }
    else if (minutes == 60) {
      minutes = 0;
    }
  }
  
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  snprintf(hour_buffer, 128, "%d", hours);
  
  text_layer_set_text(s_hour_layer, hour_buffer);
  if (hours > 0) {
    hours--;
  }
  else if (hours == 0) {
    hours = 24;
  }
}

static void click_config_provider(void *context) {
  
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  
}
  
void time_select_window_load(Window *window) {
  
  window_set_click_config_provider(window, click_config_provider);
  
  //char min_buffer[128];
  
  
  //snprintf(min_buffer, 128, "%d", minutes);
  
  
  //Sets up the colors and content of the layer
  s_hour_layer= text_layer_create(GRect(0, 55, 100, 50));
  s_minute_layer = text_layer_create(GRect(20, 55, 144, 50));
  
  text_layer_set_background_color(s_hour_layer, GColorClear);
  text_layer_set_text_color(s_hour_layer, GColorBlack);
  text_layer_set_text_alignment(s_hour_layer, GTextAlignmentCenter);
  
  text_layer_set_background_color(s_minute_layer, GColorClear);
  text_layer_set_text_color(s_minute_layer, GColorBlack);
  text_layer_set_text_alignment(s_minute_layer, GTextAlignmentCenter);
  
  text_layer_set_text(s_hour_layer, "12");
  //text_layer_set_text(s_minute_layer, min_buffer);
  
  //Sets up the font for the layer
  text_layer_set_font(s_hour_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  //text_layer_set_font(s_minute_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  
  //Adds the layer to the window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour_layer));
  //layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_minute_layer));
}

void time_select_window_unload(Window *window) {
  text_layer_destroy(s_hour_layer); //Destroys the layer to free up memory
  window_destroy(time_select_window);
}