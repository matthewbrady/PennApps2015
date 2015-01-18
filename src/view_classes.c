#include <pebble.h>
#include "view_classes.h"
#include "schedules.h"
  
  
Window *s_main_window;




void view_classes_menu_load(Window *window) {

  int numClasses = number_stored();
  SimpleMenuItem third_menu_items[numClasses];

  int num_a_items = 0;  
  int i;
  for(i=0; i<10; i++){
    if(persist_exists(i)){
      interval class;
      persist_read_data(i, &class, sizeof(class));
      
      switch (class.day) {
        case 1:
          third_menu_items[num_a_items++] = (SimpleMenuItem) {
            .title = "Sunday"
          };
          break;
        case 2:
          third_menu_items[num_a_items++] = (SimpleMenuItem) {
            .title = "Monday"
          };
          break;
        case 3:
          third_menu_items[num_a_items++] = (SimpleMenuItem) {
            .title = "Tuesday"
          };
          break;
        case 4:
          third_menu_items[num_a_items++] = (SimpleMenuItem) {
            .title = "Wednesday"
          };
          break;
        case 5:
          third_menu_items[num_a_items++] = (SimpleMenuItem) {
            .title = "Thursday"
          };
          break;
        case 6:
          third_menu_items[num_a_items++] = (SimpleMenuItem) {
            .title = "Friday"
          };
          break;
        case 7:
          third_menu_items[num_a_items++] = (SimpleMenuItem) {
            .title = "Saturday"
          };
          break;
      }
    }
  }
  
  menu_sections[0] = (SimpleMenuSection){
    .num_items = 10,
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