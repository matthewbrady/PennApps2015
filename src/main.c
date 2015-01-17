#include <pebble.h>
static Window *s_main_window;
static TextLayer *s_time_layer;


static void main_window_load(Window *window) {
  
  //Sets up the text layer in a rectangle
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  
  //Sets up the colors and content of the layer
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "Hi Matt");
  
  //Sets up the font for the layer
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer); //Destroys the layer to free up memory
}

static void init() {
  
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  
  window_stack_push(s_main_window, true);
  
}

static void deinit() {
  
  window_destroy(s_main_window);
  
}



int main(void) {

  init();
  app_event_loop();
  deinit();
  
}