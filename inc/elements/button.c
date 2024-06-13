typedef struct {
	GUI_Color* color_active;
	GUI_Color* color_passive;
	GUI_Color* color_hover;
	GUI_Color* color_down;
	GUI_Color* color_disabled;
	GUI_Shape* shape;
	GUI_Shape* shape_hover;
	GUI_Shape* shape_down;
	GUI_Shape* shape_disabled;
	GUI_CommandType command_type;
	int32_t value;
	void* font_text;
	char* text;
} GUI_Button;

GUI_ItemTree GUI_button_create(
	GUI_Dispatcher* dsp, GUI_ID id, GUI_Rect rect, char* text,
	GUI_CommandType command_type, int32_t value
) {
	GUI_Button* btn = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_Button));
	*btn = (GUI_Button) {
		.text = text, .command_type = command_type, .value = value
	};

	return (GUI_ItemTree) {
		.item = {
			.id = id, .type = GUI_ITEM_BUTTON, .status = GUI_STATUS_VSHA,
			.rect = rect,
			.element = btn
		}
	};
}

GUI_ComboEvent GUI_dispatcher_process_target_button(
	GUI_Dispatcher* dsp, size_t index, GUI_ComboEvent cevt
) {
	if (cevt.type & (GUI_EVENT_DOWN | GUI_EVENT_UP)) {
		cevt.result = GUI_OK;
		if (cevt.type == GUI_EVENT_UP) {
			GUI_Button* btn = dsp->items[index].item.element;
			if (btn->command_type) {
				cevt.type = GUI_EVENT_CMD;
				cevt.cmd.type = btn->command_type;
				cevt.cmd.value = btn->value;
			}
		}
	} else {
		cevt.type = GUI_EVENT_NONE;
	}

	return cevt;
}
