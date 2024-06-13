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
	dsp->state = GUI_STATUS_NONE;
	dsp->storage.last_mem = dsp->storage.mem;
	memset(dsp->storage.hash, 0, sizeof(dsp->storage.hash));
}

void GUI_dispatcher_free(GUI_Dispatcher* dsp)
{
	if (dsp->storage.mem != NULL) {
		free(dsp->storage.mem);
	}
}

void GUI_dispatcher_init(GUI_Dispatcher* dsp, size_t mem_size)
{
	dsp->storage.mem = malloc(mem_size);
	dsp->storage.size = mem_size;
	GUI_dispatcher_clear(dsp);
}

void* GUI_dispatcher_allocate_element(GUI_Dispatcher* dsp, uint16_t size)
{
	GUI_StorageHash* sth = &dsp->storage.hash[size];
	GUI_StorageElement* new;
	if (!sth->free_cnt) {
		new = dsp->storage.last_mem;
		dsp->storage.last_mem += sizeof(GUI_StorageElement) + size;
	} else {
		sth->free_cnt--;
		new = sth->last_free;
		sth->last_free = new->prev;
	}
	*new = (GUI_StorageElement) {
		.size = size, .status = GUI_OK, .prev = sth->last_used 
	};
	if (sth->last_used) {
		sth->last_used->next = new;
	}
	sth->last_used = new;
	sth->cnt++;
	return (void*) new + sizeof(GUI_StorageElement);
}

void GUI_dispatcher_free_storage(GUI_Dispatcher* dsp, void* ptr)
{
	if (ptr < dsp->storage.mem || ptr >= dsp->storage.last_mem) {
		printf(
			"element was not allocated: mem = %lu, last_mem = %lu, ptr = %lu\n",
			(size_t) dsp->storage.mem, (size_t) dsp->storage.last_mem, (size_t) ptr
		);
		return;
	}
	GUI_StorageElement* el = ptr - sizeof(GUI_StorageElement);
	GUI_StorageElement* prev = el->prev;
	if (prev) {
		prev->next = el->next;
	}
	GUI_StorageHash* sth = &dsp->storage.hash[el->size];
//	printf("freeing %lu bytes at %lu\n", el->size, el);
	el->status = GUI_NONE;
	sth->free_cnt++;
	sth->cnt--;

	if (sth->last_free) {
		sth->last_free->next = el;
	}

	el->prev = sth->last_free;
	el->next = NULL;
	sth->last_free = el;
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

void GUI_dispatcher_fix_rects_recursive(
	GUI_Dispatcher* dsp, GUI_Rect rect, size_t index
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
		item_rect = GUI_window_fix_rect(dsp->ctx, item_rect, irec->item.element);
	} else if (type == GUI_ITEM_TABGROUP) {
		item_rect = GUI_tabgroup_fix_rect(dsp->ctx, item_rect, irec->item.element);
	} else if (type == GUI_ITEM_HSLIDER) {
		item_rect = GUI_slider_fix_rect(dsp->ctx, item_rect, irec->item.element);
	}

	for (uint16_t i = 0; i < irec->child_cnt; i++) {
		index++;
		GUI_dispatcher_fix_rects_recursive(dsp, item_rect, index);
		index += dsp->items[index].subtree_cnt;
	}
}
/*
void GUI_dispatcher_fix_rects(GUI_Context* ctx, GUI_Dispatcher* dsp)
{
	GUI_Rect rect = { 0, 0, 0, 0 };
	for (size_t i = 0; i < dsp->uid_cnt; i++) {
		GUI_dispatcher_fix_rects_recursive(ctx, dsp, rect, dsp->uids[i].item_index);
	}
}
*/
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
	GUI_dispatcher_fix_rects_recursive(dsp, dsp->ctx->rect, item_cnt);
	return uid;
}

void GUI_dispatcher_remove_tree(GUI_Dispatcher* dsp, size_t index)
{
	GUI_ItemRecord* irec = &dsp->items[index];
	GUI_dispatcher_free_storage(dsp, irec->item.element);
	for (uint16_t i = 0; i < irec->subtree_cnt; i++) {
		index++;
		GUI_dispatcher_free_storage(dsp, dsp->items[index].item.element);
	}
}

void GUI_dispatcher_list_items(GUI_Dispatcher* dsp)
{
	static char* item_type_labels[] = {
		"None", "Window", "Caption", "Button", "Group", "TabGroup", "Tab", "Checkbox",
		"DisplayPanel", "HSlider", "ContentPane", "PlaceHolder", "BoundingBox", "ComboBox",
		"ItemList"
	};
	printf("item cnt: %u\n", dsp->item_cnt);
	printf("max uid: %lu\n", dsp->uid);
	for (uint16_t i = 0; i < dsp->item_cnt; i++) {
		printf("item #%u: type: %s, id: %u, child_cnt: %lu, sub_cnt: %lu, parent: -%lu, rect: %d %d %d %d, absrect: %d %d %d %d\n",
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
			target = (GUI_TargetResult) { GUI_OK, i, ir.index };
		}
	}

	return target;
}

