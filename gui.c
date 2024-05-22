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
#define WIN_W 960
#define WIN_H 720

typedef enum {
	ID_NONE, ID_BTN_CLOSE, ID_WIN1, ID_CAPTION, ID_TABGROUP,
	ID_TAB1, ID_TAB2, ID_TAB3,
	ID_T2G1, ID_T3G1,
	ID_T2G1_CB1, ID_T2G1_CB2, ID_T2G1_CB3,
	ID_T3G1_CB1, ID_T3G1_CB2, ID_T3G1_CB3,
	ID_PANEL1, ID_BTN1, ID_BTN2,
	ID_TEST, ID_HSLIDER_R, ID_HSLIDER_G, ID_HSLIDER_B,
	ID_HSLIDERPANE,
	ID_SBTN1, ID_SBTN2
} GUI_ID;

#include "inc/types.c"
#include "inc/elements.c"
#include "inc/dispatcher.c"
#include "inc/render.c"

static struct ModelWin1 {
	GUI_ID ids[3];
	int* flags[3];
	int t2g1_cb1, t2g1_cb2, t2g1_cb3;
	int t2g1_cb1_on, t2g1_cb2_on, t2g1_cb3_on;
	int counter;
	int quit;
} model_win1 = {
	.ids = {
		ID_T2G1_CB1, ID_T2G1_CB2, ID_T2G1_CB3
	},
	.flags = {
		&model_win1.t2g1_cb1_on, &model_win1.t2g1_cb2_on, &model_win1.t2g1_cb3_on
	},
	.t2g1_cb1 = 1, .t2g1_cb2 = 2, .t2g1_cb3 = 3,
	.quit = 0
};

void set_caption_color_win1(GUI_Dispatcher* dsp)
{
	GUI_IndexResult ir = GUI_dispatcher_find_item(dsp, dsp->last_uid_index, ID_HSLIDER_R);
	uint32_t color = ((GUI_Slider*)(dsp->items[ir.index].item.element))->value;
	ir = GUI_dispatcher_find_item(dsp, dsp->last_uid_index, ID_HSLIDER_G);
	color = (color << 8) + ((GUI_Slider*)(dsp->items[ir.index].item.element))->value;
	ir = GUI_dispatcher_find_item(dsp, dsp->last_uid_index, ID_HSLIDER_B);
	color = (color << 8) + ((GUI_Slider*)(dsp->items[ir.index].item.element))->value;
	ir = GUI_dispatcher_find_item(dsp, dsp->last_uid_index, ID_CAPTION);
	((GUI_Caption*)(dsp->items[ir.index].item.element))->color_active->bg = color;
}

void update_display_panel_win1(GUI_Dispatcher* dsp, GUI_Command cmd)
{
	GUI_IndexResult ir = GUI_dispatcher_find_item(dsp, dsp->last_uid_index, ID_PANEL1);
	if (ir.result == GUI_OK) {
		GUI_DisplayPanel* dp = dsp->items[ir.index].item.element;
		if (cmd.type == GUI_CMD_DECVAL) {
			dp->ivalue--;
		} else {
			dp->ivalue++;
		}
	}
}

void process_win1(GUI_Dispatcher* dsp, GUI_ComboEvent cevt, struct ModelWin1* model)
{
	size_t index = dsp->last_index;
	GUI_ItemRecord* irec = &dsp->items[index];
	GUI_Item item = irec->item;

	if (cevt.type == GUI_EVENT_CMD) {
		switch (cevt.cmd.type) {
			case GUI_CMD_SETCOLOR:
				set_caption_color_win1(dsp);
				break;
			case GUI_CMD_CLOSE:
				model->quit = 1;
				break;
			case GUI_CMD_INCVAL:
			case GUI_CMD_DECVAL:
				update_display_panel_win1(dsp, cevt.cmd);
				break;
		}				
		return;
	}

	if (cevt.type == GUI_EVENT_UP) {
		if (item.type == GUI_ITEM_CHECKBOX) {
			GUI_Checkbox* cb = item.element;
			int flag = item.status & GUI_STATUS_SELECTED ? 1 : 0;
			for (int i = 0; i < 3; i++) {
				if (item.id == model->ids[i]) {
					*model->flags[i] = flag;
					break;
				}
			}
		}
	}
}

void route_item(GUI_Dispatcher* dsp, GUI_ComboEvent cevt)
{
	GUI_ItemRecord* irec = &dsp->items[dsp->last_uid_index];

	if (irec->item.id == ID_WIN1) {
		process_win1(dsp, cevt, &model_win1);
	}
}

int main(int argc, char* argv[])
{
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

	#include "inc/styles.c"

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

	#include "inc/layout.c"

	GUI_dispatcher_push_tree(&dispatcher, &layout);
	GUI_dispatcher_fix_rects(&ctx, &dispatcher);

	GUI_dispatcher_list_items(&dispatcher);

	GUI_render(&dispatcher, &ctx);
	SDL_UpdateWindowSurface(window);
	SDL_Event event;

	while (!model_win1.quit) {
		uint16_t needs_redraw = 0;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				model_win1.quit = 1;
				break;
			}
			GUI_Event gui_event = GUI_convert_event(&event);
			if (gui_event.type != GUI_EVENT_NONE) {
				GUI_ComboEvent cevt = GUI_dispatcher_process_event(&dispatcher, gui_event);
				if (cevt.type != GUI_EVENT_NONE) {
					route_item(&dispatcher, cevt);
				}
				if (cevt.result == GUI_OK) {
					needs_redraw = 1;
				}
			}
		}
		if (needs_redraw) {
			GUI_render(&dispatcher, &ctx);
			SDL_UpdateWindowSurface(window);
		}
        SDL_Delay(10);
	}

	SDL_FreeSurface(ctx.surf);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
