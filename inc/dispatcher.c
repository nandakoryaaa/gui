GUI_Event GUI_convert_event(SDL_Event* event)
{
	GUI_Event gui_event = (GUI_Event) { .type = GUI_EVENT_NONE };
	if (event->type == SDL_MOUSEMOTION) {
		gui_event.type = GUI_EVENT_MOVE;
		gui_event.x = event->motion.x;
		gui_event.y = event->motion.y;
	} else if (event->type == SDL_MOUSEBUTTONDOWN) {
		gui_event.type = GUI_EVENT_DOWN;
		gui_event.x = event->button.x;
		gui_event.y = event->button.y;
	} else if (event->type == SDL_MOUSEBUTTONUP) {
		gui_event.type = GUI_EVENT_UP;
		gui_event.x = event->button.x;
		gui_event.y = event->button.y;
	}
	return gui_event;
}

void GUI_dispatcher_clear(GUI_Dispatcher* dsp)
{
	dsp->item_cnt = 0;
	dsp->uid_cnt = 0;
	dsp->uid = GUI_UID_FIRST;
}

GUI_IndexResult GUI_dispatcher_find_item(
	GUI_Dispatcher* dsp, size_t uid_index, GUI_ID id
) {
	size_t index = dsp->uids[uid_index].item_index;
	uint16_t cnt = dsp->items[index].subtree_cnt + 1;
	while (cnt--) {
		if (dsp->items[index].item.id == id) {
			return (GUI_IndexResult) { GUI_OK, index };
		}
		index++;
	}

	return (GUI_IndexResult) { GUI_ERROR };
}

GUI_IndexResult GUI_dispatcher_find_next_child(
	GUI_Dispatcher* dsp, GUI_ItemType type, size_t parent_index, size_t index
) {
	size_t end_index = parent_index + 1 + dsp->items[parent_index].child_cnt;
	if (index <= parent_index || index >= end_index) {
		return (GUI_IndexResult) { GUI_ERROR };
	}
	while (index < end_index) {
		if (dsp->items[index].item.type == type) {
			return (GUI_IndexResult) { GUI_OK, index };
		}
		index++;
	}
	return (GUI_IndexResult) { GUI_NONE };
}

uint16_t GUI_dispatcher_push_recursive(GUI_Dispatcher* dsp, GUI_ItemTree* it, size_t parent_offset)
{
	uint16_t item_cnt = dsp->item_cnt;
	dsp->items[item_cnt].item = it->item;
	dsp->items[item_cnt].parent_offset = parent_offset;
	dsp->item_cnt++;
	uint16_t cnt = 0;
	parent_offset = 1;
	for (uint16_t i = 0; i < it->child_cnt; i++) {
		uint16_t sub_cnt = GUI_dispatcher_push_recursive(dsp, &it->subtree[i], parent_offset);
		if (!sub_cnt) {
			return 0;
		}
		cnt += sub_cnt;
		parent_offset += sub_cnt;
	}

	dsp->items[item_cnt].child_cnt = it->child_cnt;
	dsp->items[item_cnt].subtree_cnt = cnt;

	return cnt + 1;
}

GUI_UID GUI_dispatcher_push_tree(GUI_Dispatcher* dsp, GUI_ItemTree* it)
{
	uint16_t item_cnt = dsp->item_cnt;
	uint16_t cnt = GUI_dispatcher_push_recursive(dsp, it, 0);
	if (!cnt) {
		dsp->item_cnt = item_cnt;
		return GUI_UID_NONE;
	}
	GUI_UID uid = dsp->uid;
	dsp->uids[dsp->uid_cnt].uid = uid;
	dsp->uids[dsp->uid_cnt].item_index = item_cnt;
	dsp->uid++;
	dsp->uid_cnt++;

	return uid;
}

