#include <pebble.h>
#include "globals.h"
#include "response.h"
#include "tope.h"

static Window *window_response;

static GFont lato_font;

static TextLayer *window_response_text_name_layer;
static BitmapLayer *window_response_profil_layer;

static ActionBarLayer *window_response_action_bar_layer;

static GBitmap *image_add_contact_bitmap;
static GBitmap *image_addno_contact_bitmap;

static GBitmap *image_profile_bitmap;

void add_contact_valid(const char *const name){
	vibes_short_pulse();
	text_layer_set_text(window_response_text_name_layer, name);
}

void back_and_down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
	// Send Message to Mobile App
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	dict_write_uint8( iter, 0, MESSAGE_TYPE_VALIDATION);
	dict_write_uint8( iter, 1, 0);
	app_message_outbox_send();
	window_stack_pop(true);
}

void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
	// Send Message to Mobile App
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	dict_write_uint8( iter, 0, MESSAGE_TYPE_VALIDATION);
	dict_write_uint8( iter, 1, 1);
	app_message_outbox_send();
	window_stack_pop(true);
}

void config_provider_response(void *context) {
	window_single_click_subscribe(BUTTON_ID_BACK, back_and_down_single_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, back_and_down_single_click_handler);
	window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
}

static void window_response_load(Window *window) {
	// Load Fonts
	lato_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_LATO_REGULAR_16));
	// Create Bitmap
	image_add_contact_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ADD_CONTACT_ACTION_BAR);
	image_addno_contact_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ADDNO_CONTACT_ACTION_BAR);
	image_profile_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PROFIL);
	// Create Name Text Layer
	window_response_text_name_layer = text_layer_create(GRect(0, 108, 124, 40));
	text_layer_set_font(window_response_text_name_layer, lato_font);
	text_layer_set_text_color(window_response_text_name_layer, GColorBlack);
	text_layer_set_text_alignment(window_response_text_name_layer, GTextAlignmentCenter);
	text_layer_set_text(window_response_text_name_layer, "Name");
	layer_add_child(window_get_root_layer(window_response), text_layer_get_layer(window_response_text_name_layer));
	// Create Profile Image Layer
	window_response_profil_layer = bitmap_layer_create(GRect(16, 8, 92, 92));
	bitmap_layer_set_bitmap(window_response_profil_layer, image_profile_bitmap);
	layer_add_child(window_get_root_layer(window_response), bitmap_layer_get_layer(window_response_profil_layer));
	// Create Action Bar Layer
	window_response_action_bar_layer = action_bar_layer_create();
	action_bar_layer_add_to_window(window_response_action_bar_layer, window);
	//action_bar_layer_set_click_config_provider(window_response_action_bar_layer, click_action_bar_config_provider);
	action_bar_layer_set_icon(window_response_action_bar_layer, BUTTON_ID_UP, image_add_contact_bitmap);
	action_bar_layer_set_icon(window_response_action_bar_layer, BUTTON_ID_DOWN, image_addno_contact_bitmap);
}

static void window_response_unload(Window *window) {
	fonts_unload_custom_font(lato_font);
	text_layer_destroy(window_response_text_name_layer);
	bitmap_layer_destroy(window_response_profil_layer);
	action_bar_layer_destroy(window_response_action_bar_layer);
	gbitmap_destroy(image_add_contact_bitmap);
	gbitmap_destroy(image_addno_contact_bitmap);
	gbitmap_destroy(image_profile_bitmap);
}

void init_response() {
	window_response = window_create();
	window_set_window_handlers(window_response, (WindowHandlers) {
		.load = window_response_load,
		.unload = window_response_unload,
	});
	window_stack_push(window_response, true);
	
	// Associate Click Config Provider 
	window_set_click_config_provider(window_response, config_provider_response);
}

void deinit_response() {
	window_destroy(window_response);
}