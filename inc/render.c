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

void GUI_render_window(GUI_Dispatcher* dsp, GUI_Context* ctx, GUI_ItemNode* node)
{
	GUI_Color* color_body;
	GUI_Color* color_title;
	GUI_Shape* shape_body;
	GUI_Shape* shape_title;

	GUI_ItemRecord* ir = &(dsp->items[node->index]);
	GUI_Window* win = (GUI_Window*) ir->item.data;

	if (ir->item.status == GUI_STATUS_ACTIVE) {
		color_body = win->color_body_active == NULL ? ctx->style_window.color_body_active : win->color_body_active;
		color_title = win->color_title_active == NULL ? ctx->style_window.color_title_active : win->color_title_active;
	} else {
		color_body = win->color_body_passive == NULL ? ctx->style_window.color_body_passive : win->color_body_passive;
		color_title = win->color_title_passive == NULL ? ctx->style_window.color_title_passive : win->color_title_passive;
	}
	shape_body = win->shape_body == NULL ? ctx->style_window.shape_body : win->shape_body;
	shape_title = win->shape_title == NULL ? ctx->style_window.shape_title : win->shape_title;
	SDL_Rect rect = ir->rect;
    SDL_FillRect(ctx->surf, &rect, color_body->bg);
	if (shape_body->bump_out) {
		rect.h = shape_body->bump_out;
		SDL_FillRect(ctx->surf, &rect, color_body->light);
		rect.y += ir->rect.h - shape_body->bump_out;
		SDL_FillRect(ctx->surf, &rect, color_body->dark);
		rect.y = ir->rect.y + rect.h;
		rect.w = rect.h;
		rect.h = ir->rect.h - shape_body->bump_out * 2;
		SDL_FillRect(ctx->surf, &rect, color_body->light);
		rect.x += ir->rect.w - rect.w;
		SDL_FillRect(ctx->surf, &rect, color_body->dark);
	}
	rect.x = ir->rect.x + shape_title->margin;
	rect.y = ir->rect.y + shape_title->margin;
	rect.w = ir->rect.w - shape_title->margin * 2;
	rect.h = 16 + shape_title->padding * 2;
    SDL_FillRect(ctx->surf, &rect, color_title->bg);
	GUI_render_text(ctx, rect.x + shape_title->padding, rect.y + shape_title->padding, color_title->fg, win->title, GUI_font08x16);
}

void GUI_render_checkbox(GUI_Dispatcher* dsp, GUI_Context* ctx, GUI_ItemNode* node)
{
	GUI_ItemRecord* irec = &dsp->items[node->index];
	GUI_Checkbox* cb = (GUI_Checkbox*) irec->item.data;
	GUI_Color* color_active = ctx->style_window.color_body_active;
	GUI_Rect rect = irec->rect;
	SDL_FillRect(ctx->surf, &rect, color_active->fg);
	rect.x += 2;
	rect.y += 2;
	rect.w -= 4;
	rect.h -= 4;
	SDL_FillRect(ctx->surf, &rect, color_active->light);
	if (irec->item.status & GUI_STATUS_SELECTED) {
		rect.x += 2;
		rect.y += 2;
		rect.w -= 4;
		rect.h -= 4;
		SDL_FillRect(ctx->surf, &rect, color_active->fg);
	}
	GUI_render_text(
		ctx, irec->rect.x + irec->rect.w + 8, irec->rect.y,
		color_active->fg, cb->label, GUI_font08x16
	);
}

void GUI_render_group(GUI_Dispatcher* dsp, GUI_Context* ctx, GUI_ItemNode* node)
{
	GUI_ItemRecord* ir = &(dsp->items[node->index]);
	GUI_GenericGroup* group = (GUI_GenericGroup*) ir->item.data;
	uint16_t x = ir->rect.x + 2;
	uint16_t y = ir->rect.y + 2;
	uint16_t w = ir->rect.w - 4;
	uint16_t h = ir->rect.h - 4;
	GUI_Color* color_passive = ctx->style_window.color_body_passive;
	SDL_FillRect(ctx->surf, &(SDL_Rect) { x, y, w, 2 }, color_passive->dark);
	SDL_FillRect(ctx->surf, &(SDL_Rect) { x, y + h, w, 2 }, color_passive->dark);
	SDL_FillRect(ctx->surf, &(SDL_Rect) { x, y, 2, h }, color_passive->dark);
	SDL_FillRect(ctx->surf, &(SDL_Rect) { x + w, y, 2, h }, color_passive->dark);
	GUI_render_text(ctx, x, y - 20, color_passive->dark, group->title, GUI_font08x16);
}

