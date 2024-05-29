typedef struct {
	uint32_t color;
} GUI_Placeholder;

GUI_ItemTree GUI_placeholder_create(
	GUI_Dispatcher* dsp, GUI_Rect rect, uint32_t color, GUI_EventType return_state)
{
	GUI_Placeholder* ph = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_Placeholder));
	ph->color = color;
	return (GUI_ItemTree) {
		.item = {
			.type = GUI_ITEM_PLACEHOLDER, .status = GUI_STATUS_VDA,
			.rect = rect,
			.element = ph,
			.return_state = return_state
		}
	};
}
