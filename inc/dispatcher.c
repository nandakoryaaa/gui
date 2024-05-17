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
	GUI_Dispatcher* dsp, size_t index, GUI_Event evt
) {
	GUI_ItemRecord* irec = &dsp->items[index];
	GUI_IndexResult res = { GUI_NONE };
	GUI_ItemStatus status = irec->item.status;
	GUI_Rect rect = irec->rect;
	if (
		(status & GUI_STATUS_VA) == 0
		|| evt.x < rect.x || evt.y < rect.y
		|| evt.x >= rect.x + rect.w || evt.y >= rect.y + rect.h
	) {
		return res;
	}

	res.result = GUI_OK;
	res.index = index;

	for (uint16_t i = 0; i < irec->child_cnt; i++) {
		index++;
		GUI_IndexResult child_index = GUI_dispatcher_find_target_recursive(
			dsp, index, evt
		);
		if (child_index.result == GUI_OK) {
			return child_index;
		}
		index += dsp->items[index].subtree_cnt;
	}
	
	return res;
}

GUI_TargetResult GUI_dispatcher_find_target(GUI_Dispatcher* dsp, GUI_Event evt)
{
	GUI_TargetResult target = { GUI_NONE };
	for (size_t i = 0; i < dsp->uid_cnt; i++) {
		GUI_IndexResult ir = GUI_dispatcher_find_target_recursive(
			dsp, dsp->uids[i].item_index, evt
		);
		if (ir.result == GUI_OK) {
			target.result = GUI_OK;
			target.uid_index = i;
			target.item_index = ir.index;
		}
	}

	return target;
}

GUI_Result GUI_dispatcher_process_target_recursive(
	GUI_Dispatcher* dsp, size_t index, GUI_Event evt
) {
	GUI_Result res = GUI_NONE;
	GUI_ItemRecord* irec = &dsp->items[index];
	switch (irec->item.type) {
		case GUI_ITEM_BUTTON:
			res = GUI_dispatcher_process_target_button(dsp, index, evt);
			break;
		case GUI_ITEM_CHECKBOX:
			res = GUI_dispatcher_process_target_checkbox(dsp, index, evt);
			break;
		case GUI_ITEM_TABGROUP:
			res = GUI_dispatcher_process_target_tabgroup(dsp, index, evt);
			break;
		case GUI_ITEM_CONTENTPANE:
			res = GUI_dispatcher_process_target_contentpane(dsp, index, evt);
			break;
		case GUI_ITEM_HSLIDER:
			res = GUI_dispatcher_process_target_hslider(dsp, index, evt);
			break;
		case GUI_ITEM_CAPTION:
			res = GUI_dispatcher_process_target_caption(dsp, index, evt);
			break;
	}

	GUI_ItemStatus return_state = dsp->items[index].item.return_state;
	if (return_state && dsp->state == return_state) {
		if (irec->parent_offset) {
			GUI_Result res_parent = GUI_dispatcher_process_target_recursive(
				dsp, index - irec->parent_offset, evt
			);
			if (res_parent == GUI_OK) {
				res = GUI_OK;
			}
		}
	}
	return res;
}

GUI_Result GUI_dispatcher_process_event(GUI_Dispatcher* dsp, GUI_Event evt)
{
	GUI_TargetResult target = GUI_dispatcher_find_target(dsp, evt);

	uint16_t needs_state_init = 0;
	GUI_Item* item = &dsp->items[dsp->last_index].item;

	if (dsp->state == GUI_STATUS_NONE) {
		if (target.result != GUI_OK) {
			return GUI_NONE;
		}
		needs_state_init = 1;
	} else if (
		dsp->state != GUI_STATUS_DRAG
		|| (item->status & GUI_STATUS_DRAGGABLE) == 0
	) {
		needs_state_init = (
			target.result != GUI_OK
			|| target.item_index != dsp->last_index
		);
	}

	if (needs_state_init) {
		if (dsp->state != GUI_STATUS_NONE) {
			item->status &= ~(GUI_STATUS_HOVER | GUI_STATUS_DOWN | GUI_STATUS_DRAG);
			dsp->state = GUI_STATUS_NONE;
		}

		if (target.result == GUI_OK) {
			dsp->items[target.item_index].item.status |= GUI_STATUS_HOVER;
			dsp->last_uid_index = target.uid_index;
			dsp->last_index = target.item_index;
			dsp->state = GUI_STATUS_HOVER;
		}
		return GUI_OK;
	}

	if (evt.type == GUI_EVENT_MOVE) {
		if (dsp->state == GUI_STATUS_DOWN) {
			dsp->state = GUI_STATUS_DRAG;
		} else if (dsp->state == GUI_STATUS_HOVER) {
			return GUI_NONE;
		}
	} else if (evt.type == GUI_EVENT_DOWN) {
		dsp->state = GUI_STATUS_DOWN;
		dsp->origin_x = evt.x;
		dsp->origin_y = evt.y;
	} else if (evt.type == GUI_EVENT_UP) {
		GUI_ItemStatus was_down = item->status & GUI_STATUS_DOWN;
		dsp->state = GUI_STATUS_HOVER;
		item->status &= ~(GUI_STATUS_DOWN | GUI_STATUS_DRAG);
		item->status |= GUI_STATUS_HOVER;
		if (!was_down) {
			return GUI_NONE;
		}
	}

	item->status |= dsp->state;

	return GUI_dispatcher_process_target_recursive(dsp, dsp->last_index, evt);
}
