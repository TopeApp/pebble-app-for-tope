#include <pebble.h>
#include <globals.h>
	
GFont FONT_LATO;

void init_globals() {
	// Load Fonts
	FONT_LATO = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_LATO_REGULAR_16));
}

void deinit_globals() {
	// Unload Fonts
	fonts_unload_custom_font(FONT_LATO);
}