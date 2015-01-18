#include <pebble.h>
#include "view_classes.h"
#include "schedules.h"
  
  
Window *s_main_window;

void view_classes_menu_load(Window *window) {

  
  numClasses = number_stored();
  SimpleMenuItem third_menu_items[numClasses];

  int num_a_items = 0;  
  int i;
  for(i=0; i<SAVE_LIMIT; i++){
    if(persist_exists(i)){
      interval class;
      da
    
      third_menu_items[num_a_items++] = (SimpleMenuItem){
      
      }
    }
  }
  void date_select_menu_load(Window *window) {
  int num_a_items = 0;
  
  second_menu_items[num_a_items++] = (SimpleMenuItem){
    // You should give each menu item a title and callback
    .title = "Monday",
    .callback = menu_select_callback,
  };
  second_menu_items[num_a_items++] = (SimpleMenuItem){
    // You should give each menu item a title and callback
    .title = "Tuesday",
    .callback = menu_select_callback,
  };
  second_menu_items[num_a_items++] = (SimpleMenuItem){
    // You should give each menu item a title and callback
    .title = "Wednesday",
    .callback = menu_select_callback,
  };

  
  
  
  
  
  
  
  
  
  
  menu_sections[0] = (SimpleMenuSection){
    .num_items = CLASSES_LIST_LENGTH,
    .items = third_menu_items,
  };
  Layer *menu_window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(menu_window_layer);
  
    // Initialize the simple menu layer
  simple_menu_layer3 = simple_menu_layer_create(bounds, window, menu_sections, NUM_MENU_SECTIONS, NULL);

  // Add it to the window for display
  layer_add_child(menu_window_layer, simple_menu_layer_get_layer(simple_menu_layer3));
}
void view_classes_menu_unload(Window *window) {
  simple_menu_layer_destroy(simple_menu_layer3);

  // Cleanup the menu icon
  //gbitmap_destroy(menu_icon_image);
  
}