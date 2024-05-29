typedef struct {
	char *label;
} GUI_Checkbox;

GUI_ItemTree GUI_checkbox_create(
	GUI_Dispatcher* dsp, GUI_ID id, GUI_Rect rect, char* label
) {
	GUI_Checkbox* cb = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_Checkbox));
	cb->label = label;
	return (GUI_ItemTree) {
		.item = {
			.id = id, .type = GUI_ITEM_CHECKBOX, .status = GUI_STATUS_VSA,
			.rect = rect,
			.element = cb
		}
	};
}

GUI_ComboEvent GUI_dispatcher_process_target_checkbox(
	GUI_Dispatcher* dsp, size_t index, GUI_ComboEvent cevt
) {
	if (cevt.type == GUI_EVENT_UP) {
		dsp->items[index].item.status ^= GUI_STATUS_SELECTED;
		cevt.result = GUI_OK;
	}

	return cevt;
}
