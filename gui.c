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

#include "inc/types.c"
#include "inc/dispatcher.c"
#include "inc/render.c"

typedef enum {
	ID_WIN1, ID_TABGROUP, ID_TAB1, ID_TAB2, ID_TAB3,
	ID_T1G1, ID_T2G1, ID_T3G1,
	ID_T1G1_CB1, ID_T1G1_CB2, ID_T1G1_CB3,
	ID_T2G1_CB1, ID_T2G1_CB2, ID_T2G1_CB3,
	ID_T3G1_CB1, ID_T3G1_CB2, ID_T3G1_CB3
} GUI_ID;

char* item_labels[] = {
	"win1", "tabgroup", "tab1", "tab2", "tab3",
	"tab1 group1", "tab2 group1", "tab3 group1",
	"t1g1 cb1", "t1g1 cb2", "t1g1 cb3",
	"t2g1 cb1", "t2g1 cb2", "t2g1 cb3",
	"t3g1 cb1", "t3g1 cb2", "t3g1 cb3",
};

static struct ModelWin1 {
	GUI_ID ids[3];
	int* values[3];
	int* flags[3];
	int t1g1_cb1, t1g1_cb2, t1g1_cb3;
	int t1g1_cb1_on, t1g1_cb2_on, t1g1_cb3_on;
} model_win1 = {
	.ids = {
		ID_T1G1_CB1, ID_T1G1_CB2, ID_T1G1_CB3
	},
	.values = {
		&model_win1.t1g1_cb1, &model_win1.t1g1_cb2, &model_win1.t1g1_cb3
	},
	.flags = {
		&model_win1.t1g1_cb1_on, &model_win1.t1g1_cb2_on, &model_win1.t1g1_cb3_on
	}
};

void process_win1(
	GUI_Dispatcher* dsp, GUI_TargetResult tr, uint16_t x, uint16_t y,
	struct ModelWin1* model
) {
	GUI_Item item = dsp->items[tr.item_index].item;
	if (item.type == GUI_ITEM_CHECKBOX) {
		GUI_Checkbox* cb = item.data;
		int flag = item.status & GUI_STATUS_SELECTED ? 1 : 0;
		for (int i = 0; i < 3; i++) {
			if (item.id == model->ids[i]) {
				*model->values[i] = cb->ivalue;
				*model->flags[i] = flag;
				break;
			}
		}
	}
}

void route_item(
	GUI_Dispatcher* dsp, GUI_TargetResult tr, uint16_t x, uint16_t y
) {
	GUI_UIDRecord uidrec = dsp->uids[tr.uid_index];
	GUI_ItemRecord* irec = &dsp->items[uidrec.item_index];

	if (irec->item.id == ID_WIN1) {
		process_win1(dsp, tr, x, y, &model_win1);
	}
}

int main(int argc, char* argv[])
{
	GUI_Color color_default = { 0xcccccc, 0x000000, 0xffffff, 0x666666, 0xff0000 };
	GUI_Color color_title_default = { 0x006600, 0x00cc00, 0xffffff, 0x333333, 0xff0000 };
	GUI_Color color_main = { 0xcccccc, 0x000000, 0xffffff, 0x999999, 0xff0000 };
	GUI_Color color_title_main = { 0xff0000, 0xffff00, 0xffffff, 0x333333, 0xff0000 };
	GUI_Shape shape_default = { 2, 2, 2, 1, 0, 4, 4 };
	GUI_Shape shape_main = { 4, 4, 4, 2, 0, 8, 8 };

	GUI_Dispatcher dispatcher = {
		0, 0,
		GUI_UID_FIRST,
	};

	#include "inc/layout.c"

	GUI_dispatcher_push_tree(&dispatcher, &layout);
	GUI_dispatcher_fix_rects(&dispatcher);

	GUI_dispatcher_list_items(&dispatcher);

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
		(SDL_Rect) { 0, 0, WIN_W, WIN_H },
		SDL_GetWindowSurface(window),
		(GUI_Window) {
			&color_default, &color_default, &color_title_default, &color_title_default,
			&color_default, &color_default,
			&shape_default, &shape_default, GUI_font08x16, GUI_font08x16, "Window"
		},
		(GUI_Button) {
			&color_default, &color_default, &color_default, &color_default, &color_default,
			&shape_default, &shape_default, &shape_default, &shape_default,
			GUI_font08x16, "Button"
		},
		GUI_font08x16
	};

	GUI_render(&dispatcher, &ctx);
	SDL_UpdateWindowSurface(window);
	SDL_Event event;

	while (1) {
		int8_t result = SDL_PollEvent(&event);
		if (result) {
			if (event.type == SDL_QUIT) {
				break;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				GUI_TargetResult target = GUI_dispatcher_find_target(
					&dispatcher, event.button.x, event.button.y
				);
				if (target.result == GUI_OK) {
					//printf("%s\n", item_labels[target.item_index]);
					GUI_process_item(
						&dispatcher, target, event.button.x, event.button.y
					);
					route_item(
						&dispatcher, target, event.button.x, event.button.y
					);
					char *cb_status[] = { "off", "on" };
					printf(
						"model cb1 is %s, val=%d, cb2 is %s, val=%d, cb3 is %s, val=%d\n",
						cb_status[model_win1.t1g1_cb1_on], model_win1.t1g1_cb1,
						cb_status[model_win1.t1g1_cb2_on], model_win1.t1g1_cb2,
						cb_status[model_win1.t1g1_cb3_on], model_win1.t1g1_cb3
					);
					GUI_render(&dispatcher, &ctx);
					SDL_UpdateWindowSurface(window);
				} else {
					printf("no target\n");
				}
            }
		}
        SDL_Delay(10);
	}

	SDL_FreeSurface(ctx.surf);
	SDL_DestroyWindow(window);
	SDL_Quit();

	printf("item cnt: %lu max uid: %lu\n", dispatcher.item_cnt, dispatcher.uid);

	return 0;
}