void GUI_dispatcher_list_items(GUI_Dispatcher* dsp)
{
	static char* item_type_labels[] = {
		"None", "Window", "Caption", "Button", "Group", "TabGroup", "Tab", "Checkbox",
		"DisplayPanel", "HSlider", "ContentPane", "PlaceHolder", "BoundingBox"
	};
	printf("item cnt: %lu\n", dsp->item_cnt);
	printf("max uid: %lu\n", dsp->uid);
	for (uint16_t i = 0; i < dsp->item_cnt; i++) {
		printf("item #%u: type: %s, id: %d, child_cnt: %u, sub_cnt: %u, parent: -%d, rect: %d %d %d %d, absrect: %d %d %d %d\n",
			i, item_type_labels[dsp->items[i].item.type],
			dsp->items[i].item.id,
			dsp->items[i].child_cnt,
			dsp->items[i].subtree_cnt,
			dsp->items[i].parent_offset,
			dsp->items[i].item.rect.x, dsp->items[i].item.rect.y, dsp->items[i].item.rect.w, dsp->items[i].item.rect.h,
			dsp->items[i].rect.x, dsp->items[i].rect.y, dsp->items[i].rect.w, dsp->items[i].rect.h
		);
	}
}

void GUI_dispatcher_fix_rects_recursive(
	GUI_Context* ctx, GUI_Dispatcher* dsp, GUI_Rect rect, size_t index
) {
	GUI_ItemRecord* irec = &dsp->items[index];
	GUI_Rect item_rect = irec->item.rect;
	if (item_rect.x < 0) {
		item_rect.x += rect.x + rect.w;
	} else {
		item_rect.x += rect.x;
	}
	item_rect.y += rect.y;
	if (item_rect.w == 0) {
		item_rect.w = rect.w;
	}
	if (item_rect.h == 0) {
		item_rect.h = rect.h;
	}
	irec->rect = item_rect;
	GUI_ItemType type = irec->item.type;
	if (type == GUI_ITEM_WINDOW) {
		item_rect = GUI_window_fix_rect(ctx, item_rect, irec->item.element);
	} else if (type == GUI_ITEM_TABGROUP) {
		item_rect = GUI_tabgroup_fix_rect(ctx, item_rect, irec->item.element);
	} else if (type == GUI_ITEM_HSLIDER) {
		item_rect = GUI_slider_fix_rect(ctx, item_rect, irec->item.element);
	}

	for (uint16_t i = 0; i < irec->child_cnt; i++) {
		index++;
		GUI_dispatcher_fix_rects_recursive(ctx, dsp, item_rect, index);
		index += dsp->items[index].subtree_cnt;
	}
}

void GUI_dispatcher_fix_rects(GUI_Context* ctx, GUI_Dispatcher* dsp)
{
	GUI_Rect rect = { 0, 0, 0, 0 };
	for (size_t i = 0; i < dsp->uid_cnt; i++) {
		GUI_dispatcher_fix_rects_recursive(ctx, dsp, rect, dsp->uids[i].item_index);
	}
}

GUI_IndexResult GUI_dispatcher_find_target_recursive(
	GUI_Dispatcher* dsp, size_t index, uint16_t x, uint16_t y
) {
	GUI_ItemRecord* irec = &dsp->items[index];
	GUI_IndexResult res = { GUI_NONE };
	if ((irec->item.status & GUI_STATUS_VA) == 0) {
		return res;
	}
	GUI_Rect rect = irec->rect;
	if (
		x >= rect.x && y >= rect.y && x < rect.x + rect.w && y < rect.y + rect.h
	) {
		res.result = GUI_OK;
		res.index = index;
		for (uint16_t i = 0; i < irec->child_cnt; i++) {
			index++;
			GUI_IndexResult found_index = GUI_dispatcher_find_target_recursive(
				dsp, index, x, y
			);
			if (found_index.result == GUI_OK) {
				return found_index;
			}
			index += dsp->items[index].subtree_cnt;
		}
	}
	return res;
}

