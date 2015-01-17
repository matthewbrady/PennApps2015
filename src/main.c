#include <pebble.h>
static Window *s_main_window;
static TextLayer *s_time_layer;
static int secondCount = 0;
static int x_accel = 0, y_accel = 0, z_accel = 0;
static int x_prev = 0, y_prev = 0, z_prev = 0;
static bool buzz = false;

//Tracks whether the watch is moving or not
//Sensitivity should be updated after testing
bool moving() { 
  
  //Checks to see if watch has moved by 50 units on any axis
  if ((x_accel >= x_prev + 50) || (x_accel <= x_prev - 50) ||
        (y_accel >= y_prev + 50) || (y_accel <= y_prev - 50) ||
        (z_accel >= z_prev + 50) || (z_accel <= z_prev - 50)) {
    //If yes, we know the watch was moving
    return true;
  }
  else {
    //If not, it was stationary
    return false;
  }

}

bool keepBuzzing() {
  //Checks to see if watch has moved by 2000 units on any axis (user flicks after being woken up)
  if ((x_accel >= x_prev + 1000) || (x_accel <= x_prev - 1000) ||
        (y_accel >= y_prev + 1000) || (y_accel <= y_prev - 1000) ||
        (z_accel >= z_prev + 1000) || (z_accel <= z_prev - 1000)) {
    //If yes, no need to keep buzzing
    return false;
  }
  else {
    //If no, keep going
    return true;
  }
}

static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  
  if (x_accel == 0 && y_accel == 0 && z_accel == 0) {
    x_accel = data[0].x;    //If this is the first recording, then just record the first values
    y_accel = data[0].y;
    z_accel = data[0].z;
  }
  else {
    
    x_prev = x_accel;    //Holding these values to compare to new ones in the timeticker
    y_prev = y_accel;
    z_prev = z_accel;
    
    x_accel = data[0].x;
    y_accel = data[0].y;
    z_accel = data[0].z;
  }
  
  x_accel = data[0].x;
  y_accel = data[0].y;
  z_accel = data[0].z;
  
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_time_layer, "Up pressed!");
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_time_layer, "Select pressed!");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_time_layer, "Down pressed!");
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}


//Calls the update function every x seconds based on the units_changed variable
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  
  static char s_buffer[128];
  
  //Handling each second
  if (moving()) {
      text_layer_set_text(s_time_layer, "Good job!");
      psleep(5000); //Sleeps the watch for 5 seconds
      secondCount = 0; //Resets the counter for a fresh start
  } 
  else {
    //If it has been five seconds, give a warning
    if (secondCount == 5) {
      text_layer_set_text(s_time_layer, "Feeling tired?");
    }
    else if (secondCount == 10) {
      text_layer_set_text(s_time_layer, "Asleep?");
    }
    else if (secondCount == 20) {
      text_layer_set_text(s_time_layer, "Buzzer coming in 10...");
    }
    else if (secondCount >= 30) {
      buzz = true;
      while (buzz) {
        vibes_long_pulse();
        vibes_short_pulse();
        psleep(1000);
        buzz = keepBuzzing();
      }
    }
    secondCount++;
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
  //window_set_click_config_provider(s_main_window, click_config_provider);
  
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