GUI_ComboEvent GUI_dispatcher_process_target_recursive(
	GUI_Dispatcher* dsp, size_t index, GUI_ComboEvent cevt
) {
	GUI_ItemRecord* irec = &dsp->items[index];
	switch (irec->item.type) {
		case GUI_ITEM_BUTTON:
			cevt = GUI_dispatcher_process_target_button(dsp, index, cevt);
			break;
		case GUI_ITEM_CHECKBOX:
			cevt = GUI_dispatcher_process_target_checkbox(dsp, index, cevt);
			break;
		case GUI_ITEM_TABGROUP:
			cevt = GUI_dispatcher_process_target_tabgroup(dsp, index, cevt);
			break;
		case GUI_ITEM_CONTENTPANE:
			cevt = GUI_dispatcher_process_target_contentpane(dsp, index, cevt);
			break;
		case GUI_ITEM_HSLIDER:
			cevt = GUI_dispatcher_process_target_hslider(dsp, index, cevt);
			break;
		case GUI_ITEM_WINDOW:
			cevt = GUI_dispatcher_process_target_window(dsp, index, cevt);
			break;
		case GUI_ITEM_COMBOBOX:
			cevt = GUI_dispatcher_process_target_combobox(dsp, index, cevt);
			break;
		case GUI_ITEM_LIST:
			cevt = GUI_dispatcher_process_target_itemlist(dsp, index, cevt);
			break;
	}

	if (cevt.type != GUI_EVENT_NONE) {
		if (dsp->items[index].item.return_state == cevt.type) {
			if (irec->parent_offset) {
				cevt = GUI_dispatcher_process_target_recursive(
					dsp, index - irec->parent_offset, cevt
				);
			}
		}
	}

	return cevt;
}


void GUI_dispatcher_set_tree_status(
	GUI_Dispatcher* dsp, uint16_t index, GUI_ItemStatus status
) {
	uint16_t cnt = 1 + dsp->items[index].subtree_cnt;
	while (cnt--) {
		dsp->items[index++].item.status |= status;
	}
}

void GUI_dispatcher_clear_tree_status(
	GUI_Dispatcher* dsp, uint16_t index, GUI_ItemStatus status
) {
	status = ~status;
	uint16_t cnt = 1 + dsp->items[index].subtree_cnt;
	while (cnt--) {
		dsp->items[index++].item.status &= status;
	}
}

void GUI_dispatcher_remove_volatile(GUI_Dispatcher* dsp)
{
	dsp->volatile_evt = GUI_EVENT_NONE;
	if (!dsp->uid_cnt) {
		return;
	}
	GUI_ItemRecord* irec = &dsp->items[dsp->volatile_index];
	if (irec->item.type == GUI_ITEM_LIST) {
		GUI_itemlist_close(dsp, irec->item.element);
	}
	dsp->uid_cnt--;
	dsp->item_cnt = dsp->volatile_index;
	GUI_dispatcher_remove_tree(dsp, dsp->item_cnt);
}

GUI_Result GUI_dispatcher_check_volatile(GUI_Dispatcher* dsp, GUI_Event evt)
{
	if (dsp->volatile_evt != GUI_EVENT_NONE && dsp->volatile_evt == evt.type) {
		if (!(dsp->items[dsp->last_index].item.status & GUI_STATUS_VOLATILE)) {
			GUI_dispatcher_remove_volatile(dsp);
			return GUI_OK;
		}
	}
	return GUI_NONE;
}

GUI_ComboEvent GUI_dispatcher_process_event(GUI_Dispatcher* dsp, GUI_Event evt)
{
	GUI_TargetResult target = GUI_dispatcher_find_target(dsp, evt);
	GUI_ComboEvent cevt = { .type = evt.type, .result = GUI_NONE, .evt = evt };
	uint16_t state = (dsp->state != GUI_STATUS_NONE) | ((target.result == GUI_OK) << 1);
	uint16_t same_target = (state == 3) & (dsp->last_index == target.index);

	switch (state) {
		case 0: // dsp state is none and target is none, nothing to do
			return cevt;
		case 1: // dsp state is some and target is none, clear dsp state
			dsp->items[dsp->last_index].item.status &= GUI_STATUS_CLEAR;
			dsp->state = GUI_STATUS_NONE;
			cevt.result = GUI_OK;
			GUI_dispatcher_check_volatile(dsp, evt);
			return cevt;
		case 2: // dsp state is none and target is some, set dsp state
			dsp->last_uid_index = target.uid_index;
			dsp->last_index = target.index;
			dsp->state = GUI_STATUS_HOVER;
			break;
		case 3: // dsp state is some and target is some, update dsp state
			if (cevt.type == GUI_EVENT_MOVE) {
				if ((dsp->items[dsp->last_index].item.status & GUI_STATUS_DRAGREADY) == GUI_STATUS_DRAGREADY) {
					cevt.type = GUI_EVENT_DRAG;
				} else if (!same_target) {
					dsp->items[dsp->last_index].item.status &= GUI_STATUS_CLEAR;
					dsp->last_uid_index = target.uid_index;
					dsp->last_index = target.index;
					dsp->state = GUI_STATUS_HOVER;
					cevt.result = GUI_OK;
					GUI_dispatcher_check_volatile(dsp, evt);
				}// else { // just moving inside last target, nothing to do
					// return cevt;
					// except when moving inside item list...
				//}
			} else if (cevt.type == GUI_EVENT_DOWN) {
				dsp->state = GUI_STATUS_DOWN;
				dsp->origin_x = evt.x;
				dsp->origin_y = evt.y;
				cevt.result = GUI_dispatcher_check_volatile(dsp, evt);
			} else if (cevt.type == GUI_EVENT_UP) {
				GUI_Item* item = &dsp->items[dsp->last_index].item;
				GUI_ItemStatus was_down = item->status & GUI_STATUS_DOWN;
				dsp->state = GUI_STATUS_HOVER;
				item->status &= GUI_STATUS_CLEAR;
				item->status |= GUI_STATUS_HOVER;
				if (!was_down) {
					return cevt;
				}
			}
	}

	dsp->items[dsp->last_index].item.status |= dsp->state;
	return GUI_dispatcher_process_target_recursive(dsp, dsp->last_index, cevt);
}
