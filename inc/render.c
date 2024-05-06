GUI_Rect GUI_render_panel(GUI_Context* ctx, SDL_Rect rect, GUI_Color* color, GUI_Shape* shape)
{
	uint16_t bump = shape->bump_out;
	if (bump) {
		SDL_Rect r = { rect.x, rect.y, rect.w, bump };
		rect.h -= bump << 1;
		rect.w -= bump << 1;
		rect.y += bump;
		SDL_FillRect(ctx->surf, &r, color->light);
		r.y += rect.h + bump;
		SDL_FillRect(ctx->surf, &r, color->dark);
		r.y = rect.y;
		r.w = bump;
		r.h = rect.h;
		SDL_FillRect(ctx->surf, &r, color->light);
		rect.x += bump;
		r.x += rect.w + bump;
		SDL_FillRect(ctx->surf, &r, color->dark);
	}
	bump = shape->border;
	if (bump) {
		SDL_Rect r = { rect.x, rect.y, rect.w, bump };
		rect.h -= bump << 1;
		rect.w -= bump << 1;
		rect.y += bump;
		SDL_FillRect(ctx->surf, &r, color->border);
		r.y += rect.h + bump;
		SDL_FillRect(ctx->surf, &r, color->border);
		r.y = rect.y;
		r.w = bump;
		r.h = rect.h;
		SDL_FillRect(ctx->surf, &r, color->border);
		rect.x += bump;
		r.x += rect.w + bump;
		SDL_FillRect(ctx->surf, &r, color->border);
	}
	bump = shape->bump_in;
	if (bump) {
		SDL_Rect r = { rect.x, rect.y, rect.w, bump };
		rect.h -= bump << 1;
		rect.w -= bump << 1;
		rect.y += bump;
		SDL_FillRect(ctx->surf, &r, color->dark);
		r.y += rect.h + bump;
		SDL_FillRect(ctx->surf, &r, color->light);
		r.y = rect.y;
		r.w = bump;
		r.h = rect.h;
		SDL_FillRect(ctx->surf, &r, color->dark);
		rect.x += bump;
		r.x += rect.w + bump;
		SDL_FillRect(ctx->surf, &r, color->light);
	}
	SDL_FillRect(ctx->surf, &rect, color->bg);
	return rect;
}

void GUI_render_text(GUI_Context* ctx, int16_t x, int16_t y, uint32_t color, uint8_t* text, uint8_t* font)
{
	uint8_t bytewidth = font[0];
	uint8_t h = font[1];
	uint16_t chunk_size = bytewidth * h + 2;
	uint16_t pos = 0;
	SDL_Surface* surf = ctx->surf;
	uint8_t* addr = (uint8_t*)surf->pixels + y * surf->pitch + x * 4;
	while (text[pos]) {
		uint8_t* start = font + 2 + chunk_size * (text[pos] - 1);
		uint8_t subst = *start++;
		uint8_t width = *start++;
		uint32_t offset_y = 0;
		for (uint8_t i = 0; i < h; i++) {
			for (uint8_t j = 0; j < bytewidth; j++) {
				uint8_t offset_x = j * 8 * 4;
				uint8_t byte = *start++;
				while (byte) {
					if (byte & 128) {
						*((uint32_t*)(addr + offset_y + offset_x)) = color;
					}
					offset_x += 4;
					byte <<= 1;
				}
			}
			offset_y += surf->pitch;
		}
		addr += width * 4;
		pos++;
	}
}

void GUI_render_number(GUI_Context* ctx, int16_t x, int16_t y, uint32_t color, uint32_t number, uint8_t* font)
{
	char digits[16];
	digits[15] = 0;
	size_t i = 15;
	do {
		digits[--i] = (number % 10) + '0';
		number /= 10;
	} while (number);

	GUI_render_text(ctx, x, y, color, &digits[i], font);
}

