
typedef uint64_t GUI_UID;
typedef SDL_Rect GUI_Rect;

typedef enum {
	GUI_ERROR, GUI_NONE, GUI_OK
} GUI_Result;

typedef enum {
	GUI_EVENT_NONE, GUI_EVENT_MOVE, GUI_EVENT_DOWN, GUI_EVENT_UP
} GUI_EventType;

//typedef enum {
//	GUI_COMMAND_NONE, GUI_COMMAND_CLOSE, GUI_COMMAND_ADD
//} GUI_Command;

typedef struct {
	GUI_EventType type;
	uint16_t x;
	uint16_t y;
} GUI_Event;

typedef enum {
	GUI_STATUS_NONE = 0, GUI_STATUS_DISABLED = 1,
	GUI_STATUS_VISIBLE = 2, GUI_STATUS_ACTIVE = 4,
	GUI_STATUS_SELECTABLE = 8, GUI_STATUS_HOVERABLE = 16,
	GUI_STATUS_DRAGGABLE = 32,
	GUI_STATUS_HOVER = 64, GUI_STATUS_DOWN = 128,
	GUI_STATUS_DRAG = 256, GUI_STATUS_SELECTED = 512,
	GUI_STATUS_VA = 6, GUI_STATUS_VSA = 14, GUI_STATUS_VSHA = 30,
	GUI_STATUS_VSDA = 46, GUI_STATUS_VSDHA = 62
} GUI_ItemStatus;

typedef enum {
	GUI_ITEM_NONE, GUI_ITEM_WINDOW, GUI_ITEM_CAPTION, GUI_ITEM_BUTTON,
	GUI_ITEM_GROUP, GUI_ITEM_TABGROUP, GUI_ITEM_TAB, GUI_ITEM_CHECKBOX,
	GUI_ITEM_DISPLAYPANEL, GUI_ITEM_HSLIDER, GUI_ITEM_CONTENTPANE,
	GUI_ITEM_PLACEHOLDER, GUI_ITEM_BOUNDINGBOX
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

typedef struct GUI_Item {
	uint32_t id;
	GUI_ItemType type;
	GUI_ItemStatus status;
	GUI_Rect rect;
	void* element;
	GUI_ID parent_id;
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
	size_t parent_offset;
	size_t child_cnt;
	size_t subtree_cnt;
} GUI_ItemRecord;

typedef struct GUI_ItemTree GUI_ItemTree;
struct GUI_ItemTree {
	GUI_Item item;
	uint16_t child_cnt;
	GUI_ItemTree* subtree;
};

typedef struct {
	SDL_Rect rect;
	SDL_Surface* surf;
	GUI_Color* color_body_active;
	GUI_Color* color_body_passive;
	GUI_Color* color_caption_active;
	GUI_Color* color_caption_passive;
	GUI_Color* color_button_active;
	GUI_Color* color_button_passive;
	GUI_Color* color_button_hover;
	GUI_Color* color_button_down;
	GUI_Shape* shape_body;
	GUI_Shape* shape_caption;
	GUI_Shape* shape_button;
	GUI_Shape* shape_button_down;
	void* font_body;
	void* font_caption;
	void* font_button;
	GUI_Color* current_color;
} GUI_Context;

typedef struct {
	size_t uid_cnt;
	uint16_t item_cnt;
	GUI_UID uid;
	GUI_UIDRecord uids[GUI_MAX_ITEMS];
	GUI_ItemRecord items[GUI_MAX_ITEMS];
	GUI_ItemStatus state;
	size_t last_uid_index;
	size_t last_index;
	size_t last_target;
	uint16_t origin_x;
	uint16_t origin_y;
} GUI_Dispatcher;
