#include <pebble.h>

int DATA_LOG_TAG_HAMILTON = 52;

static Window *window;
static TextLayer *text_layer;
static DataLoggingSessionRef s_log_ref;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Up");

  int value = 321;
  s_log_ref = data_logging_create(DATA_LOG_TAG_HAMILTON, DATA_LOGGING_INT, sizeof(int), true);
  DataLoggingResult result = data_logging_log(s_log_ref, &value, 1);
  if (result != DATA_LOGGING_SUCCESS) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error datalogging: %d (%d)", value, result);
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Success");
  }
  //text_layer_set_text(text_layer, "Down");
  data_logging_finish(s_log_ref);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  int value = 123;
  s_log_ref = data_logging_create(DATA_LOG_TAG_HAMILTON, DATA_LOGGING_INT, sizeof(int), true);
  DataLoggingResult result = data_logging_log(s_log_ref, &value, 1);
  if (result != DATA_LOGGING_SUCCESS) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error datalogging: %d (%d)", value, result);
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Success");
  }
  //text_layer_set_text(text_layer, "Down");
  data_logging_finish(s_log_ref);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 64 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press up to call an uber");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));

  text_layer = text_layer_create((GRect) { .origin = { 0, 84 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press down for an");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));

  text_layer = text_layer_create((GRect) { .origin = { 0, 99 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "EMERGENCY");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
