
typedef uint64_t GUI_UID;
typedef SDL_Rect GUI_Rect;

static char* item_type_labels[] = {
	"None", "Window", "Button", "Group", "TabGroup", "Tab", "Checkbox"
};

typedef enum {
	GUI_ERROR, GUI_OK
} GUI_Result;

typedef enum {
	GUI_ITEM_NONE, GUI_ITEM_WINDOW, GUI_ITEM_BUTTON,
	GUI_ITEM_GROUP, GUI_ITEM_TABGROUP, GUI_ITEM_TAB, GUI_ITEM_CHECKBOX
} GUI_ItemType;

typedef enum {
	GUI_STATUS_NONE = 0, GUI_STATUS_VISIBLE = 1, GUI_STATUS_SELECTABLE = 2,
	GUI_STATUS_ACTIVE = 4, GUI_STATUS_VA = 5, GUI_STATUS_VSA = 7,
	GUI_STATUS_DISABLED = 8, GUI_STATUS_HOVER = 16, GUI_STATUS_PRESSED = 32,
	GUI_STATUS_SELECTED = 64
} GUI_ItemStatus;

typedef struct GUI_Item {
	uint32_t id;
	GUI_ItemType type;
	GUI_ItemStatus status;
	GUI_Rect rect;
	void* data;
} GUI_Item;

typedef struct {
	GUI_Result result;
	size_t index;
} GUI_IndexResult;

typedef struct {
	GUI_Result result;
	GUI_Item item;
} GUI_ItemResult;

typedef struct {
	GUI_Result result;
	size_t uid_index;
	size_t item_index;
} GUI_TargetResult;

typedef struct {
	GUI_UID uid;
	size_t item_index;
} GUI_UIDRecord;

typedef struct {
	GUI_Item item;
	GUI_Rect rect;
	size_t child_cnt;
	size_t subtree_cnt;
} GUI_ItemRecord;

typedef struct GUI_ItemTree GUI_ItemTree;
struct GUI_ItemTree {
	GUI_Item item;
	uint16_t child_cnt;
	GUI_ItemTree* subtree;
};

typedef struct GUI_ItemNode GUI_ItemNode;
struct GUI_ItemNode {
	GUI_ItemNode* parent;
	size_t index;
};

typedef struct {
	uint32_t bg;
	uint32_t fg;
	uint32_t light;
	uint32_t dark;
	uint32_t border;
} GUI_Color;

typedef struct {
	uint8_t bump_in;
	uint8_t bump_out;
	uint8_t border;
	uint8_t outline;
	uint8_t radius;
	uint8_t margin;
	uint8_t padding;
} GUI_Shape;

typedef struct {
	GUI_Color* color_body_active;
	GUI_Color* color_body_passive;
	GUI_Color* color_title_active;
	GUI_Color* color_title_passive;
	GUI_Color* color_frame_active;
	GUI_Color* color_frame_passive;
	GUI_Shape* shape_body;
	GUI_Shape* shape_title;
	void* font_title;
	void* font_body;
	char* title;
} GUI_Window;

typedef struct {
	GUI_Color* color_active;
	GUI_Color* color_hover;
	GUI_Color* color_pressed;
	GUI_Color* color_disabled;
	GUI_Color* color_passive;
	GUI_Shape* shape_active;
	GUI_Shape* shape_hover;
	GUI_Shape* shape_pressed;
	GUI_Shape* shape_disabled;
	void* font_text;
	char* text;
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
	uint16_t id;
	char *label;
	uint64_t ivalue;
	void* svalue;
} GUI_Checkbox;

typedef struct {
	SDL_Rect rect;
	SDL_Surface* surf;
	GUI_Window style_window;
	GUI_Button style_button;
	void* font;
} GUI_Context;

typedef struct {
	size_t uid_cnt;
	size_t item_cnt;
	GUI_UID uid;
	GUI_UIDRecord uids[GUI_MAX_ITEMS];
	GUI_ItemRecord items[GUI_MAX_ITEMS];
} GUI_Dispatcher;
