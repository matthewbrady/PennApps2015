#include <pebble.h>
#include "time_select_menu.h"
#include "date_select_menu.h"
#include "schedules.h"
  
static TextLayer *s_banner_layer;
static TextLayer *s_hour_layer;
static TextLayer *s_minute_layer;
static TextLayer *s_colon_layer;
Window *time_select_menu;

int hours = 12;
int minutes = 30;
char hour_buffer[128];
char min_buffer[128];
bool edit_hours = true;

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  if (edit_hours) {
    if (hours < 24) {
      hours++;
    }
    else {
      hours = 0;
    }
  }
  else {
    if (minutes < 60) {
      minutes++;
    }
    else {
      minutes = 0;
    }
  }
  snprintf(hour_buffer, 128, "%d", hours);
  text_layer_set_text(s_hour_layer, hour_buffer);
  snprintf(min_buffer, 128, "%02d", minutes);
  text_layer_set_text(s_minute_layer, min_buffer);
  
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (edit_hours) {
    if (hours > 0) {
      hours--;
    }
    else {
      hours = 23;
    }
  }
  else {
    if (minutes > 0) {
      minutes--;
    }
    else {
      minutes = 59;
    } 
  }
  
  snprintf(hour_buffer, 128, "%d", hours);
  text_layer_set_text(s_hour_layer, hour_buffer);
  snprintf(min_buffer, 128, "%02d", minutes);
  text_layer_set_text(s_minute_layer, min_buffer);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (edit_hours) {
    edit_hours = !edit_hours;
    text_layer_set_text(s_banner_layer, "Now the minutes:");
  }
  else if (!edit_hours) {
    edit_hours = true;
    interval to_save;
    if (weekDay == 6) {
      to_save.day = 1;//Offset by to account for differences in start of the week
    } 
    else {
      to_save.day = weekDay + 1;
    }
    to_save.begin_time_mins = hours*60 + minutes;
    
    interval_write(to_save);
    schedule_all();
    
    window_stack_pop(true /* Animated */);
    window_stack_pop(true /* Animated */);
    
  }
}
static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (!edit_hours) {
      edit_hours = !edit_hours;
      text_layer_set_text(s_banner_layer, "Choose the hour:");
    } else {
      window_stack_pop(true /* Animated */);
  }
}

static void click_config_provider(void *context) {
  
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
  
}
  
void time_select_window_load(Window *window) {
  
  char temp[128];
  snprintf(temp, 128, "%d", weekDay);
  APP_LOG(APP_LOG_LEVEL_DEBUG, temp);
  
  window_set_click_config_provider(window, click_config_provider);
  
  //char min_buffer[128];
  
  
  //snprintf(min_buffer, 128, "%d", minutes);
  
  
  //Sets up the colors and content of the layer
  s_banner_layer = text_layer_create(GRect(5, 5, 150, 100));
  s_hour_layer= text_layer_create(GRect(10, 55, 100, 50));
  s_colon_layer = text_layer_create(GRect(25, 54, 100, 50));
  s_minute_layer = text_layer_create(GRect(15, 55, 144, 50));
  
  text_layer_set_background_color(s_banner_layer, GColorClear);
  text_layer_set_background_color(s_hour_layer, GColorClear);
  text_layer_set_text_color(s_hour_layer, GColorBlack);
  text_layer_set_text_alignment(s_hour_layer, GTextAlignmentCenter);
  
  text_layer_set_background_color(s_banner_layer, GColorClear);
  text_layer_set_background_color(s_minute_layer, GColorClear);
  text_layer_set_text_color(s_minute_layer, GColorBlack);
  text_layer_set_text_alignment(s_minute_layer, GTextAlignmentCenter);
  
  text_layer_set_background_color(s_banner_layer, GColorClear);
  text_layer_set_background_color(s_colon_layer, GColorClear);
  text_layer_set_text_color(s_colon_layer, GColorBlack);
  text_layer_set_text_alignment(s_colon_layer, GTextAlignmentCenter);
  
  snprintf(hour_buffer, 128, "%d", hours);
  text_layer_set_text(s_hour_layer, hour_buffer);
  text_layer_set_text(s_colon_layer, ":");
  snprintf(min_buffer, 128, "%d", minutes);
  text_layer_set_text(s_minute_layer, min_buffer);
  text_layer_set_text(s_banner_layer, "Choose the hour:");
  
  //Sets up the font for the layer
  text_layer_set_font(s_hour_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_font(s_minute_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_font(s_colon_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_font(s_banner_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  
  //Adds the layer to the window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_minute_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_colon_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_banner_layer));
}

void time_select_window_unload(Window *window) {
  text_layer_destroy(s_hour_layer); //Destroys the layer to free up memory
  window_destroy(time_select_window);
}