typedef struct {
	GUI_Color* color_active;
	GUI_Color* color_passive;
	GUI_Shape* shape;
	void* font;
	char* title;
} GUI_Caption;

GUI_ComboEvent GUI_dispatcher_process_target_caption(
	GUI_Dispatcher* dsp, size_t index, GUI_ComboEvent cevt
) {
	if (cevt.type == GUI_EVENT_DRAG) {
		int16_t dx = cevt.evt.x - dsp->origin_x;
		int16_t dy = cevt.evt.y - dsp->origin_y;
		index = dsp->uids[dsp->last_uid_index].item_index;
		uint16_t cnt = dsp->items[index].subtree_cnt + 1;
		while (cnt--) {
			dsp->items[index].rect.x += dx;
			dsp->items[index].rect.y += dy;
			index++;
		}
		dsp->origin_x = cevt.evt.x;
		dsp->origin_y = cevt.evt.y;
		cevt.result = GUI_OK;
	}

	return cevt;
}