GUI_TargetResult GUI_dispatcher_find_target(
	GUI_Dispatcher* dsp, uint16_t x, uint16_t y
) {
	GUI_TargetResult target = { GUI_NONE };
	for (size_t i = 0; i < dsp->uid_cnt; i++) {
		GUI_IndexResult ir = GUI_dispatcher_find_target_recursive(
			dsp, dsp->uids[i].item_index, x, y
		);
		if (ir.result == GUI_OK) {
			target.result = GUI_OK;
			target.uid_index = i;
			target.item_index = ir.index;
		}
	}
	return target;
}

GUI_Result GUI_dispatcher_process_item_down(GUI_Dispatcher* dsp)
{
	GUI_ItemRecord* irec = &dsp->items[dsp->last_index];

	if (irec->item.type == GUI_ITEM_HSLIDER) {
		uint16_t x = dsp->origin_x;
		uint16_t y = dsp->origin_y;
		GUI_Slider* slider = irec->item.element;
		GUI_Rect rect = GUI_slider_adjust_rect(slider, irec->rect);
		// does it have buttons?
		if (irec->child_cnt) {
			rect.x += rect.h;
			rect.w -= (rect.h << 1);
		}
		uint32_t range = slider->max - slider->min;
		// thumb position according to value
		uint16_t thumb_pos = rect.x + (slider->value - slider->min) * rect.w / range;
		// is thumb hit?
		if (x >= thumb_pos && x < thumb_pos + rect.h) {
			// do nothing
			return GUI_NONE;
		}
		// try positioning thumb at x as its center
		thumb_pos = x - (rect.h >> 1);
		if (thumb_pos < rect.x) {
			slider->value = slider->min;
		} else if (thumb_pos >= rect.x + rect.w) {
			slider->value = slider->max;
		} else {
			uint32_t range = slider->max - slider->min;
			slider->value = slider->min + (thumb_pos - rect.x) * range / rect.w;
		}
		return GUI_OK;
	}

	return GUI_NONE;
}

GUI_Result update_contentpane(GUI_Dispatcher* dsp, size_t index, uint16_t value)
{
	GUI_ItemRecord* irec = &dsp->items[index];
	GUI_ContentPane* cp = irec->item.element;
	GUI_Rect rect = cp->offset_rect;

	GUI_IndexResult ir = GUI_dispatcher_find_next_child(dsp, GUI_ITEM_BOUNDINGBOX, index, index + 1);
	if (ir.result == GUI_OK) {
		uint16_t w = dsp->items[ir.index].rect.w;
		int16_t x = dsp->items[ir.index].rect.x;
		int16_t base_x = x - rect.x;
		int16_t new_pos = base_x - (w - irec->rect.w) * value / 255;
		int16_t dx = new_pos - x;
		cp->offset_rect.x = new_pos - base_x;
		size_t cnt = dsp->items[ir.index].subtree_cnt + 1;
		while (cnt--) {
			dsp->items[ir.index].rect.x += dx;
			ir.index++;
		}
		return GUI_OK;
	}
	return GUI_NONE;

}

GUI_Result GUI_process_parent_elements(GUI_Dispatcher* dsp, GUI_ItemRecord* irec)
{
	GUI_Result result = GUI_NONE;
	size_t index = dsp->last_index - irec->parent_offset;
	GUI_ItemRecord* irec_parent = &dsp->items[index];

	if (irec_parent->item.id == irec->item.parent_id) {
		if (irec->item.type == GUI_ITEM_BUTTON) {
			GUI_Button* btn = irec->item.element;
			if (irec_parent->item.type == GUI_ITEM_HSLIDER) {
				GUI_Slider* slider = irec_parent->item.element;
				slider->value += btn->value;
				if (slider->value > slider->max) {
					slider->value = slider->max;
				} else if (slider->value < slider->min) {
					slider->value = slider->min;
				}
				result = GUI_OK;
			}
		} else if (irec->item.type == GUI_ITEM_HSLIDER) {
			GUI_Slider* slider = irec->item.element;
			if (irec_parent->item.type == GUI_ITEM_CONTENTPANE) {
				result = update_contentpane(dsp, index, slider->value);
			}
		}
	}
	if (irec_parent->item.parent_id) {
		size_t last_index = dsp->last_index;
		dsp->last_index = index;
		result = GUI_process_parent_elements(dsp, irec_parent);
		dsp->last_index = last_index;
	}

	return result;
}

