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

void GUI_dispatcher_free_element(GUI_Dispatcher* dsp, void* ptr)
{
	if (ptr < dsp->storage.mem || ptr >= dsp->storage.last_mem) {
		//printf(
		//	"element was not allocated: mem = %lu, last_mem = %lu, ptr = %lu\n",
		//	(size_t) dsp->storage.mem, (size_t) dsp->storage.last_mem, (size_t) ptr
		//);
		return;
	}
	GUI_StorageElement* el = ptr - sizeof(GUI_StorageElement);
	GUI_StorageElement* prev = el->prev;
	if (prev) {
		prev->next = el->next;
	}
	GUI_StorageHash* sth = &dsp->storage.hash[el->size];
	//printf("freeing %u bytes at %lu\n", el->size, (uint64_t) el);
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

GUI_IndexResult GUI_dispatcher_find_item_by_id(
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

GUI_IndexResult GUI_dispatcher_find_uid_index(GUI_Dispatcher* dsp, GUI_UID uid)
{
	for (uint16_t i = 0; i < dsp->uid_cnt; i++) {
		if (dsp->uids[i].uid == uid) {
			return (GUI_IndexResult) { GUI_OK, i };
		}
	}

	return (GUI_IndexResult) { GUI_ERROR };
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

void GUI_dispatcher_move_tree_to_top(GUI_Dispatcher* dsp, uint16_t uid_index)
{
	if (!dsp->uid_cnt || dsp->uid_cnt - 1 == uid_index) {
		return;
	}
	GUI_UIDRecord urec = dsp->uids[uid_index];
	for (uint16_t i = uid_index; i < dsp->uid_cnt - 1; i++) {
		dsp->uids[i] = dsp->uids[i + 1];
	}
	dsp->uids[dsp->uid_cnt - 1] = urec;
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

void GUI_dispatcher_remove_tree(GUI_Dispatcher* dsp, uint16_t uid_index)
{
	GUI_UIDRecord urec = dsp->uids[uid_index];
	uint16_t index = urec.item_index;
	dsp->uid_cnt--;
	for (uint16_t i = uid_index; i < dsp->uid_cnt; i++) {
		dsp->uids[i] = dsp->uids[i + 1];
	}
	uint16_t subtree_cnt = dsp->items[index].subtree_cnt + 1;
	for (uint16_t i = 0; i < dsp->uid_cnt; i++) {
		if (dsp->uids[i].item_index > index) {
			dsp->uids[i].item_index -= subtree_cnt;
		}
	}
	for (uint16_t i = 0; i < subtree_cnt; i++) {
		GUI_dispatcher_free_element(dsp, dsp->items[index + i].item.element);
	}
	dsp->item_cnt -= subtree_cnt;
	for (uint16_t i = index; i < dsp->item_cnt; i++) {
		dsp->items[i] = dsp->items[i + subtree_cnt];
	}
	printf("dsp item cnt: %u\n", dsp->item_cnt);
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
//	dsp->uid_cnt--;
//	dsp->item_cnt = dsp->volatile_index;
	GUI_dispatcher_remove_tree(dsp, dsp->uid_cnt - 1);
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

GUI_Result GUI_dispatcher_switch_to(GUI_Dispatcher* dsp, uint16_t uid_index)
{
	if (dsp->uid_cnt - 1 != uid_index) {
		GUI_dispatcher_move_tree_to_top(dsp, uid_index);
		dsp->last_uid_index = dsp->uid_cnt - 1;
		return GUI_OK;
	}
	return GUI_NONE;
}

GUI_ComboEvent GUI_dispatcher_process_event(GUI_Dispatcher* dsp, GUI_Event evt)
{
	GUI_TargetResult target = GUI_dispatcher_find_target(dsp, evt);
	GUI_ComboEvent cevt = { .type = evt.type, .result = GUI_NONE, .evt = evt };
	uint16_t old_state = dsp->state != GUI_STATUS_NONE;
	uint16_t new_state = target.result == GUI_OK;
	uint16_t same_target = !(old_state | new_state) | ((old_state & new_state) & (dsp->last_index == target.index));
	if ((cevt.type == GUI_EVENT_MOVE) & old_state) { // check if we're dragging
		if ((dsp->items[dsp->last_index].item.status & GUI_STATUS_DRAGREADY) == GUI_STATUS_DRAGREADY) {
			cevt.type = GUI_EVENT_DRAG; // if draggable, new state does not matter
			same_target = 1;
		}
	}
	if (!same_target) {
		if (old_state) { // clear old state if exists
			dsp->items[dsp->last_index].item.status &= GUI_STATUS_CLEAR;
			dsp->state = GUI_STATUS_NONE;
		}
		if (new_state) { // set new state if exists
			dsp->last_uid_index = target.uid_index;
			dsp->last_index = target.index;
			dsp->state = GUI_STATUS_HOVER;
		}
		GUI_dispatcher_check_volatile(dsp, evt);
		cevt.result = GUI_OK;
	}
	if (cevt.type == GUI_EVENT_DOWN) {
		dsp->state = GUI_STATUS_DOWN;
		dsp->origin_x = evt.x;
		dsp->origin_y = evt.y;
		cevt.result = GUI_dispatcher_check_volatile(dsp, evt);
		if (new_state && GUI_dispatcher_switch_to(dsp, target.uid_index)) {
			cevt.result = GUI_OK;
		}
	} else if (cevt.type == GUI_EVENT_UP && new_state) {
		GUI_Item* item = &dsp->items[dsp->last_index].item;
		new_state = item->status & GUI_STATUS_DOWN;
		dsp->state = GUI_STATUS_HOVER;
		item->status &= GUI_STATUS_CLEAR;
	}
	if (new_state) {
		dsp->items[dsp->last_index].item.status |= dsp->state;
		return GUI_dispatcher_process_target_recursive(dsp, dsp->last_index, cevt);
	}
	return cevt;
}
