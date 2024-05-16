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
	int32_t value;
	void* font_text;
	char* text;
} GUI_Button;

GUI_Result GUI_dispatcher_process_target_button(GUI_Dispatcher* dsp, size_t index, GUI_Event evt)
{
	if (evt.type & (GUI_EVENT_DOWN | GUI_EVENT_UP)) {
		return GUI_OK;
	}

	return GUI_NONE;
}