GUI_Result GUI_dispatcher_process_item_up(GUI_Dispatcher* dsp, uint16_t x, uint16_t y)
{
	GUI_ItemRecord* irec = &dsp->items[dsp->last_index];
	GUI_Result result = GUI_NONE;

	if (irec->item.type == GUI_ITEM_CHECKBOX) {
		irec->item.status ^= GUI_STATUS_SELECTED;
		result = GUI_OK;
	} else if (irec->item.type == GUI_ITEM_TABGROUP) {
		GUI_Rect rect = irec->rect;
		GUI_TabGroup* tg = irec->item.element;
		uint16_t child_cnt = irec->child_cnt;
		if (dsp->origin_y - rect.y < tg->head_h) {
			uint16_t tab_width = rect.w / child_cnt;
			uint16_t active = (dsp->origin_x - rect.x) / tab_width;
			size_t index = dsp->last_index + 1;
			tg->current = active;
			for (uint16_t i = 0; i < child_cnt; i++) {
				dsp->items[index].item.status =	i == active ? GUI_STATUS_VSA : GUI_STATUS_NONE;
				index += 1 + dsp->items[index].subtree_cnt;
			}
			result = GUI_OK;
		}
	}
	if (irec->item.parent_id) {
		result = GUI_process_parent_elements(dsp, irec);
	}

	return result;
}

GUI_Result GUI_dispatcher_process_item_drag(GUI_Dispatcher* dsp, uint16_t x, uint16_t y)
{
	GUI_ItemRecord* irec = &dsp->items[dsp->last_index];
	GUI_Result res = GUI_NONE;
	int16_t dx = x - dsp->origin_x;
	int16_t dy = y - dsp->origin_y;
	switch (irec->item.type) {
		case GUI_ITEM_HSLIDER: {
			if (dx != 0) {
				GUI_Slider* slider = irec->item.element;
				GUI_Rect rect = GUI_slider_adjust_rect(slider, irec->rect);
				if (irec->child_cnt) {
					rect.x += rect.h;
					rect.w -= (rect.h << 1);
				}
				uint32_t range = slider->max - slider->min;
				// thumb position according to value
				uint16_t thumb_pos = rect.x + (slider->value - slider->min) * rect.w / range;
				// last cursor offset within thumb
				uint16_t local_x = dsp->origin_x - thumb_pos;
				// new thumb pos at x with same cursor offset
				thumb_pos = x - local_x;
				if (thumb_pos < rect.x) {
					slider->value = slider->min;
				} else if (thumb_pos >= rect.x + rect.w) {
					slider->value = slider->max;
				} else {
					slider->value = slider->min + (thumb_pos - rect.x) * range / rect.w;
				}
				// correct thumb pos against new value because they don't always match
				thumb_pos = rect.x + (slider->value - slider->min) * rect.w / range;
				dsp->origin_x = thumb_pos + local_x;
			}
			res = GUI_OK;
	if (irec->item.parent_id) {
		res = GUI_process_parent_elements(dsp, irec);
	}

			break;
		}
		default: {
			size_t index = dsp->uids[dsp->last_uid_index].item_index;
			uint16_t cnt = dsp->items[index].subtree_cnt + 1;
			while (cnt--) {
				dsp->items[index].rect.x += dx;
				dsp->items[index].rect.y += dy;
				index++;
			}
			dsp->origin_x = x;
			dsp->origin_y = y;

			res = GUI_OK;
		}
	}

	return res;
}

