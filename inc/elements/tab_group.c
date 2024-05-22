typedef struct {
	uint16_t current;
	uint16_t head_h;
} GUI_TabGroup;

GUI_Rect GUI_tabgroup_fix_rect(GUI_Context* ctx, GUI_Rect rect, GUI_TabGroup* tg)
{
	rect.y += tg->head_h;
	rect.h -= tg->head_h;
	return rect;
}

GUI_ComboEvent GUI_dispatcher_process_target_tabgroup(
	GUI_Dispatcher* dsp, size_t index, GUI_ComboEvent cevt
) {
	if (cevt.type == GUI_EVENT_UP) {
		GUI_ItemRecord* irec = &dsp->items[index];
		GUI_Rect rect = irec->rect;
		GUI_TabGroup* tg = irec->item.element;
		uint16_t child_cnt = irec->child_cnt;
		if (dsp->origin_y - rect.y < tg->head_h) {
			uint16_t tab_width = rect.w / child_cnt;
			uint16_t active = (dsp->origin_x - rect.x) / tab_width;
			if (tg->current == active) {
				return cevt;
			}			
			tg->current = active;
			for (uint16_t i = 0; i < child_cnt; i++) {
				index++;
				dsp->items[index].item.status =	i == active ? GUI_STATUS_VSA : GUI_STATUS_NONE;
				index += dsp->items[index].subtree_cnt;
			}
			cevt.result = GUI_OK;
		}
	}

	return cevt;
}
