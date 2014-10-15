Pebble App for TOPE!
====================

Communication with Android App
------------------------------

### Send

###### MESSAGE_TYPE_APP_START

When the Tope! Pebble App start.

Send :
* DictionaryIterator[0] = (uint8) messageType = 0

###### MESSAGE_TYPE_TOPE_EVENT

When user does a tope.

Send :
* DictionaryIterator[0] = (uint8) messageType = 1
* DictionaryIterator[1] = (uint8) seconde            // remove in next version
* DictionaryIterator[2] = (uint8) millisecondes      // remove in next version

###### MESSAGE_TYPE_VALIDATION

When user validate a tope.

Send :
* DictionaryIterator[0] = (uint8) messageType = 2
* DictionaryIterator[1] = (uint8) validate          // 0 if user refuse and 1 if user accept

### Receive

###### MESSAGE_TYPE_RESPONSE_TOPE

When the server return a response "tope".

Receive :
* DictionaryIterator[0] = (uint8) messageType = 0
* DictionaryIterator[1] = (cstring) userName        // Name of the person who has just topped with user
