#include <pebble.h>
#include <string.h>

Window *window;
TextLayer *text_layer;
TextLayer *text_layer2;
TextLayer *text_layer3;
TextLayer *text_layer4;

char location_buffer[64];

enum {
  KEY_LOCATION = 0,
  KEY_TEMPERATURE = 1,
};

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
   text_layer_set_text(text_layer4, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
   text_layer_set_text(text_layer4, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
   text_layer_set_text(text_layer4, "Down");
}

static void click_config_provider(void *context) {
   window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
   window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
   window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void process_tuple(Tuple *t){
   //Get key
   int key = t->key;

   //Get integer value, if present
   int value = t->value->int32;

   //Get string value, if present
   char string_value[32];

   //Get string value, if present
   strcpy(string_value, t->value->cstring);

   //Decide what to do
   switch(key) {
      case KEY_LOCATION:
         //Location received
         APP_LOG(APP_LOG_LEVEL_INFO, "processing %d => %s", key, t->value->cstring);
         snprintf(location_buffer, sizeof("123456789012345678901234567890"), string_value);
         text_layer_set_text(text_layer4, (char*) &location_buffer);
         persist_write_data(KEY_LOCATION, string_value, sizeof(string_value));
         break;
      case KEY_TEMPERATURE:
         //Temperature received
         snprintf(location_buffer, sizeof("Temperature: XX \u00B0C"), "Temperature: %d \u00B0C", value);
         text_layer_set_text(text_layer3, (char*) &location_buffer);
         break;
   }
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
   (void) context;
   APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");

   //Get data
   Tuple *t = dict_read_first(iterator);
   Tuple *data = dict_find(iterator, KEY_LOCATION);
   if (data != NULL) {  
      APP_LOG(APP_LOG_LEVEL_DEBUG, "[DBUG] in_recv_handler - NONEMPTY TUPLE");
   } else { 
      APP_LOG(APP_LOG_LEVEL_DEBUG, "[DBUG] in_recv_handler - EMPTY TUPLE"); 
   }

   while(t != NULL)
   {
      process_tuple(t);

      //Get next
      t = dict_read_next(iterator);
   }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void appmsg_in_dropped(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "In dropped: %i", reason);
}

char *itoa(int num) {
  static char buff[20] = {};
  int i = 0, temp_num = num, length = 0;
  char *string = buff;
  if(num >= 0) {
      // count how many characters in the number
      while(temp_num) {
      temp_num /= 10;
      length++;
    }
    // assign the number to the buffer starting at the end of the 
    // number and going to the begining since we are doing the
    // integer to character conversion on the last number in the
    // sequence
    for(i = 0; i < length; i++) {
      buff[(length-1)-i] = '0' + (num % 10);
      num /= 10;
    }
    buff[i] = '\0'; // can't forget the null byte to properly end our string
  }
  else {
    return "Unsupported Number";
  }
  return string;
}

char *returnText(void) {
  static char buff[20] = {};
  char *str = buff;
  for(int i=0; i<10; i++) {
     strcat(str, itoa(i));
  }
  
  return str;
}

void handle_init(void) {
	// Create a window and text layer
	window = window_create();
	text_layer = text_layer_create(GRect(0, 0, 144, 60));
	text_layer2 = text_layer_create(GRect(0, 60, 144, 30));
	text_layer3 = text_layer_create(GRect(0, 100, 144, 30));
	text_layer4 = text_layer_create(GRect(0, 130, 144, 30));
	
	// Set the text, font, and text alignment
	text_layer_set_text(text_layer, "Hi, I'm a Pebble!");
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	
	// Set the text, font, and text alignment
	text_layer_set_text(text_layer2, "By Dave");
	text_layer_set_font(text_layer2, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_alignment(text_layer2, GTextAlignmentCenter);
	
	text_layer_set_text(text_layer3, returnText());
	text_layer_set_font(text_layer3, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_alignment(text_layer3, GTextAlignmentCenter);
	
  
  char string_value[32];
  persist_read_data(KEY_LOCATION, string_value, sizeof(string_value));
  if ( string_value != NULL) {
     snprintf(location_buffer, sizeof("123456789012345678901234567890"), string_value);
     text_layer_set_text(text_layer4, (char*) &location_buffer);
  } else {
     text_layer_set_text(text_layer4, "Waiting...");
  }
	text_layer_set_font(text_layer4, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_alignment(text_layer4, GTextAlignmentCenter);
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer2));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer3));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer4));

  window_set_click_config_provider(window, click_config_provider);

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_register_inbox_dropped(appmsg_in_dropped);

  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  // Push the window
	window_stack_push(window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(text_layer);
	text_layer_destroy(text_layer2);
	text_layer_destroy(text_layer3);
	text_layer_destroy(text_layer4);
	
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
