
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
	}

	return cevt;
}