void GUI_render_tabgroup(GUI_Dispatcher* dsp, GUI_Context* ctx, GUI_ItemNode* node)
{
	size_t item_index = node->index;
	GUI_ItemRecord* ir = &(dsp->items[item_index]);
	GUI_Item* item = &ir->item;
	GUI_TabGroup* tg = (GUI_TabGroup*) item->data;

	uint16_t x = ir->rect.x + 2;
	uint16_t y = ir->rect.y + 2;
	uint16_t w = ir->rect.w - 4;
	uint16_t h = tg->head_h;

	uint16_t tab_width = w / ir->child_cnt;
	item_index++;
	SDL_Rect rect;
	uint16_t i;
	GUI_Color* color_passive = ctx->style_window.color_body_passive;
	GUI_Color* color_active = ctx->style_window.color_body_active;
	for (i = 0; i < tg->current; i++) {
		SDL_FillRect(ctx->surf, &(SDL_Rect) { x + i * tab_width, y, tab_width - 2, 2 }, color_passive->dark);
		SDL_FillRect(ctx->surf, &(SDL_Rect) { x + i * tab_width, y + 2, 2, h - 4 }, color_passive->dark);
		GUI_GenericGroup* tab = (GUI_GenericGroup*) dsp->items[item_index].item.data;
		GUI_render_text(ctx, x + i * tab_width + 4, y + 4, color_passive->dark, tab->title, GUI_font08x16);
		item_index += 1 + dsp->items[item_index].subtree_cnt;
	}
	SDL_FillRect(ctx->surf, &(SDL_Rect) { x + 2, y + h, tab_width * tg->current, 2 }, color_active->light);
	SDL_FillRect(ctx->surf, &(SDL_Rect) { x + 2 + tab_width * tg->current, y + 2, 2, h }, color_active->light);
	SDL_FillRect(ctx->surf, &(SDL_Rect) { x + 2 + tab_width * tg->current, y, tab_width, 2 }, color_active->light);
	SDL_FillRect(ctx->surf, &(SDL_Rect) { x + 2 + tab_width * tg->current + tab_width, y, 2, h - 2 }, color_passive->dark);
	SDL_FillRect(ctx->surf, &(SDL_Rect) { x + 2 + tab_width * tg->current + tab_width, y + h - 2,
		(ir->child_cnt - 1 - tg->current) * tab_width - 2, 2 }, color_passive->dark);
	GUI_GenericGroup* tab = (GUI_GenericGroup*) dsp->items[item_index].item.data;
	GUI_render_text(ctx, x + 2 + tab_width * tg->current + 4, y + 4, color_active->fg, tab->title, GUI_font08x16);
	
	item_index += 1 + dsp->items[item_index].subtree_cnt;
	for (i = tg->current + 1; i < ir->child_cnt; i++) {
		SDL_FillRect(ctx->surf, &(SDL_Rect) { x + i * tab_width + 6, y, tab_width - 8, 2 }, color_passive->dark);
		SDL_FillRect(ctx->surf, &(SDL_Rect) { x + i * tab_width + tab_width - 4, y + 2, 2, h - 4 }, color_passive->dark);
		GUI_GenericGroup* tab = (GUI_GenericGroup*) dsp->items[item_index].item.data;
		GUI_render_text(ctx, x + i * tab_width + 6, y + 4, color_passive->dark, tab->title, GUI_font08x16);
		item_index += 1 + dsp->items[item_index].subtree_cnt;
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
		case GUI_ITEM_TABGROUP:
			GUI_render_tabgroup(dsp, ctx, node);
			break;
		case GUI_ITEM_GROUP:
			GUI_render_group(dsp, ctx, node);
			break;
		case GUI_ITEM_CHECKBOX:
			GUI_render_checkbox(dsp, ctx, node);
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
