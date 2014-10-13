#include <pebble.h>
#include <globals.h>
#include <tope.h>
#include <response.h>

static Window *window_tope;

static BitmapLayer *window_tope_background_layer;
static TextLayer *window_tope_text_layer;
static BitmapLayer *window_tope_logo_layer;

static GBitmap *logo_wait_bitmap;
static GBitmap *logo_tope_bitmap;

static int tope_sharing = 0;

void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
	// Send Message Start to Resend Queue in Mobile App
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	dict_write_uint8( iter, 0, MESSAGE_TYPE_APP_START);
	app_message_outbox_send();
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	tope_sharing++;
	if(tope_sharing>3){
		text_layer_set_text(window_tope_text_layer, "Tope it!");
		bitmap_layer_set_bitmap(window_tope_logo_layer, logo_wait_bitmap);
		tick_timer_service_unsubscribe();
		tope_sharing = 0;
	}
}

void accel_tap_handler(AccelAxisType axis, int32_t direction) {
	if(tope_sharing==0){
		// Change Display
		text_layer_set_text(window_tope_text_layer, "Sharing ...");
		bitmap_layer_set_bitmap(window_tope_logo_layer, logo_tope_bitmap);
		// Send Message to Mobile App
		DictionaryIterator *iter;
		app_message_outbox_begin(&iter);
		dict_write_uint8( iter, 0, MESSAGE_TYPE_TOPE_EVENT);
		app_message_outbox_send();
		// Go to Sharing Mode
		tope_sharing++;
		tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
	}
}

void config_provider_tope(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
}

void window_tope_load(Window *window) {
	// Create Bitmap
	logo_wait_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOGO_WAIT_WHITE);
	logo_tope_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOGO_TOPE_WHITE);
	// Create Background Layer
	window_tope_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_background_color(window_tope_background_layer, GColorBlack);
	layer_add_child(window_get_root_layer(window_tope), bitmap_layer_get_layer(window_tope_background_layer));
	// Create Text Layer
	window_tope_text_layer = text_layer_create(GRect(0, 120, 144, 25));
	text_layer_set_font(window_tope_text_layer, FONT_LATO);
	text_layer_set_background_color(window_tope_text_layer, GColorClear);
	text_layer_set_text_color(window_tope_text_layer, GColorWhite);
	text_layer_set_text_alignment(window_tope_text_layer, GTextAlignmentCenter);
	text_layer_set_text(window_tope_text_layer, "Tope it!");
	layer_add_child(window_get_root_layer(window_tope), text_layer_get_layer(window_tope_text_layer));
	// Create Logo Layer
	window_tope_logo_layer = bitmap_layer_create(GRect(0, 10, 144, 100));
	bitmap_layer_set_bitmap(window_tope_logo_layer, logo_wait_bitmap);
	layer_add_child(window_get_root_layer(window_tope), bitmap_layer_get_layer(window_tope_logo_layer));
}

void window_tope_appear(Window *window) {
	accel_tap_service_subscribe(accel_tap_handler);
}

void window_tope_disappear(Window *window) {
	accel_tap_service_unsubscribe();
}

void window_tope_unload(Window *window) {
	gbitmap_destroy(logo_wait_bitmap);
	gbitmap_destroy(logo_tope_bitmap);
	bitmap_layer_destroy(window_tope_background_layer);
	text_layer_destroy(window_tope_text_layer);
	bitmap_layer_destroy(window_tope_logo_layer);
}

void init_tope() {
	// Create and Show Window
	window_tope = window_create();
	window_set_window_handlers(window_tope, (WindowHandlers) {
		.load = window_tope_load,
        .appear = window_tope_appear,
        .disappear = window_tope_disappear,
		.unload = window_tope_unload,
	});
	window_stack_push(window_tope, true);
	
	// Associate Click Config Provider 
	window_set_click_config_provider(window_tope, config_provider_tope);
}

void deinit_tope() {
	window_destroy(window_tope);
}