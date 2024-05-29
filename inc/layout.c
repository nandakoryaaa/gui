GUI_ItemTree layout = {
	.item = {
		ID_WIN1, GUI_ITEM_WINDOW, GUI_STATUS_VSDA, { 280, 160, 400, 400 },
		&(GUI_Window) {
			.color_active = NULL,
			.color_passive = NULL,
			.shape = NULL,
			.font = NULL,
			.title = "Hello GUI"
		}
	},
	.child_cnt = 2,
	.subtree = &(GUI_ItemTree[]) {
		{
			.item = {
				ID_CAPTION, GUI_ITEM_CAPTION, GUI_STATUS_VSDA, { 0, 0, 0, 40 },
				&(GUI_Caption) {
					&(GUI_Color) { .bg = 0x808080, .fg = 0xffffff },
					NULL, NULL, NULL, "Hello GUI"
				}, .return_state = GUI_EVENT_DRAG
			},
			.child_cnt = 1,
			.subtree = & (GUI_ItemTree[]) {
				GUI_button_create(&dispatcher, ID_NONE, (GUI_Rect) { -34, 8, 24, 24 }, "x", GUI_CMD_CLOSE, 0)
			}[0]
		}, {
			.item = {
				ID_NONE, GUI_ITEM_TABGROUP, GUI_STATUS_VSA, { 0, 40, 0, 0 },
				&(GUI_TabGroup) { 1, 32 }
			},
			.child_cnt = 3,
			.subtree = &(GUI_ItemTree[]) {
				{
					.item = {
						ID_NONE, GUI_ITEM_TAB, GUI_STATUS_NONE, { 0, 0, 0, 0 },
						&(GUI_GenericGroup) { "Tab 1", NULL }
					},
					.child_cnt = 7,
					.subtree = &(GUI_ItemTree[]) {
						GUI_combobox_create(
							&dispatcher, ID_NONE, (GUI_Rect) { 120, 30, 200, 32 },
							3, 0, (char*[]){ "aaa", "bbb", "ccc" }
						),
						{
							.item = {
								ID_PANEL1, GUI_ITEM_DISPLAYPANEL, GUI_STATUS_VSA, { 120, 80, 120, 32 },
								&(GUI_DisplayPanel) {
									.color_active = &(GUI_Color) { .bg = 0, .fg = 0xffffff, .light = 0xcccccc, .dark = 0x333333 },
									.color_passive = &(GUI_Color) { .bg = 0, .fg = 0xffffff, .light = 0xcccccc, .dark = 0x333333 },
									.shape = &(GUI_Shape) { .bump_out = 0, .bump_in = 4 },
									.label = "Counter",
									.ivalue = 1000
								}
							}
						},
						GUI_button_create(
							&dispatcher, ID_NONE, (GUI_Rect) { 80, 80, 32, 32 }, "-", GUI_CMD_DECVAL, 0
						),
						GUI_button_create(
							&dispatcher, ID_NONE, (GUI_Rect) { 248, 80, 32, 32 }, "+", GUI_CMD_INCVAL, 0
						),
						GUI_slider_create(
							&dispatcher, ID_HSLIDER_R, (GUI_Rect) { 80, 140, 176, 32 },
							0, 255, 127, GUI_CMD_SETCOLOR, 1
						),
						GUI_slider_create(
							&dispatcher, ID_HSLIDER_G, (GUI_Rect) { 80, 180, 176, 32 },
							0, 255, 127, GUI_CMD_SETCOLOR, 1
						),
						GUI_slider_create(
							&dispatcher, ID_HSLIDER_B, (GUI_Rect) { 80, 220, 176, 32 },
							0, 255, 127, GUI_CMD_SETCOLOR, 1
						)
					}[0]
				}, {
					.item = {
						ID_NONE, GUI_ITEM_TAB, GUI_STATUS_VSA, { 0, 0, 0, 0 },
						&(GUI_GenericGroup) { "Tab 2" }
					},
					.child_cnt = 1,
					.subtree = &(GUI_ItemTree) {
						.item = {
							ID_NONE, GUI_ITEM_CONTENTPANE, GUI_STATUS_VSDA, { 40, 40, 292, 200 },
							&(GUI_ContentPane) { .offset_rect = { 0, 0, 0, 1 } }
						},
						.child_cnt = 2,
						.subtree = &(GUI_ItemTree[]) {
							{
								{
									ID_NONE, GUI_ITEM_CONTENTPANE, GUI_STATUS_VSDA, { 10, 10, 272, 80 },
									&(GUI_ContentPane) { .offset_rect = { 0, 0, 1, 0 } }, .return_state = GUI_EVENT_DRAG
								},
								.child_cnt = 3,
								.subtree = &(GUI_ItemTree[]) {
									GUI_placeholder_create(
										&dispatcher, (GUI_Rect) { 10, 10, 64, 64 }, 0xff0000, GUI_EVENT_DRAG
									),
									GUI_placeholder_create(
										&dispatcher, (GUI_Rect) { 90, 10, 64, 64 }, 0x00ff00, GUI_EVENT_DRAG
									),
									GUI_placeholder_create(
										&dispatcher, (GUI_Rect) { 170, 10, 64, 64 }, 0x0000ff, GUI_EVENT_DRAG
									),
								}[0]
							}, {
								{
									ID_NONE, GUI_ITEM_CONTENTPANE, GUI_STATUS_VSDA, { 10, 100, 272, 80 },
									&(GUI_ContentPane) { .offset_rect = { 0, 0, 1, 0 } }, .return_state = GUI_EVENT_DRAG
								},
								.child_cnt = 3,
								.subtree = &(GUI_ItemTree[]) {
									GUI_placeholder_create(
										&dispatcher, (GUI_Rect) { 10, 10, 64, 64 }, 0xff8000 , GUI_EVENT_DRAG
									),
									GUI_placeholder_create(
										&dispatcher, (GUI_Rect) { 90, 10, 64, 64 }, 0x00ff80, GUI_EVENT_DRAG
									),
									GUI_placeholder_create(
										&dispatcher, (GUI_Rect) { 170, 10, 64, 64 }, 0x8000ff, GUI_EVENT_DRAG
									)
								}[0]
							}
						}[0]
					}
				}, {
					.item = {
						ID_NONE, GUI_ITEM_TAB, GUI_STATUS_NONE, { 0, 0, 0, 0 },
						&(GUI_GenericGroup) { "Tab 3", NULL }
					},
					.child_cnt = 1,
					.subtree = &(GUI_ItemTree) {
						.item = {
							ID_NONE, GUI_ITEM_GROUP, GUI_STATUS_VSA, { 80, 80, 200, 200 },
							&(GUI_GenericGroup) { "Group 3", NULL }
						},
						.child_cnt = 4,
						.subtree = &(GUI_ItemTree[]) {
							GUI_checkbox_create(&dispatcher, ID_CB1, (GUI_Rect) { 10, 10, 16, 16 }, "checkbox 1"),
							GUI_checkbox_create(&dispatcher, ID_CB2, (GUI_Rect) { 10, 30, 16, 16 }, "checkbox 2"),
							GUI_checkbox_create(&dispatcher, ID_CB3, (GUI_Rect) { 10, 50, 16, 16 }, "checkbox 3")
						}[0]
					}
				}
			}[0]
		}
	}[0]
};
