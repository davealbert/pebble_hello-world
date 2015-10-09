#include <pebble.h>
#include <string.h>

Window *window;
TextLayer *text_layer;
TextLayer *text_layer2;
TextLayer *text_layer3;

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
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer2));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer3));

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
	
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
