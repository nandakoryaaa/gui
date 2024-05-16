typedef struct {
	char *label;
} GUI_Checkbox;

GUI_Result GUI_dispatcher_process_target_checkbox(GUI_Dispatcher* dsp, size_t index, GUI_Event evt)
{
	if (evt.type == GUI_EVENT_UP) {
		dsp->items[index].item.status ^= GUI_STATUS_SELECTED;
		return GUI_OK;
	}

	return GUI_NONE;
}
