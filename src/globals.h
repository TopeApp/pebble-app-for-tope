#ifndef GLOBALS_H
#define GLOBALS_H

// Message Type Pebble to Smartphone
#define MESSAGE_TYPE_APP_START 0
#define MESSAGE_TYPE_TOPE_EVENT 1
#define MESSAGE_TYPE_VALIDATION 2

// Message Type Smartphone to Pebble
#define MESSAGE_TYPE_RESPONSE_TOPE 0

// Ressources
extern GFont FONT_LATO;

void init_globals();
void deinit_globals();

#endif