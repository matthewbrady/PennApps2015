#include <pebble.h>
#include "view_classes.h"
#include "schedules.h"
  
  
Window *s_main_window;
MenuLayer *menu_layer;

char buff[128];
static interval class;

/*void view_classes_menu_load(Window *window) {

  int numClasses = number_stored();
  printf("classes: %d", numClasses);
  SimpleMenuItem third_menu_items[numClasses];

  int num_a_items = 0;  
  int i;
  for(i=0; i<number_stored(); i++){
    if(persist_exists(i)){
      
      persist_read_data(i, &class, sizeof(class));
      
      third_menu_items[num_a_items++] = (SimpleMenuItem){
        // You should give each menu item a title and callback
        .title = "Tuesday",
      };
    }
  }
  
  menu_sections[0] = (SimpleMenuSection){
    .num_items = number_stored(),
    .items = third_menu_items,
  };
  Layer *menu_window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(menu_window_layer);
  
    // Initialize the simple menu layer
  simple_menu_layer3 = simple_menu_layer_create(bounds, window, menu_sections, NUM_MENU_SECTIONS, NULL);

  // Add it to the window for display
  layer_add_child(menu_window_layer, simple_menu_layer_get_layer(simple_menu_layer3));
}*/

void view_classes_menu_load(Window *window)
{
	//Create it - 12 is approx height of the top bar
	menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));
	
	//Give it its callbacks
	MenuLayerCallbacks callbacks = {
		.draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
		.get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
	};
	menu_layer_set_callbacks(menu_layer, NULL, callbacks);
	
	//Add to Window
	layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));
}
void view_classes_menu_unload(Window *window) {
  simple_menu_layer_destroy(simple_menu_layer3);

  // Cleanup the menu icon
  //gbitmap_destroy(menu_icon_image);
  
}

uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
	return number_stored();
}

void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
  char * weekday = "";
  char time_buffer[128];
  if (persist_exists(cell_index->row)) {
      interval class;
      persist_read_data(cell_index->row, &class, sizeof(class));
      switch (class.day) {
        case 0:
          weekday = "Sunday";
          break;
        case 1:
          weekday = "Monday";
          break;
        case 2:
          weekday = "Tuesday";
          break;
        case 3:
          weekday = "Wednesday";
          break;
        case 4:
          weekday = "Thursday";
          break;
        case 5:
          weekday = "Friday";
          break;
        case 6:
          weekday = "Saturday";
          break;
      }
      snprintf(time_buffer, 128, "%d%s%d", class.begin_time_mins/60, ":", class.begin_time_mins%60);
  }
  menu_cell_basic_draw(ctx, cell_layer, weekday, time_buffer, NULL);
}