#include <pebble.h>
static Window *s_main_window;
static TextLayer *s_time_layer;
static int secondCount = 0;


//Updates the displayed time --> NOT USING NOW
static void update_time() { 
  
  //Sets up the time container
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //Buffer to hold the time so that the face can reference it
  static char buffer[] = "00:00";
  
  //Write current hours and minutes
  if (clock_is_24h_style() == true) {
    //24h format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  }
  else {
    //12h format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  text_layer_set_text(s_time_layer, buffer);
  
}
//Calls the update function every x seconds based on the units_changed variable
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  //update_time();
  
  //Handling each second
  if (secondCount < 5) {
    //If it hasn't been 5 seconds yet, do nothing
    secondCount++;
  }
  else {
    //If it has been five seconds, vibrate then reset
    vibes_long_pulse();
    vibes_short_pulse();
    secondCount = 0;
  }
}

static void main_window_load(Window *window) {
  
  //Sets up the text layer in a rectangle
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  
  //Sets up the colors and content of the layer
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text_alignment(s_time_layer, GAlignCenter);
  text_layer_set_text(s_time_layer, "Try to stay awake!");
  
  //Sets up the font for the layer
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer); //Destroys the layer to free up memory
}

static void init() {
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler); //Sets the pebble to call the handler every second
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  
  window_stack_push(s_main_window, true);
  //update_time();
  
}

static void deinit() {
  
  window_destroy(s_main_window);
  
}



int main(void) {

  init();
  app_event_loop();
  deinit();
  
}