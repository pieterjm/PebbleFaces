#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID { 0x5E, 0x1E, 0xD0, 0x9C, 0x26, 0x24, 0x4F, 0x25, 0x8E, 0xC1, 0x32, 0xB0, 0x56, 0x30, 0x36, 0xAC }
PBL_APP_INFO(MY_UUID,
             "Template App", "Your Company",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;



#define STATE_FOCUS    0x0
#define STATE_UNFOCUS  0x1
#define CMD_SET_NUM_VALUES     0x2
#define CMD_SET_VALUES     0x3
#define CMD_SET_TITLES     0x4
	
#define MAX_SCREENS 8
	
Layer mainLayer[MAX_SCREENS];
TextLayer valueTextLayer[MAX_SCREENS];
TextLayer nameTextLayer[MAX_SCREENS];
char title[MAX_SCREENS][12];
char value[MAX_SCREENS][12];

int numberofscreens = 1;
	

static bool callbacks_registered;
static AppMessageCallbacksNode app_callbacks;

static void app_send_failed(DictionaryIterator* failed, AppMessageResult reason, void* context) {
  // TODO: error handling
}

void createTextLayer(uint8_t index,GRect bounds,GColor color) {
	layer_init(&(mainLayer[index]),bounds);	
	layer_set_clips(&(mainLayer[index]),false);
	layer_add_child(&window.layer,&(mainLayer[index]));
	
	if ( bounds.size.h < 57) {		
		text_layer_init(&(nameTextLayer[index]), GRect(0,0,bounds.size.w,15));
 		text_layer_set_font(&(nameTextLayer[index]), fonts_get_system_font(FONT_KEY_GOTHIC_14));
	} else if ( bounds.size.h < 100 ) {
		text_layer_init(&(nameTextLayer[index]), GRect(0,0,bounds.size.w,19));
 		text_layer_set_font(&(nameTextLayer[index]), fonts_get_system_font(FONT_KEY_GOTHIC_18));
    } else if ( bounds.size.h < 130 ) {
		text_layer_init(&(nameTextLayer[index]), GRect(0,0,bounds.size.w,25));
 		text_layer_set_font(&(nameTextLayer[index]), fonts_get_system_font(FONT_KEY_GOTHIC_24));
    } else {
    	text_layer_init(&(nameTextLayer[index]), GRect(0,0,bounds.size.w,29));
 		text_layer_set_font(&(nameTextLayer[index]), fonts_get_system_font(FONT_KEY_GOTHIC_28));
    }
	
	if ( bounds.size.w < 80 ) {		
			text_layer_init(&(valueTextLayer[index]),GRect(0,bounds.size.h/2-14,bounds.size.w,bounds.size.h));
			text_layer_set_font(&(valueTextLayer[index]), fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	} else {
//		if ( bounds.size.h < 80 ) {
//			text_layer_init(&(valueTextLayer[index]),GRect(0,bounds.size.h/2-21,bounds.size.w,bounds.size.h));
//			text_layer_set_font(&(valueTextLayer[index]), fonts_get_system_font(FONT_KEY_GOTHAM_42_LIGHT));
//		} else {
			text_layer_init(&(valueTextLayer[index]),GRect(0,bounds.size.h/2-21,bounds.size.w,bounds.size.h));
			text_layer_set_font(&(valueTextLayer[index]), fonts_get_system_font(FONT_KEY_GOTHAM_42_BOLD));
//		}
	}

	layer_set_clips(&(valueTextLayer[index].layer),false);
	text_layer_set_text_color(&(valueTextLayer[index]),GColorWhite);
	text_layer_set_background_color(&(valueTextLayer[index]), GColorClear);
	text_layer_set_text_alignment(&(valueTextLayer[index]), GTextAlignmentCenter);
	text_layer_set_text(&(valueTextLayer[index]),"");
  	layer_add_child(&(mainLayer[index]), &(valueTextLayer[index].layer));
																
  	text_layer_set_background_color(&(nameTextLayer[index]),GColorClear);
  	text_layer_set_text_color(&(nameTextLayer[index]),GColorWhite);
  	text_layer_set_text_alignment(&(nameTextLayer[index]), GTextAlignmentLeft);
	text_layer_set_text(&(nameTextLayer[index]),"");	
	layer_set_clips(&(nameTextLayer[index].layer),false);
	layer_add_child(&(mainLayer[index]), &(nameTextLayer[index].layer));
}

void initValues() {
	layer_remove_child_layers(&window.layer);
	switch ( numberofscreens ) {
	case 1: // een groot window
		createTextLayer(0,GRect(0,0,144,168),GColorWhite);
		break;
	case 2: // twee windows van gelijke windows
		createTextLayer(0,GRect(0,0,144,84),GColorWhite);
		createTextLayer(1,GRect(0,84,144,84),GColorBlack);
		break;
	case 3: // een groot scherm, en twee schermen
		createTextLayer(0,GRect(0,0,144,112),GColorWhite);
		createTextLayer(1,GRect(0,112,72,56),GColorWhite);
		createTextLayer(2,GRect(72,112,72,56),GColorBlack);
		break;
	case 4: // vier gelijk verdeelde schermen
		createTextLayer(0,GRect(0,0,72,84),GColorWhite);
		createTextLayer(1,GRect(72,0,72,84),GColorBlack);
		createTextLayer(2,GRect(0,84,72,84),GColorBlack);
		createTextLayer(3,GRect(72,84,72,84),GColorWhite);
		break;
	case 5: // een groot scherm en vier kleine schermen
		createTextLayer(0,GRect(0,0,144,56),GColorWhite);
		createTextLayer(1,GRect(0,56,72,56),GColorWhite);
		createTextLayer(2,GRect(72,56,72,56),GColorBlack);
		createTextLayer(3,GRect(0,112,72,56),GColorBlack);
		createTextLayer(4,GRect(72,112,72,56),GColorWhite);
		break;
	case 6: // zes gelijke windows
		createTextLayer(0,GRect(0,0,72,56),GColorWhite);
		createTextLayer(1,GRect(72,0,72,56),GColorBlack);
		createTextLayer(2,GRect(0,56,72,56),GColorBlack);
		createTextLayer(3,GRect(72,56,72,56),GColorWhite);
		createTextLayer(4,GRect(0,112,72,56),GColorWhite);
		createTextLayer(5,GRect(72,112,72,56),GColorBlack);
		break;
	case 7: // 1 grote sensor en zes kleine
		createTextLayer(0,GRect(0,0,144,42),GColorWhite);
		createTextLayer(1,GRect(0,42,72,42),GColorWhite);
		createTextLayer(2,GRect(72,42,72,42),GColorBlack);
		createTextLayer(3,GRect(0,84,72,42),GColorBlack);
		createTextLayer(4,GRect(72,84,72,42),GColorWhite);
		createTextLayer(5,GRect(0,126,72,42),GColorWhite);
		createTextLayer(6,GRect(72,126,72,42),GColorBlack);		
		break;
	case 8: // 8 gelijk verdeelde sensoren
		createTextLayer(0,GRect(0,0,72,42),GColorWhite);
		createTextLayer(1,GRect(72,0,72,42),GColorBlack);
		createTextLayer(2,GRect(0,42,72,42),GColorBlack);
		createTextLayer(3,GRect(72,42,72,42),GColorWhite);
		createTextLayer(4,GRect(0,84,72,42),GColorWhite);
		createTextLayer(5,GRect(72,84,72,42),GColorBlack);
		createTextLayer(6,GRect(0,126,72,42),GColorBlack);
		createTextLayer(7,GRect(72,126,72,42),GColorWhite);		
		break;
	}
}

void updateValue(int index,Tuple *tuple) {
	if ( index < MAX_SCREENS ) {
		strncpy(value[index], tuple->value->cstring, 12);
	}
	if ( index < numberofscreens ) {
		text_layer_set_text(&(valueTextLayer[index]),value[index]);  			
	}
}

void updateName(int index,Tuple *tuple) {
	if ( index < MAX_SCREENS ) {
		strncpy(title[index], tuple->value->cstring, 12);
	}
	if ( index < numberofscreens ) {
		text_layer_set_text(&(nameTextLayer[index]),title[index]);  			
	}
}

static void app_received_msg(DictionaryIterator* received, void* context) {
	Tuple *tuple = dict_read_first(received);
	if ( !tuple ) return;
	do {
		if(tuple->key >= 0xF000 && tuple->key <= 0xFFFF) continue;
		
		switch ( tuple->key ) {
			case CMD_SET_NUM_VALUES:
				if ( numberofscreens != tuple->value->uint8 ) {
					numberofscreens = tuple->value->uint8;
					initValues();
			    }
				break;
			case CMD_SET_VALUES:
				{
					char *pch = NULL;
					char *start = tuple->value->cstring;
					int index = 0;
					pch = tuple->value->cstring;
					while (*pch) {
						if ( *pch == ';' ) {
	    					strncpy(value[index],start,pch - start);
							start = pch + 1;
							index++;
                        }
                     	pch++;
                    }
					if ( *start ) {
	    				strcpy(value[index],start);
					}
					for(int i=0;(i<numberofscreens);i++) {
						text_layer_set_text(&(valueTextLayer[i]),value[i]);  			
					}
			    }
				break;
			case CMD_SET_TITLES:
				{
					char *pch = NULL;
					char *start = tuple->value->cstring;
					int index = 0;
					pch = tuple->value->cstring;
					while (*pch) {
						if ( *pch == ';' ) {
	    					strncpy(title[index],start,pch - start);
							start = pch + 1;
							index++;
                        }
                     	pch++;
                    }
					if ( *start ) {
	    				strcpy(title[index],start);
					}
					for(int i=0;(i<numberofscreens);i++) {
						text_layer_set_text(&(nameTextLayer[i]),title[i]);  			
					}
			    }
				break;
			default:
				break;
		}		
	 } while((tuple = dict_read_next(received)));
}

void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;
	
	numberofscreens++;
	if ( numberofscreens > 8 ) {
		numberofscreens = 1;
	}
	initValues();
}


static void send_cmd(uint8_t cmd) {
  Tuplet value = TupletInteger(0x0, cmd);
  
  DictionaryIterator *iter;
  app_message_out_get(&iter);
  
  if (iter == NULL)
    return;
  
  dict_write_tuplet(iter, &value);
  dict_write_end(iter);
  
  app_message_out_send();
  app_message_out_release();
}

bool register_callbacks() {
	if (callbacks_registered) {
		if (app_message_deregister_callbacks(&app_callbacks) == APP_MSG_OK)
			callbacks_registered = false;
	}
	if (!callbacks_registered) {
		app_callbacks = (AppMessageCallbacksNode){
			.callbacks = {
				.out_failed = app_send_failed,
        		.in_received = app_received_msg
			},
			.context = NULL
		};
		if (app_message_register_callbacks(&app_callbacks) == APP_MSG_OK) {
      		callbacks_registered = true;
        }
	}
	return callbacks_registered;
}



// This usually won't need to be modified

void click_config_provider(ClickConfig **config, Window *window) {
  (void)window;

//  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;

//  config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) select_long_click_handler;

//  config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
//  config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;

  config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;
  config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;
}


void handle_init(AppContextRef ctx) {
	(void)ctx;

  	window_init(&window, "Window Name");
  	window_set_background_color(&window, GColorBlack);							  
  	window_set_fullscreen(&window,true);
  	window_stack_push(&window, true /* Animated */);
	
	initValues();

	// Attach our desired button functionality
  	window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);
	
	register_callbacks();
	
	send_cmd(STATE_FOCUS);


}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
	.messaging_info = {
      .buffer_sizes = {
        .inbound = 1024,
        .outbound = 256,
      }
    }	
  };
  app_event_loop(params, &handlers);
}
