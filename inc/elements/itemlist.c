typedef struct {
	uint16_t cnt;
	uint16_t pos;
	char** items;
	GUI_CommandType command_type;
} GUI_ItemList;

GUI_ComboEvent GUI_dispatcher_process_target_itemlist(
	GUI_Dispatcher* dsp, size_t index, GUI_ComboEvent cevt
) {
	return cevt;
}

struct GUI_ComboBoxTree {
	GUI_DisplayPanel panel;
	GUI_Button btn;
	GUI_Color color_active;
	GUI_Color color_passive;
	GUI_Shape shape;
	GUI_ItemTree subtree[2];
};

// not finished
GUI_ItemTree GUI_itemlist_create(
	GUI_ID id, GUI_Rect rect, GUI_ComboBox* cb, struct GUI_ComboBoxTree* tree
) {
	uint16_t h = rect.h - 4;
	tree->btn = (GUI_Button) { .text = "v", .command_type = GUI_CMD_COMBO };
	tree->color_active = (GUI_Color) { .bg = 0xffffff, .fg = 0, .light = 0xcccccc, .dark = 0x333333 };
	tree->color_passive = (GUI_Color) { .bg = 0, .fg = 0xffffff, .light = 0xcccccc, .dark = 0x333333 };
	tree->shape = (GUI_Shape) { .bump_out = 0, .bump_in = 2 };
	tree->panel = (GUI_DisplayPanel) {
		.color_active = &tree->color_active,
		.color_passive = &tree->color_passive,
		.shape = &tree->shape,
		.label = NULL,
		.svalue = cb->items[cb->pos]
	};
	tree->subtree[0] = (GUI_ItemTree) {
		.item = {
			.type = GUI_ITEM_BUTTON, .status = GUI_STATUS_VSHA,
			.rect = { -rect.h + 2, 2, h, h },
			.element = &tree->btn, .return_state = GUI_EVENT_CMD
		}
	};
	tree->subtree[1] = (GUI_ItemTree) {
		.item = {
			.type = GUI_ITEM_DISPLAYPANEL, .status = GUI_STATUS_VSA,
			.rect = { 0, 0, rect.w - h - 2, h },
			.element = &tree->panel
		}
	};
	return (GUI_ItemTree) {
		.item = {
			.id = id, .type = GUI_ITEM_COMBOBOX, .status = GUI_STATUS_VSA,
			.rect = rect,
			.element = cb
		},
		.child_cnt = 2,
		.subtree = tree->subtree
	};
}
