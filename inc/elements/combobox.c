GUI_ComboEvent GUI_dispatcher_process_target_combobox(
	GUI_Dispatcher* dsp, size_t index, GUI_ComboEvent cevt
) {
	if (cevt.type == GUI_EVENT_CMD) {
		GUI_ItemRecord* irec = &dsp->items[index];
		if (cevt.cmd.type == GUI_CMD_OPEN) {
			if (!(irec->item.status & GUI_STATUS_SELECTED)) {
				GUI_ItemTree itemlist_tree = GUI_itemlist_create(dsp, ID_NONE, index);
				dsp->volatile_index = dsp->item_cnt;
				dsp->volatile_evt = GUI_EVENT_DOWN;
				GUI_dispatcher_push_tree(dsp, &itemlist_tree);
				GUI_dispatcher_set_tree_status(dsp, index, GUI_STATUS_VOLATILE);
				GUI_dispatcher_set_tree_status(dsp, dsp->volatile_index, GUI_STATUS_VOLATILE);
				irec->item.status |= GUI_STATUS_SELECTED;
			} else {
				GUI_dispatcher_remove_volatile(dsp);
			}
		} else if (cevt.cmd.type == GUI_CMD_CLOSE) {
			irec->item.status &= ~GUI_STATUS_SELECTED;
			GUI_dispatcher_clear_tree_status(dsp, index, GUI_STATUS_VOLATILE);
			for (uint16_t i = 0; i < irec->child_cnt; i++) {
				index++;
				if (dsp->items[index].item.type == GUI_ITEM_DISPLAYPANEL) {
					GUI_DisplayPanel* dp = dsp->items[index].item.element;
					GUI_ComboBox* cb = irec->item.element;
					dp->svalue = cb->items[cb->pos];
					break;
				}
			}
		}

		cevt.result = GUI_OK;
	}
	cevt.type = GUI_EVENT_NONE;
	return cevt;
}

GUI_ItemTree GUI_combobox_create(
	GUI_Dispatcher* dsp, GUI_ID id, GUI_Rect rect, uint16_t cnt, uint16_t pos, char* items[]
) {
	uint16_t h = rect.h - 4;
	GUI_Button* btn = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_Button));
	*btn = (GUI_Button) { .text = "v", .command_type = GUI_CMD_OPEN };

	GUI_DisplayPanel* panel = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_DisplayPanel));
	*panel = (GUI_DisplayPanel) {
		.color_active = dsp->ctx->color_panel_active,
		.color_passive = dsp->ctx->color_panel_passive,
		.shape = dsp->ctx->shape_panel,
		.svalue = items[pos]
	};

	GUI_ItemTree* subtree = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_ItemTree) * 2);
	subtree[0] = (GUI_ItemTree) {
		.item = {
			.type = GUI_ITEM_DISPLAYPANEL, .status = GUI_STATUS_VSA,
			.rect = { 0, 0, rect.w - h - 2, h },
			.element = panel
		}
	};
	subtree[1] = (GUI_ItemTree) {
		.item = {
			.type = GUI_ITEM_BUTTON, .status = GUI_STATUS_VSHA,
			.rect = { -rect.h + 2, 2, h, h },
			.element = btn,
			.return_state = GUI_EVENT_CMD
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
