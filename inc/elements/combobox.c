typedef struct {
	uint16_t cnt;
	uint16_t pos;
	char** items;
	GUI_CommandType command_type;
} GUI_ComboBox;

GUI_ComboEvent GUI_dispatcher_process_target_combobox(
	GUI_Dispatcher* dsp, size_t index, GUI_ComboEvent cevt
) {
	return cevt;
}

GUI_ItemTree GUI_combobox_create(
	GUI_Dispatcher* dsp, GUI_ID id, GUI_Rect rect, uint16_t cnt, uint16_t pos, char* items[]
) {
	uint16_t h = rect.h - 4;
	GUI_Button* btn = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_Button));
	*btn = (GUI_Button) { .text = "v", .command_type = GUI_CMD_COMBO };

	GUI_DisplayPanel* panel = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_DisplayPanel));
	*panel = (GUI_DisplayPanel) {
		.color_active = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_Color)),
		.color_passive = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_Color)),
		.shape = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_Shape)),
		.label = NULL,
		.svalue = items[pos]
	};

	*panel->color_active = (GUI_Color) { .bg = 0xffffff, .fg = 0, .light = 0xcccccc, .dark = 0x333333 };
	*panel->color_passive = (GUI_Color) { .bg = 0, .fg = 0xffffff, .light = 0xcccccc, .dark = 0x333333 };
	*panel->shape = (GUI_Shape) { .bump_out = 0, .bump_in = 2 };

	GUI_ItemTree* subtree = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_ItemTree) * 2);
	subtree[0] = (GUI_ItemTree) {
		.item = {
			.type = GUI_ITEM_BUTTON, .status = GUI_STATUS_VSHA,
			.rect = { -rect.h + 2, 2, h, h },
			.element = btn,
			.return_state = GUI_EVENT_CMD
		}
	};

	subtree[1] = (GUI_ItemTree) {
		.item = {
			.type = GUI_ITEM_DISPLAYPANEL, .status = GUI_STATUS_VSA,
			.rect = { 0, 0, rect.w - h - 2, h },
			.element = panel
		}
	};

	GUI_ComboBox* cb = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_ComboBox));
	*cb = (GUI_ComboBox) { .cnt = cnt, .pos = pos, .items = items };

	return (GUI_ItemTree) {
		.item = {
			.id = id, .type = GUI_ITEM_COMBOBOX, .status = GUI_STATUS_VSA,
			.rect = rect,
			.element = cb
		},
		.child_cnt = 2,
		.subtree = subtree
	};
}
