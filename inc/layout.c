GUI_Color color_body = {
	.bg = 0x999999, .fg = 0x000000, .light = 0xe0e0e0, .dark = 0x404040,
	.border = 0xccbb40, .outline = 0
};
GUI_Color color_caption = {
	.bg = 0x004040, .fg = 0xffffff, .light = 0x009090, .dark = 0x000f0f,
	.border = 0x004040, .outline = 0
};
GUI_Color color_button_hover = {
	.bg = 0xb0b0b0, .fg = 0xffffff, .light = 0xe0e0e0, .dark = 0x404040,
	.border = 0xccbb40, .outline = 0
};
GUI_Color color_button_down = {
	.bg = 0x808080, .fg = 0xffffff, .light = 0xe0e0e0, .dark = 0x404040,
	.border = 0xccbb40, .outline = 0
};
GUI_Shape shape_body = {
	.bump_out = 4, .bump_in = 4, .border = 4, .outline = 0, .radius = 0, .padding = 0, .margin = 0
};
GUI_Shape shape_caption = {
	.bump_out = 0, .bump_in = 1, .border = 0, .outline = 0, .radius = 0, .padding = 8, .margin = 4
};
GUI_Shape shape_button = {
	.bump_out = 2, .bump_in = 0, .border = 0, .outline = 0, .radius = 0, .padding = 0, .margin = 0
};
GUI_Shape shape_button_down = {
	.bump_out = 0, .bump_in = 2, .border = 0, .outline = 0, .radius = 0, .padding = 0, .margin = 2
};

GUI_ItemTree layout = {
	.item = {
		ID_WIN1, GUI_ITEM_WINDOW, GUI_STATUS_VSDA, { 100, 100, 400, 400 },
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
				ID_CAPTION1, GUI_ITEM_CAPTION, GUI_STATUS_VSDA, { 0, 0, 0, 40 },
				&(GUI_Caption) { NULL, NULL, NULL, NULL, "Hello GUI" }
			},
			.child_cnt = 1,
			.subtree = &(GUI_ItemTree) {
				.item = {
					ID_BTN_CLOSE, GUI_ITEM_BUTTON, GUI_STATUS_VSHA, { -34, 8, 24, 24 },
					&(GUI_Button) {
						.text = "x"
					}
				},
				.child_cnt = 0, .subtree = NULL
			}
		}, {
			.item = {
				ID_TABGROUP, GUI_ITEM_TABGROUP, GUI_STATUS_VSA, { 0, 40, 0, 0 },
				&(GUI_TabGroup) { 0, 32 }
			},
			.child_cnt = 3,
			.subtree = &(GUI_ItemTree[]) {
				{
					.item = {
						ID_TAB1, GUI_ITEM_TAB, GUI_STATUS_VSA, { 0, 0, 0, 0 },
						&(GUI_GenericGroup) { "Tab 1", NULL }
					},
					.child_cnt = 4,
					.subtree = &(GUI_ItemTree[]) {
						{
							.item = {
								ID_PANEL1, GUI_ITEM_DISPLAYPANEL, GUI_STATUS_VSA, { 120, 80, 120, 32 },
								&(GUI_DisplayPanel) {
									.color_active = &(GUI_Color) { .bg = 0, .fg = 0xffffff, .light = 0xcccccc, .dark = 0x333333 },
									.color_passive = &(GUI_Color) { .bg = 0, .fg = 0xffffff, .light = 0xcccccc, .dark = 0x333333 },
									.shape = &(GUI_Shape) { .bump_out = 0, .bump_in = 4 },
									.label = "Counter",
									.svalue = NULL,
									.ivalue = 1000
								}
							},
							.child_cnt = 0
						}, {
							.item = {
								ID_BTN1, GUI_ITEM_BUTTON, GUI_STATUS_VSHA, { 80, 80, 32, 32 },
								&(GUI_Button) {
									.text = "-"
								}
							},
							.child_cnt = 0
						}, {
							.item = {
								ID_BTN2, GUI_ITEM_BUTTON, GUI_STATUS_VSHA, { 248, 80, 32, 32 },
								&(GUI_Button) {
									.text = "+"
								}
							},
							.child_cnt = 0
						}, {
							.item = {
								ID_TEST, GUI_ITEM_CHECKBOX, GUI_STATUS_VSA, { 80, 140, 32, 32 },
								&(GUI_Checkbox) {
									.label = "test checkbox"
								}
							}
						}
					}[0]
				}, {
					.item = {
						ID_TAB2, GUI_ITEM_TAB, GUI_STATUS_NONE, { 0, 0, 0, 0 },
						&(GUI_GenericGroup) { "Tab 2", NULL }
					},
					.child_cnt = 1,
					.subtree = &(GUI_ItemTree) {
						.item = {
							ID_T2G1, GUI_ITEM_GROUP, GUI_STATUS_VSA, { 80, 80, 200, 200 },
							&(GUI_GenericGroup) { "Group 2", NULL }
						},
						.child_cnt = 3,
						.subtree = &(GUI_ItemTree[]) {
							{
								{
									ID_T2G1_CB1, GUI_ITEM_CHECKBOX, GUI_STATUS_VSA, { 10, 10, 16, 16 },
									&(GUI_Checkbox) { "checkbox t2-g2-1" }
								},
								0, NULL
							}, {
								{
									ID_T2G1_CB2, GUI_ITEM_CHECKBOX, GUI_STATUS_VSA, { 10, 30, 16, 16 },
									&(GUI_Checkbox) { "checkbox t2-g2-2" }
								},
								0, NULL
							}, {
								{
									ID_T2G1_CB3, GUI_ITEM_CHECKBOX, GUI_STATUS_VSA, { 10, 50, 16, 16 },
									&(GUI_Checkbox) { "checkbox t2-g2-3" }
								},
								0, NULL
							},
						}[0]
					}
				}, {
					.item = {
						ID_TAB3, GUI_ITEM_TAB, GUI_STATUS_NONE, { 0, 0, 0, 0 },
						&(GUI_GenericGroup) { "Tab 3", NULL }
					},
					.child_cnt = 1,
					.subtree = &(GUI_ItemTree) {
						.item = {
							ID_T3G1, GUI_ITEM_GROUP, GUI_STATUS_VSA, { 80, 80, 200, 200 },
							&(GUI_GenericGroup) { "Group 3", NULL }
						},
						.child_cnt = 3,
						.subtree = &(GUI_ItemTree[]) {
							{
								{
									ID_T3G1_CB1, GUI_ITEM_CHECKBOX, GUI_STATUS_VSA, { 10, 10, 16, 16 },
									&(GUI_Checkbox) { "checkbox t3-g3-1" }
								},
								0, NULL
							}, {
								{
									ID_T3G1_CB2, GUI_ITEM_CHECKBOX, GUI_STATUS_VSA, { 10, 30, 16, 16 },
									&(GUI_Checkbox) { "checkbox t3-g3-2" }
								},
								0, NULL
							}, {
								{
									ID_T3G1_CB3, GUI_ITEM_CHECKBOX, GUI_STATUS_VSA, { 10, 50, 16, 16 },
									&(GUI_Checkbox) { "checkbox t3-g3-3" }
								},
								0, NULL
							}
						}[0]
					}
				}
			}[0]
		}
	}[0]
};
