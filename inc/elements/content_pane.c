typedef struct {
	GUI_Rect offset_rect;
} GUI_ContentPane;

GUI_Result GUI_dispatcher_process_target_contentpane(GUI_Dispatcher* dsp, size_t index, GUI_Event evt)
{
	GUI_ItemRecord* irec = &dsp->items[index];
	GUI_ContentPane* cp = irec->item.element;
	if (dsp->state == GUI_STATUS_DRAG) {
		int16_t dx = (evt.x - dsp->origin_x) * cp->offset_rect.w;
		int16_t dy = (evt.y - dsp->origin_y) * cp->offset_rect.h;
		if (dx) {
			dsp->origin_x = evt.x;
		}
		if (dy) {
			dsp->origin_y = evt.y;
		}
		size_t cnt = irec->subtree_cnt;
		while (cnt--) {
			index++;
			dsp->items[index].rect.x += dx;
			dsp->items[index].rect.y += dy;
		}
		return GUI_OK;
	}
	return GUI_NONE;
}