GUI_Result GUI_dispatcher_process_event(GUI_Dispatcher* dsp, GUI_Event evt)
{
	size_t last_index = dsp->last_index;
	GUI_ItemStatus last_state = dsp->state;
	GUI_Result res = GUI_NONE;
	if (last_state == GUI_STATUS_NONE) {
		GUI_TargetResult target = GUI_dispatcher_find_target(dsp, evt.x, evt.y);
		if (target.result == GUI_OK) {
			GUI_ItemStatus st = dsp->items[target.item_index].item.status;
			if (evt.type == GUI_EVENT_MOVE && (st & GUI_STATUS_HOVERABLE)) {
				dsp->state = GUI_STATUS_HOVER;
			} else if (evt.type == GUI_EVENT_DOWN && (st & GUI_STATUS_SELECTABLE)) {
				dsp->state = GUI_STATUS_DOWN;
				dsp->origin_x = evt.x;
				dsp->origin_y = evt.y;
			}
			if (dsp->state != GUI_STATUS_NONE) {
				dsp->items[target.item_index].item.status |= dsp->state;
				dsp->last_uid_index = target.uid_index;
				dsp->last_index = target.item_index;
				if (dsp->state == GUI_STATUS_DOWN) {
					GUI_dispatcher_process_item_down(dsp);
				}
			}
		}
	} else if (last_state == GUI_STATUS_DOWN) {
		GUI_ItemRecord* irec = &dsp->items[last_index];
		if (evt.type == GUI_EVENT_MOVE) {
			if (irec->item.status & GUI_STATUS_DRAGGABLE) {
				irec->item.status |= GUI_STATUS_DRAG;
				res = GUI_dispatcher_process_item_drag(dsp, evt.x, evt.y);
			}
			if (res != GUI_OK) {
				GUI_TargetResult target = GUI_dispatcher_find_target(dsp, evt.x, evt.y);
				if (target.result == GUI_NONE || target.item_index != last_index) {
					irec->item.status &= ~(GUI_STATUS_HOVER | GUI_STATUS_DOWN | GUI_STATUS_DRAG);
					dsp->state = GUI_STATUS_NONE;
				}
			}
		} else if (evt.type == GUI_EVENT_UP) {
			irec->item.status &= ~(GUI_STATUS_DOWN | GUI_STATUS_DRAG);
			dsp->state = irec->item.status & GUI_STATUS_HOVER;
			GUI_dispatcher_process_item_up(dsp, evt.x, evt.y);
		}
	} else if (last_state == GUI_STATUS_HOVER) {
		GUI_ItemRecord* irec = &dsp->items[last_index];
		if (evt.type == GUI_EVENT_MOVE) {
			GUI_Rect rect = irec->rect;
			if (
				evt.x < rect.x || evt.x >= rect.x + rect.w
				|| evt.y < rect.y || evt.y >= rect.y + rect.h
			) {
				irec->item.status &= ~GUI_STATUS_HOVER;
				dsp->state = GUI_STATUS_NONE;
				GUI_TargetResult target = GUI_dispatcher_find_target(dsp, evt.x, evt.y);
				if (target.result == GUI_OK) {
					irec = &dsp->items[target.item_index];
					if (irec->item.status & GUI_STATUS_HOVERABLE) {
						irec->item.status |= GUI_STATUS_HOVER;
						dsp->last_uid_index = target.uid_index;
						dsp->last_index = target.item_index;
						dsp->state = GUI_STATUS_HOVER;
					}
				}
			}
		} else if (evt.type == GUI_EVENT_DOWN) {
			irec->item.status |= GUI_STATUS_DOWN;
			dsp->state = GUI_STATUS_DOWN;
			dsp->origin_x = evt.x;
			dsp->origin_y = evt.y;
			GUI_dispatcher_process_item_down(dsp);
		}
	}

	if (last_index != dsp->last_index || last_state != dsp->state) {
		return GUI_OK;
	}
	return res;
}
