typedef struct {
	char** items;
	uint16_t cnt;
	uint16_t pos;
	GUI_CommandType command_type;
} GUI_ComboBox;

typedef struct {
	char** items;
	uint16_t parent_index;
	uint16_t cnt;
	uint16_t pos;
	GUI_CommandType command_type;
} GUI_ItemList;

GUI_ComboEvent GUI_dispatcher_process_target_itemlist(
	GUI_Dispatcher* dsp, size_t index, GUI_ComboEvent cevt
) {
	GUI_ItemRecord* irec = &dsp->items[index];
	GUI_ItemList* itemlist = irec->item.element;
	GUI_Rect rect = irec->rect;
	GUI_Shape* shape = dsp->ctx->shape_list;
	uint16_t bump = shape->bump_in + shape->bump_out + shape->border + shape->outline;
	rect.y += bump;
	rect.h -= bump + bump;
	if (cevt.type & (GUI_EVENT_MOVE | GUI_EVENT_DOWN)) {
		uint16_t pos = (cevt.evt.y - rect.y) / 20;
		if (pos != itemlist->pos) {
			itemlist->pos = pos;
			cevt.result = GUI_OK;
		}
	} else if (cevt.type == GUI_EVENT_UP) {
		GUI_ItemRecord* irec = &dsp->items[itemlist->parent_index];
		GUI_ComboBox* cb = irec->item.element;
		cb->pos = itemlist->pos;
		irec->item.status &= ~GUI_STATUS_SELECTED;
		cevt.result = GUI_OK;
		GUI_dispatcher_remove_volatile(dsp);
	}
	cevt.type = GUI_EVENT_NONE;
	return cevt;
}

GUI_ItemTree GUI_itemlist_create(GUI_Dispatcher* dsp, GUI_ID id, size_t index)
{
	GUI_ItemRecord* irec = &dsp->items[index];
	GUI_ComboBox* cb = irec->item.element;
	GUI_ItemList* itemlist = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_ItemList));

	*itemlist = (GUI_ItemList) {
		.parent_index = index,
		.cnt = cb->cnt,
		.pos = cb->pos,
		.items = cb->items,
		.command_type = GUI_CMD_CLOSE
	};

	GUI_Rect rect = irec->rect;
	rect.y += rect.h;
	rect.h = 20 * cb->cnt;
	return (GUI_ItemTree) {
		.item = {
			.id = id, .type = GUI_ITEM_LIST, .status = GUI_STATUS_VSHA,
			.rect = rect,
			.element = itemlist,
			.return_state = GUI_EVENT_CMD
		}
	};
}

void GUI_itemlist_close(GUI_Dispatcher* dsp, GUI_ItemList* itemlist)
{
	GUI_dispatcher_process_target_combobox(
		dsp,
		itemlist->parent_index,
		(GUI_ComboEvent) {
			.type = GUI_EVENT_CMD, .cmd = (GUI_Command) { .type = GUI_CMD_CLOSE }
		}
	);
}
