typedef enum {
	GUI_ITEM_NONE, GUI_ITEM_WINDOW, GUI_ITEM_CAPTION, GUI_ITEM_BUTTON,
	GUI_ITEM_GROUP, GUI_ITEM_TABGROUP, GUI_ITEM_TAB, GUI_ITEM_CHECKBOX,
	GUI_ITEM_DISPLAYPANEL, GUI_ITEM_HSLIDER,
	GUI_ITEM_COLORPICKER
} GUI_ItemType;

typedef struct {
	uint32_t bg;
	uint32_t fg;
	uint32_t light;
	uint32_t dark;
	uint32_t border;
	uint32_t outline;
} GUI_Color;

typedef struct {
	uint8_t bump_out;
	uint8_t bump_in;
	uint8_t border;
	uint8_t outline;
	uint8_t radius;
	uint8_t padding;
	uint8_t margin;
} GUI_Shape;

typedef struct {
	GUI_Color* color_active;
	GUI_Color* color_passive;
	GUI_Shape* shape;
	void* font;
	char* title;
} GUI_Window;

typedef struct {
	GUI_Color* color_active;
	GUI_Color* color_passive;
	GUI_Shape* shape;
	void* font;
	char* title;
} GUI_Caption;

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
	void* font_text;
	char* text;
	//void* target;
} GUI_Button;

typedef struct {
	uint16_t current;
	uint16_t head_h;
} GUI_TabGroup;

typedef struct {
	char* title;
	char* font;
} GUI_GenericGroup;

typedef struct {
	char *label;
} GUI_Checkbox;

typedef struct {
	GUI_Color* color_active;
	GUI_Color* color_passive;
	GUI_Shape* shape;
	char *label;
	char *svalue;
	uint32_t ivalue;
} GUI_DisplayPanel;

typedef struct {
	uint32_t value;
	uint32_t min;
	uint32_t max;
	uint16_t margin;
} GUI_Slider;
