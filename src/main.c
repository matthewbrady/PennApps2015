#include <pebble.h>

#define NUM_MENU_SECTIONS 1
#define NUM_MENU_ITEMS 2

enum App_State {
  awake,
  level1,
  level2,
  level3,
  ringing    
};
  
static Window *s_menu_window;
static SimpleMenuLayer *simple_menu_layer;
static SimpleMenuSection menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem first_menu_items[NUM_MENU_ITEMS];

static Window *s_main_window;
static TextLayer *s_time_layer;
static int secondCount = 0; //Keeps track of how many seconds since user has moved
static int x_accel = 0, y_accel = 0, z_accel = 0; //Keeps track of most recent accelerometer reading
static int x_prev = 0, y_prev = 0, z_prev = 0;    //2nd most recent accelerometer reading
static int sleep_threshold[4] = {5, 10, 20, 30};

static void main_window_load(Window *window);
static void menu_select_callback(int index, void *ctx);
static void main_window_unload(Window *window);




// This initializes the menu upon window load
static void menu_window_load(Window *window) {
  // We'll have to load the icon before we can use it
  //menu_icon_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON1);

  // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items later
  int num_a_items = 0;

  // This is an example of how you'd set a simple menu item
  first_menu_items[num_a_items++] = (SimpleMenuItem){
    // You should give each menu item a title and callback
    .title = "Stay Awake!",
    .callback = menu_select_callback,
  };
  // The menu items appear in the order saved in the menu items array
  first_menu_items[num_a_items++] = (SimpleMenuItem){
    .title = "Scheduled Classes",
    // You can also give menu items a subtitle
    .subtitle = "Here's a subtitle",
    .callback = menu_select_callback,
  };

  // Bind the menu items to the corresponding menu sections
  menu_sections[0] = (SimpleMenuSection){
    .num_items = NUM_MENU_ITEMS,
    .items = first_menu_items,
  };


  // Now we prepare to initialize the simple menu layer
  // We need the bounds to specify the simple menu layer's viewport size
  // In this case, it'll be the same as the window's
  Layer *menu_window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(menu_window_layer);

  // Initialize the simple menu layer
  simple_menu_layer = simple_menu_layer_create(bounds, window, menu_sections, NUM_MENU_SECTIONS, NULL);

  // Add it to the window for display
  layer_add_child(menu_window_layer, simple_menu_layer_get_layer(simple_menu_layer));
}

void menu_window_unload(Window *window) {
  simple_menu_layer_destroy(simple_menu_layer);

  // Cleanup the menu icon
  //gbitmap_destroy(menu_icon_image);
}

//Tracks whether the watch is moving or not
//Sensitivity should be updated after testing
bool moving(int sensitivity) { 
  
  //Checks to see if watch has moved by 50 units on any axis
  if ((x_accel >= x_prev + sensitivity) || (x_accel <= x_prev - sensitivity) ||
        (y_accel >= y_prev + sensitivity) || (y_accel <= y_prev - sensitivity) ||
        (z_accel >= z_prev + sensitivity) || (z_accel <= z_prev - sensitivity)) {
    //If yes, we know the watch was moving
    return true;
  }
  else {
    //If not, it was stationary
    return false;
  }

}

enum App_State get_state(int seconds){
  if (seconds>=sleep_threshold[3]){
    return ringing;
  }
  else if (seconds>=sleep_threshold[2]){
    return level3;
  }
  else if (seconds>=sleep_threshold[1]){
    return level2;
  }
  else if (seconds>=sleep_threshold[0]){
    return level1;
  }
  else return awake;
}

static void menu_select_callback(int index, void *ctx) {
  if (index == 0){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "NOW WE WILL BEGIN");
    //start the new window and push it 
    s_main_window = window_create();
  
    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = main_window_load,
      .unload = main_window_unload
    });
    
    window_stack_push(s_main_window, true);
    
  }
  else {
    first_menu_items[index].subtitle = "You've hit select here!";
    layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer));
  }
  
  /*// Here we just change the subtitle to a literal string
  first_menu_items[index].subtitle = "You've hit select here!";
  // Mark the layer to be updated
  layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer));*/
}

static void data_handler(AccelData *data, uint32_t num_samples) {
  /*Needed to get the data from the accelerometer
  The 'data' variable has everything we need and has attributes x y and z
  'num_samples' is how many sample we want for each cycle of sampling ("We're at 10Hz")
  Only using one to conserve memory/battery*/
  
  //This block stops the pebble from vibrating if shaken
  enum App_State sleep_state = get_state(secondCount);
  if (sleep_state == ringing && moving(200)) {
    secondCount = 0;
    sleep_state = awake;
  }
  
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
  
}

//Calls the function every x where x is units_changed variable in the init() function (using seconds for now)
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  static char buff[128];
  enum App_State sleep_state = get_state(secondCount);
  
  if (moving(50) && (sleep_state != ringing)){
    secondCount = 0;
    sleep_state = awake;
  }
  
  switch (sleep_state) {
    case awake:
      text_layer_set_text(s_time_layer, "Good job!");
      secondCount++;//note:this is a special case because it sleeps for the entire interval, essentially waiting for level1
      break;
    case level1:
      text_layer_set_text(s_time_layer, "Feeling tired?");
      secondCount++;
      break;
    case level2:
      text_layer_set_text(s_time_layer, "Asleep?");
      secondCount++;
      break;
    case level3:
      snprintf(buff, sizeof(buff), "Buzzer coming in %d...", 30-secondCount);
      text_layer_set_text(s_time_layer, buff);
      secondCount++;
      break;
    case ringing:
      text_layer_set_text(s_time_layer, "Wakey wakey!");
      vibes_long_pulse();
      secondCount++;
      break;
  }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  enum App_State sleep_state = get_state(secondCount);
  if (sleep_state == ringing) {
    secondCount = 0;
    text_layer_set_text(s_time_layer, "Try to stay awake!");
  }
}

static void click_config_provider(void *context) {
  
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  
}




static void main_window_load(Window *window) {
  
  window_set_click_config_provider(window, click_config_provider);

  tick_timer_service_subscribe(SECOND_UNIT, tick_handler); //Sets the pebble to call the handler every second
  
  int num_samples = 10;
  accel_data_service_subscribe(num_samples, data_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
  //Sets up the text layer in a rectangle
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  
  //Sets up the colors and content of the layer
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text(s_time_layer, "Try to stay awake!");
  
  //Sets up the font for the layer
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  
  //Adds the layer to the window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer); //Destroys the layer to free up memory
  window_destroy(s_main_window);
}

static void init() {
  s_menu_window = window_create();

  // Setup the window handlers
  window_set_window_handlers(s_menu_window, (WindowHandlers) {
    .load = menu_window_load,
    .unload = menu_window_unload,
  });

  window_stack_push(s_menu_window, true /* Animated */);
 
  
  //tick_timer_service_subscribe(SECOND_UNIT, tick_handler); //Sets the pebble to call the handler every second
  //window_set_click_config_provider(s_main_window, click_config_provider);
  
  /*Sets up the accelerometer
    -Takes ten samples (num_samples) every time data is collected
    -Gives the data to the data_handler function
    -Sets sampling rate to 10Hz
  */
  /*
  int num_samples = 10;
  accel_data_service_subscribe(num_samples, data_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
  
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_set_click_config_provider(s_main_window, click_config_provider);
  
  window_stack_push(s_main_window, true);*/
}

static void deinit() {
  window_destroy(s_menu_window);
  //window_destroy(s_main_window);
}

int main(void) {

  init();
  app_event_loop();
  deinit();
  
}