void GUI_render_displaypanel(GUI_Dispatcher* dsp, GUI_Context* ctx, GUI_ItemNode* node)
{
	GUI_Color* color;
	GUI_Shape* shape;

	GUI_ItemRecord* irec = &(dsp->items[node->index]);
	GUI_DisplayPanel* panel = (GUI_DisplayPanel*) irec->item.element;

	if (irec->item.status & GUI_STATUS_ACTIVE) {
		color = panel->color_active == NULL ? ctx->color_body_active : panel->color_active;
	} else {
		color = panel->color_passive == NULL ? ctx->color_body_passive : panel->color_passive;
	}
	shape = panel->shape == NULL ? ctx->shape_body : panel->shape;
	GUI_Rect rect = GUI_render_panel(ctx, irec->rect, color, shape);
	GUI_render_text(ctx, rect.x, rect.y - 20, ctx->current_color->fg, panel->label, GUI_font08x16);
	GUI_render_number(ctx, rect.x + 4, rect.y + 4, color->fg, panel->ivalue, GUI_font08x16);
	
}

void GUI_render_caption(GUI_Dispatcher* dsp, GUI_Context* ctx, GUI_ItemNode* node)
{
	GUI_ItemRecord* irec = &(dsp->items[node->index]);
	GUI_Caption* cap = (GUI_Caption*) irec->item.element;
	GUI_Color* color;
	if (irec->item.status & GUI_STATUS_ACTIVE) {
		color = cap->color_active == NULL ? ctx->color_caption_active : cap->color_active;
	} else {
		color = cap->color_passive == NULL ? ctx->color_caption_passive : cap->color_passive;
	}
	GUI_Shape* shape = cap->shape == NULL ? ctx->shape_caption : cap->shape;
	uint16_t m = shape->margin;
	GUI_Rect rect = irec->rect;
	rect.x += m;
	rect.y += m;
	rect.w -= m << 1;
	rect.h -= m << 1;
	rect = GUI_render_panel(ctx, rect, color, shape);
	GUI_render_text(ctx, rect.x + shape->padding, rect.y + shape->padding, color->fg, cap->title, GUI_font08x16);
}

void GUI_render_window(GUI_Dispatcher* dsp, GUI_Context* ctx, GUI_ItemNode* node)
{
	GUI_ItemRecord* irec = &(dsp->items[node->index]);
	GUI_Window* win = (GUI_Window*) irec->item.element;

	GUI_Color* color;
	if (irec->item.status & GUI_STATUS_ACTIVE) {
		color = win->color_active == NULL ? ctx->color_body_active : win->color_active;
	} else {
		color = win->color_passive == NULL ? ctx->color_body_passive : win->color_passive;
	}
	GUI_Shape* shape = win->shape == NULL ? ctx->shape_body : win->shape;
	GUI_Rect rect = GUI_render_panel(ctx, irec->rect, color, shape);
	ctx->current_color = color;
}

void GUI_render_button(GUI_Dispatcher* dsp, GUI_Context* ctx, GUI_ItemNode* node)
{
	GUI_Color* color;
	GUI_Shape* shape;

	GUI_ItemRecord* irec = &(dsp->items[node->index]);
	GUI_Button* btn = (GUI_Button*) irec->item.element;

	GUI_ItemStatus status = irec->item.status;
	if (status & GUI_STATUS_ACTIVE) {
		if (status & GUI_STATUS_DOWN) {
			color = btn->color_down == NULL ? ctx->color_button_down : btn->color_down;
			shape = btn->shape_down == NULL ? ctx->shape_button_down : btn->shape_down;
		} else if (status & GUI_STATUS_HOVER) {
			color = btn->color_hover == NULL ? ctx->color_button_hover : btn->color_hover;
			shape = btn->shape == NULL ? ctx->shape_button : btn->shape;
		} else {
			color = btn->color_active == NULL ? ctx->color_body_active : btn->color_active;
			shape = btn->shape == NULL ? ctx->shape_button : btn->shape;
		}
	} else {
		color = btn->color_passive == NULL ? ctx->color_body_passive : btn->color_passive;
		shape = btn->shape == NULL ? ctx->shape_button : btn->shape;
	}
	GUI_Rect rect = GUI_render_panel(ctx, irec->rect, color, shape);
	uint16_t len = strlen(btn->text);
	GUI_render_text(ctx, rect.x + ((rect.w - len * 8) >> 1), rect.y + ((rect.w - 16) >> 1) + shape->margin, color->fg, btn->text, GUI_font08x16);
}

