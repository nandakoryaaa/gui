GUI_IndexResult GUI_dispatcher_get_index_from_uid(GUI_Dispatcher* dsp, GUI_UID uid)
{
	for (size_t i = 0; i < dsp->item_cnt; i++) {
		if (dsp->uids[i].uid == uid) {
			return (GUI_IndexResult) { GUI_OK, dsp->uids[i].item_index };
		}
	}

	return (GUI_IndexResult) { GUI_ERROR, 0 };
}

GUI_UID GUI_dispatcher_get_uid_from_index(GUI_Dispatcher* dsp, size_t index)
{
	if (index < dsp->item_cnt) {
		return dsp->uids[index].uid;
	}

	return GUI_UID_NONE;
}

void GUI_dispatcher_clear(GUI_Dispatcher* dsp)
{
	dsp->item_cnt = 0;
	dsp->uid_cnt = 0;
	dsp->uid = GUI_UID_FIRST;
}

GUI_UID GUI_dispatcher_push_item(GUI_Dispatcher* dsp, GUI_Item* item)
{
	if (dsp->item_cnt == GUI_MAX_ITEMS) {
		return GUI_UID_NONE;
	}
	GUI_UID uid = dsp->uid;
	dsp->uids[dsp->uid_cnt].uid = uid;
	dsp->uids[dsp->uid_cnt].item_index = dsp->item_cnt;
	dsp->uid_cnt++;
	dsp->uid++;
	dsp->items[dsp->item_cnt++].item = *item;
	dsp->items[dsp->item_cnt++].child_cnt = 0;
	dsp->items[dsp->item_cnt++].subtree_cnt = 0;

	return uid;
}

uint16_t GUI_dispatcher_push_recursive(GUI_Dispatcher* dsp, GUI_ItemTree* it)
{
	if (dsp->item_cnt + 1 + it->child_cnt >= GUI_MAX_ITEMS) {
		return 0;
	}
	
	uint16_t item_cnt = dsp->item_cnt;
	dsp->items[item_cnt].item = it->item;
	dsp->item_cnt++;
	uint16_t cnt = 0;

	for (uint16_t i = 0; i < it->child_cnt; i++) {
		uint16_t sub_cnt = GUI_dispatcher_push_recursive(dsp, &it->subtree[i]);
		if (!sub_cnt) {
			return 0;
		}
		cnt += sub_cnt;
	}

	dsp->items[item_cnt].child_cnt = it->child_cnt;
	dsp->items[item_cnt].subtree_cnt = cnt;

	return cnt + 1;
}

GUI_UID GUI_dispatcher_push_tree(GUI_Dispatcher* dsp, GUI_ItemTree* it)
{
	size_t item_cnt = dsp->item_cnt;
	uint16_t cnt = GUI_dispatcher_push_recursive(dsp, it);
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
	printf("item cnt: %lu\n", dsp->item_cnt);
	printf("max uid: %lu\n", dsp->uid);
	for (uint16_t i = 0; i < dsp->item_cnt; i++) {
		GUI_UID uid = GUI_dispatcher_get_uid_from_index(dsp, i);
		printf("item #%u: uid = %lu, type = %s, child_cnt: %u, sub_cnt: %u, rect: %d %d %d %d, absrect: %d %d %d %d\n",
			i, uid, item_type_labels[dsp->items[i].item.type],
			dsp->items[i].child_cnt,
			dsp->items[i].subtree_cnt,
			dsp->items[i].item.rect.x, dsp->items[i].item.rect.y, dsp->items[i].item.rect.w, dsp->items[i].item.rect.h,
			dsp->items[i].rect.x, dsp->items[i].rect.y, dsp->items[i].rect.w, dsp->items[i].rect.h
		);
	}
}

void GUI_dispatcher_fix_rects_recursive(
	GUI_Dispatcher* dsp, GUI_Rect rect, size_t item_index
) {
	GUI_ItemRecord* irec = &dsp->items[item_index];
	GUI_Rect item_rect = irec->item.rect;

	irec->rect = (GUI_Rect) {
		item_rect.x + rect.x, item_rect.y + rect.y,
		item_rect.w ? item_rect.w : rect.w, item_rect.h ? item_rect.h : rect.h
	};

	item_index++;
	for (uint16_t i = 0; i < irec->child_cnt; i++) {
		GUI_dispatcher_fix_rects_recursive(dsp, irec->rect, item_index);
		item_index += 1 + dsp->items[item_index].subtree_cnt;
	}
}

void GUI_dispatcher_fix_rects(GUI_Dispatcher* dsp)
{
	GUI_Rect rect = { 0, 0, 0, 0 };
	for (size_t i = 0; i < dsp->uid_cnt; i++) {
		GUI_dispatcher_fix_rects_recursive(dsp, rect, dsp->uids[i].item_index);
	}
}

GUI_IndexResult GUI_dispatcher_find_target_recursive(
	GUI_Dispatcher* dsp, size_t item_index, uint16_t x, uint16_t y
) {
	GUI_ItemRecord* irec = &dsp->items[item_index];
	GUI_IndexResult res = { GUI_ERROR };
	if ((irec->item.status & GUI_STATUS_VA) == 0) {
		return res;
	}
	GUI_Rect rect = irec->rect;
	if (
		x >= rect.x && y >= rect.y	&& x < rect.x + rect.w
		&& y < rect.y + rect.h
	) {
		res.result = GUI_OK;
		res.index = item_index;
		item_index++;
		for (uint16_t i = 0; i < irec->child_cnt; i++) {
			GUI_IndexResult found_index = GUI_dispatcher_find_target_recursive(
				dsp, item_index, x, y
			);
			if (found_index.result == GUI_OK) {
				return found_index;
			}
			item_index += 1 + dsp->items[item_index].subtree_cnt;
		}
	}
	return res;
}

GUI_TargetResult GUI_dispatcher_find_target(
	GUI_Dispatcher* dsp, uint16_t x, uint16_t y
) {
	GUI_TargetResult target = { GUI_ERROR };
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

GUI_Result GUI_process_item(
	GUI_Dispatcher* dsp, GUI_TargetResult tr, uint16_t x, uint16_t y
) {
	size_t item_index = tr.item_index;
	GUI_ItemRecord* irec = &dsp->items[item_index];
	GUI_Item item = irec->item;
	GUI_Rect rect = irec->rect;
	uint16_t local_x = x - rect.x;
	uint16_t local_y = y - rect.y;
	uint16_t child_cnt = irec->child_cnt;

	if (item.type == GUI_ITEM_TABGROUP) {
		GUI_TabGroup* tg = item.data;
		if (local_y < tg->head_h) {
			uint16_t tab_width = rect.w / child_cnt;
			uint16_t active = local_x / tab_width;
			item_index++;
			tg->current = active;
			for (uint16_t i = 0; i < child_cnt; i++) {
				dsp->items[item_index].item.status =
					i == active ? GUI_STATUS_VSA : GUI_STATUS_NONE;
				item_index += 1 + dsp->items[item_index].subtree_cnt;
			}
		}
	} else if (item.type == GUI_ITEM_CHECKBOX) {
		irec->item.status ^= GUI_STATUS_SELECTED;
	}
}
