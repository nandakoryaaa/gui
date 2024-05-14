
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
	//GUI_ItemType target_type;
	int32_t value;
	void* font_text;
	char* text;
} GUI_Button;