void GUI_render_slider(GUI_Dispatcher* dsp, GUI_Context* ctx, GUI_ItemNode* node)
{
	GUI_Color* color_bg = ctx->color_button_down;
	GUI_Shape* shape_bg = ctx->shape_button_down;
	GUI_Shape* shape = ctx->shape_button;
	GUI_Color* color;

	GUI_ItemRecord* irec = &(dsp->items[node->index]);
	GUI_Slider* slider = (GUI_Slider*) irec->item.element;

	GUI_ItemStatus status = irec->item.status;
	color = status & GUI_STATUS_ACTIVE ? ctx->color_body_active : ctx->color_body_passive;
	uint16_t h = irec->rect.h;
	GUI_Rect rect = GUI_render_panel(ctx, irec->rect, color_bg, shape_bg);
	/** @todo add init phase for one-time adjustments */
	slider->margin = h - rect.h;
	uint32_t range = slider->max - slider->min;
	uint16_t pos = slider->value - slider->min;
	uint16_t w = rect.w - rect.h;
	rect.x += pos * w / range;
	rect.w = rect.h;
	GUI_render_panel(ctx, rect, color, shape);
}

void GUI_render_checkbox(GUI_Dispatcher* dsp, GUI_Context* ctx, GUI_ItemNode* node)
{
	GUI_ItemRecord* irec = &dsp->items[node->index];
	GUI_Checkbox* cb = (GUI_Checkbox*) irec->item.element;
	GUI_Color* color_active = ctx->color_body_active;
	GUI_Rect rect = irec->rect;
	SDL_FillRect(ctx->surf, &rect, color_active->fg);
	rect.x += 2;
	rect.y += 2;
	rect.w -= 4;
	rect.h -= 4;
	SDL_FillRect(ctx->surf, &rect, color_active->light);
	if (irec->item.status & GUI_STATUS_SELECTED) {
		rect.x += (rect.w - 8) >> 1;
		rect.y += (rect.h - 8) >> 1;
		rect.w = 8;
		rect.h = 8;
		SDL_FillRect(ctx->surf, &rect, color_active->fg);
	}
	GUI_render_text(
		ctx, irec->rect.x + irec->rect.w + 8, irec->rect.y,
		color_active->fg, cb->label, GUI_font08x16
	);
}

void GUI_render_group(GUI_Dispatcher* dsp, GUI_Context* ctx, GUI_ItemNode* node)
{
	GUI_ItemRecord* irec = &(dsp->items[node->index]);
	GUI_GenericGroup* group = (GUI_GenericGroup*) irec->item.element;
	uint16_t x = irec->rect.x + 2;
	uint16_t y = irec->rect.y + 2;
	uint16_t w = irec->rect.w - 4;
	uint16_t h = irec->rect.h - 4;
	GUI_Color* color = ctx->color_body_passive;
	SDL_FillRect(ctx->surf, &(SDL_Rect) { x, y, w, 2 }, color->dark);
	SDL_FillRect(ctx->surf, &(SDL_Rect) { x, y + h, w, 2 }, color->dark);
	SDL_FillRect(ctx->surf, &(SDL_Rect) { x, y, 2, h }, color->dark);
	SDL_FillRect(ctx->surf, &(SDL_Rect) { x + w, y, 2, h }, color->dark);
	GUI_render_text(ctx, x, y - 20, color->dark, group->title, GUI_font08x16);
}

