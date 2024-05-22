typedef struct {
	GUI_Rect offset_rect;
} GUI_ContentPane;

GUI_ComboEvent GUI_dispatcher_process_target_contentpane(
	GUI_Dispatcher* dsp, size_t index, GUI_ComboEvent cevt
) {
	GUI_ItemRecord* irec = &dsp->items[index];
	GUI_ContentPane* cp = irec->item.element;

	if (cevt.type == GUI_EVENT_DRAG) {
		int16_t dx = (cevt.evt.x - dsp->origin_x) * cp->offset_rect.w;
		int16_t dy = (cevt.evt.y - dsp->origin_y) * cp->offset_rect.h;
		if (dx) {
			dsp->origin_x = cevt.evt.x;
		}
		if (dy) {
			dsp->origin_y = cevt.evt.y;
		}
		size_t cnt = irec->subtree_cnt;
		while (cnt--) {
			index++;
			dsp->items[index].rect.x += dx;
			dsp->items[index].rect.y += dy;
		}
		cevt.result = GUI_OK;
	}

	return cevt;
}
