#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "inc/font.c"

#define GUI_MAX_ITEMS 1024
#define GUI_UID_NONE 0
#define GUI_UID_FIRST 1
#define WIN_W 1280
#define WIN_H 960

typedef enum {
	ID_BTN_CLOSE, ID_WIN1, ID_CAPTION1, ID_TABGROUP,
	ID_TAB1, ID_TAB2, ID_TAB3,
	ID_T2G1, ID_T3G1,
	ID_T2G1_CB1, ID_T2G1_CB2, ID_T2G1_CB3,
	ID_T3G1_CB1, ID_T3G1_CB2, ID_T3G1_CB3,
	ID_PANEL1, ID_BTN1, ID_BTN2,
	ID_TEST
} GUI_ID;

#include "inc/elements.c"
#include "inc/types.c"
#include "inc/dispatcher.c"
#include "inc/render.c"

static struct ModelWin1 {
	GUI_ID ids[3];
	int* flags[3];
	int t2g1_cb1, t2g1_cb2, t2g1_cb3;
	int t2g1_cb1_on, t2g1_cb2_on, t2g1_cb3_on;
	int counter;
} model_win1 = {
	.ids = {
		ID_T2G1_CB1, ID_T2G1_CB2, ID_T2G1_CB3
	},
	.flags = {
		&model_win1.t2g1_cb1_on, &model_win1.t2g1_cb2_on, &model_win1.t2g1_cb3_on
	},
	.t2g1_cb1 = 1, .t2g1_cb2 = 2, .t2g1_cb3 = 3
};

void process_win1(GUI_Dispatcher* dsp, struct ModelWin1* model)
{
	GUI_Item item = dsp->items[dsp->last_index].item;
	if (item.type == GUI_ITEM_BUTTON) {
		if (item.id == ID_BTN1 || item.id == ID_BTN2) {
			GUI_IndexResult ir = GUI_dispatcher_find_item(dsp, dsp->last_uid_index, ID_PANEL1);
			if (ir.result == GUI_OK) {
				GUI_DisplayPanel* dp = dsp->items[ir.index].item.data;
				if (item.id == ID_BTN1) {
					dp->ivalue--;
				} else {
					dp->ivalue++;
				}
			}
		}
	} else if (item.type == GUI_ITEM_CHECKBOX) {
		GUI_Checkbox* cb = item.data;
		int flag = item.status & GUI_STATUS_SELECTED ? 1 : 0;
		for (int i = 0; i < 3; i++) {
			if (item.id == model->ids[i]) {
				*model->flags[i] = flag;
				break;
			}
		}
	}
}

void route_item(GUI_Dispatcher* dsp)
{
	GUI_ItemRecord* irec = &dsp->items[dsp->last_uid_index];

	if (irec->item.id == ID_WIN1) {
		process_win1(dsp, &model_win1);
	}
}

int main(int argc, char* argv[])
{
	#include "inc/layout.c"

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow(
		"GUI",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIN_W,
		WIN_H,
		SDL_WINDOW_ALLOW_HIGHDPI
	);

	if (window == NULL) {
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	GUI_Context ctx = {
		.rect = (SDL_Rect) { 0, 0, WIN_W, WIN_H },
		.surf = SDL_GetWindowSurface(window),
		.color_body_active = &color_body,
		.color_body_passive = &color_body,
		.color_caption_active = &color_caption,
		.color_caption_passive = &color_caption,
		.color_button_active = &color_body,
		.color_button_passive = & color_body,
		.color_button_hover = &color_button_hover,
		.color_button_down = &color_button_down,
		.shape_body = &shape_body,
		.shape_caption = &shape_caption,
		.shape_button = &shape_button,
		.shape_button_down = &shape_button_down,
		.font_body = GUI_font08x16,
		.font_caption = GUI_font08x16,
		.font_button = GUI_font08x16,
		.current_color = ctx.color_body_active
	};
	GUI_Dispatcher dispatcher = {
		.uid_cnt = 0, .item_cnt = 0, .uid = GUI_UID_FIRST,
		.state = GUI_STATUS_NONE
	};
	GUI_dispatcher_push_tree(&dispatcher, &layout);
	GUI_dispatcher_fix_rects(&ctx, &dispatcher);

	GUI_dispatcher_list_items(&dispatcher);

	GUI_render(&dispatcher, &ctx);
	SDL_UpdateWindowSurface(window);
	SDL_Event event;

	while (1) {
		if (SDL_PollEvent(&event)) {
			GUI_Result dirty = GUI_NONE;
			if (event.type == SDL_QUIT) {
				break;
			}
			GUI_Event gui_event = GUI_convert_event(&event);
			if (gui_event.type != GUI_EVENT_NONE) {
				dirty = GUI_dispatcher_process_event(&dispatcher, gui_event);
			}
			if (dirty == GUI_OK) {
				if (gui_event.type == GUI_EVENT_UP) {
					route_item(&dispatcher);
				}
				GUI_render(&dispatcher, &ctx);
				SDL_UpdateWindowSurface(window);
			}
		}
        SDL_Delay(10);
	}

	SDL_FreeSurface(ctx.surf);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
