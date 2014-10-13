#include <pebble.h>
#include <globals.h>
#include <response.h>
#include <tope.h>

void out_sent_handler(DictionaryIterator *sent, void *context) {
	// outgoing message was delivered
}

void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
	// outgoing message failed
}

void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *message_type = dict_find(received, 0);
	switch(message_type->value->uint8){
		case MESSAGE_TYPE_RESPONSE_TOPE :
			init_response();
			Tuple *text_name = dict_find(received, 1);
			add_contact_valid(text_name->value->cstring);
		break;
	}
}

void in_dropped_handler(AppMessageResult reason, void *context) {
	// incoming message dropped
}

void init() {
	init_globals();
	init_tope();
	
	// App Message Handler
	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_register_outbox_sent(out_sent_handler);
	app_message_register_outbox_failed(out_failed_handler);

	const uint32_t inbound_size = 64;
	const uint32_t outbound_size = 64;
	app_message_open(inbound_size, outbound_size);
	
	// Send Message Start to Mobile App
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	dict_write_uint8( iter, 0, MESSAGE_TYPE_APP_START);
	app_message_outbox_send();
}

void deinit() {
	deinit_tope();
	deinit_response();
	deinit_globals();
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}