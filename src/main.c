#include <pebble.h>
static Window *s_main_window;
static TextLayer *s_time_layer;
static int secondCount = 0;
static int x_accel = 0, y_accel = 0, z_accel = 0;
static bool asleep = false;


static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  
  x_accel = data[0].x;
  y_accel = data[0].y;
  z_accel = data[0].z;
  
}

//Calls the update function every x seconds based on the units_changed variable
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  
  static char s_buffer[128];
  
  //Handling each second
  if (secondCount < 5) {
    //If it hasn't been 5 seconds yet, just print the x-acceleration
    secondCount++;
    // Compose string of all data
    snprintf(s_buffer, sizeof(s_buffer), 
      "X: %d Y: %d Z: %d", x_accel, y_accel, z_accel);
    text_layer_set_text(s_time_layer, s_buffer);
  }
  else {
    //If it has been five seconds, vibrate then reset
    /*vibes_long_pulse();
    vibes_short_pulse();*/
    secondCount = 0;
  }
}

static void main_window_load(Window *window) {
  
  //Sets up the text layer in a rectangle
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  
  //Sets up the colors and content of the layer
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text(s_time_layer, "Try to stay awake!");
  
  //Sets up the font for the layer
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer); //Destroys the layer to free up memory
}

static void init() {
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler); //Sets the pebble to call the handler every second
  
  /*Sets up the accelerometer
    -Takes one sample (num_samples) every time data is collected
    -Gives the data to the data_handler function
    -Sets sampling rate to 10Hz
  */
  int num_samples = 1;
  accel_data_service_subscribe(num_samples, data_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
  
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