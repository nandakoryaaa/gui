typedef struct {
	char *label;
} GUI_Checkbox;

GUI_ComboEvent GUI_dispatcher_process_target_checkbox(
	GUI_Dispatcher* dsp, size_t index, GUI_ComboEvent cevt
) {
	if (cevt.type == GUI_EVENT_UP) {
		dsp->items[index].item.status ^= GUI_STATUS_SELECTED;
		cevt.result = GUI_OK;
	}

	return cevt;
}
