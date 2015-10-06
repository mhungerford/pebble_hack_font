#include <pebble.h>

Window* my_window = NULL;

//Image Display
BitmapLayer *bitmap_layer = NULL;
GBitmap *gbitmap = NULL;

//Time Display
char time_string[] = "00:00";  // Make this longer to show AM/PM
TextLayer* time_outline_layer = NULL;
TextLayer* time_text_layer = NULL;

void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  clock_copy_time_string(time_string,sizeof(time_string));
  layer_mark_dirty(text_layer_get_layer(time_outline_layer));
  layer_mark_dirty(text_layer_get_layer(time_text_layer));
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  const GPoint center = grect_center_point(&bounds);
  
  //Add layers from back to front (background first)

  //Create bitmap layer for background image
  bitmap_layer = bitmap_layer_create(bounds);
  //Enable alpha blending for bitmap layer
  bitmap_layer_set_compositing_mode(bitmap_layer, GCompOpSet);
  //Add bitmap_layer to window layer
  layer_add_child(window_layer, bitmap_layer_get_layer(bitmap_layer));

  //Load bitmap image
  gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_1);
  bitmap_layer_set_bitmap(bitmap_layer, gbitmap);

  //Setup the time outline display
  time_outline_layer = text_layer_create(GRect(0, bounds.size.h / 2 - 42 / 2, bounds.size.w, 44));
  text_layer_set_text(time_outline_layer, time_string);
	text_layer_set_font(time_outline_layer, fonts_load_custom_font(
        resource_get_handle(RESOURCE_ID_FONT_BOXY_OUTLINE_42)));
  text_layer_set_text_alignment(time_outline_layer, GTextAlignmentCenter);
  text_layer_set_background_color(time_outline_layer, GColorClear);
  text_layer_set_text_color(time_outline_layer, GColorBlack);
  
  //Add clock text second
  layer_add_child(window_layer, text_layer_get_layer(time_outline_layer));

  //Setup the time display
  time_text_layer = text_layer_create(GRect(0, bounds.size.h / 2 - 42 / 2, bounds.size.w, 44));
  text_layer_set_text(time_text_layer, time_string);
	text_layer_set_font(time_text_layer, fonts_load_custom_font(
        resource_get_handle(RESOURCE_ID_FONT_BOXY_TEXT_42)));
  text_layer_set_text_alignment(time_text_layer, GTextAlignmentCenter);
  text_layer_set_background_color(time_text_layer, GColorClear);
  text_layer_set_text_color(time_text_layer, GColorWhite);
  
  //Add clock text second
  layer_add_child(window_layer, text_layer_get_layer(time_text_layer));

  //Force time update
  time_t current_time = time(NULL);
  struct tm *current_tm = localtime(&current_time);
  tick_handler(current_tm, MINUTE_UNIT);

  //Setup tick time handler
  tick_timer_service_subscribe((MINUTE_UNIT), tick_handler);
}

static void window_unload(Window *window) {
  bitmap_layer_destroy(bitmap_layer);
  gbitmap_destroy(gbitmap);
	window_destroy(my_window);
}

void handle_init(void) { 
  my_window = window_create();
  //window_set_fullscreen(my_window, true);
  window_set_background_color(my_window, GColorBlue);
  window_set_window_handlers(my_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(my_window, false);
}

void handle_deinit(void) {
}


int main(void) {
	  handle_init();
	  app_event_loop();
	  handle_deinit();
}