void GUI_render_tabgroup(GUI_Dispatcher* dsp, GUI_Context* ctx, GUI_ItemNode* node)
{
	size_t item_index = node->index;
	GUI_ItemRecord* irec = &(dsp->items[item_index]);
	GUI_TabGroup* tg = (GUI_TabGroup*) irec->item.element;

	uint32_t cp_dark = ctx->color_body_passive->dark;
	uint32_t ca_dark = ctx->color_body_active->dark;
	uint32_t ca_light = ctx->color_body_active->light;
	uint32_t ca_fg = ctx->color_body_active->fg;

	uint16_t child_cnt = irec->child_cnt;
	uint16_t current = tg->current;
	uint16_t tab_width = (irec->rect.w - 4) / child_cnt;
	uint16_t head_h = tg->head_h;
	uint16_t x = irec->rect.x;
	uint16_t y = irec->rect.y;
	uint16_t baseline = y + ((head_h - 18) >> 1);

	SDL_Rect rect_v = { x + 2, y + 2, 2, head_h - 2 };
	SDL_Rect rect_h = { x + 4, y, tab_width - 4, 2 };

	item_index++;
	for (uint16_t i = 0; i < current; i++) {
		GUI_GenericGroup* tab = (GUI_GenericGroup*) dsp->items[item_index].item.element;
		GUI_render_text(ctx, rect_h.x + 4, baseline, cp_dark, tab->title, GUI_font08x16);
		SDL_FillRect(ctx->surf, &rect_v, cp_dark);
		SDL_FillRect(ctx->surf, &rect_h, cp_dark);
		rect_v.x += tab_width;
		rect_h.x += tab_width;
		item_index += 1 + dsp->items[item_index].subtree_cnt;
	}

	GUI_GenericGroup* tab = (GUI_GenericGroup*) dsp->items[item_index].item.element;
	GUI_render_text(ctx, rect_h.x + 4, baseline, ca_fg, tab->title, GUI_font08x16);
	SDL_FillRect(ctx->surf, &rect_v, ca_light);
	SDL_FillRect(ctx->surf, &(SDL_Rect){ x, y + head_h, rect_v.x - x, 2 }, ca_light);
	SDL_FillRect(ctx->surf, &rect_h, ca_light);
	rect_v.x += tab_width - 2;
	SDL_FillRect(ctx->surf, &rect_v, ca_dark);
	SDL_FillRect(
		ctx->surf, &(SDL_Rect){ rect_v.x + 2, y + head_h, x + irec->rect.w - rect_v.x, 2 }, ca_light
	);
	rect_h.x += tab_width;
	rect_v.x += tab_width;

	for (uint16_t i = current + 1; i < child_cnt; i++) {
		item_index += 1 + dsp->items[item_index].subtree_cnt;
		GUI_GenericGroup* tab = (GUI_GenericGroup*) dsp->items[item_index].item.element;
		GUI_render_text(ctx, rect_h.x + 4, baseline, cp_dark, tab->title, GUI_font08x16);
		SDL_FillRect(ctx->surf, &rect_v, cp_dark);
		SDL_FillRect(ctx->surf, &rect_h, cp_dark);
		rect_v.x += tab_width;
		rect_h.x += tab_width;
	}
}

void GUI_render_item_recursive(
	GUI_Dispatcher* dsp, GUI_Context* ctx, GUI_ItemNode* node
) {
	size_t item_index = node->index;
	GUI_ItemRecord* ir = &dsp->items[item_index];
	if ((ir->item.status & GUI_STATUS_VISIBLE) == 0) {
		return;
	}

	switch (ir->item.type) {
		case GUI_ITEM_WINDOW:
			GUI_render_window(dsp, ctx, node);
			break;
		case GUI_ITEM_CAPTION:
			GUI_render_caption(dsp, ctx, node);
			break;
		case GUI_ITEM_TABGROUP:
			GUI_render_tabgroup(dsp, ctx, node);
			break;
		case GUI_ITEM_GROUP:
			GUI_render_group(dsp, ctx, node);
			break;
		case GUI_ITEM_CHECKBOX:
			GUI_render_checkbox(dsp, ctx, node);
			break;
		case GUI_ITEM_DISPLAYPANEL:
			GUI_render_displaypanel(dsp, ctx, node);
			break;
		case GUI_ITEM_BUTTON:
			GUI_render_button(dsp, ctx, node);
			break;
		case GUI_ITEM_HSLIDER:
			GUI_render_slider(dsp, ctx, node);
			break;
	}

	if (ir->child_cnt) {
		GUI_ItemNode next_node = {
			.parent = node,
			.index = item_index + 1
		};

		for (uint16_t i = 0; i < ir->child_cnt; i++) {
			GUI_render_item_recursive(dsp, ctx, &next_node);
			next_node.index += 1 + dsp->items[next_node.index].subtree_cnt;
		}
	}
}

void GUI_render(GUI_Dispatcher* dsp, GUI_Context* ctx)
{
	SDL_FillRect(ctx->surf, NULL, 0);
	GUI_ItemNode node = { .parent = NULL };
	for (size_t i = 0; i < dsp->uid_cnt; i++) {
		node.index = dsp->uids[i].item_index;
		GUI_render_item_recursive(dsp, ctx, &node);
	}
}
