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
				}
			},
			.child_cnt = 1,
			.subtree = &(GUI_ItemTree) {
				{
					ID_BTN_CLOSE, GUI_ITEM_BUTTON, GUI_STATUS_VSHA, { -34, 8, 24, 24 },
					&(GUI_Button) { .text = "x" }
				}
			}
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
					.child_cnt = 6,
					.subtree = &(GUI_ItemTree[]) {
						{
							{
								ID_PANEL1, GUI_ITEM_DISPLAYPANEL, GUI_STATUS_VSA, { 120, 60, 120, 32 },
								&(GUI_DisplayPanel) {
									.color_active = &(GUI_Color) { .bg = 0, .fg = 0xffffff, .light = 0xcccccc, .dark = 0x333333 },
									.color_passive = &(GUI_Color) { .bg = 0, .fg = 0xffffff, .light = 0xcccccc, .dark = 0x333333 },
									.shape = &(GUI_Shape) { .bump_out = 0, .bump_in = 4 },
									.label = "Counter",
									.ivalue = 1000
								}
							}
						}, {
							{
								ID_BTN1, GUI_ITEM_BUTTON, GUI_STATUS_VSHA, { 80, 60, 32, 32 },
								&(GUI_Button) { .text = "-" }
							}
						}, {
							{
								ID_BTN2, GUI_ITEM_BUTTON, GUI_STATUS_VSHA, { 248, 60, 32, 32 },
								&(GUI_Button) { .text = "+" }
							}
						}, {
							{
								ID_HSLIDER_R, GUI_ITEM_HSLIDER, GUI_STATUS_VSDA, { 108, 120, 120, 32 },
								&(GUI_Slider) { .min = 0, .max = 255, .value = 0x80 }
							}
						}, {
							{
								ID_HSLIDER_G, GUI_ITEM_HSLIDER, GUI_STATUS_VSDA, { 108, 160, 120, 32 },
								&(GUI_Slider) { .min = 0, .max = 255, .value = 0x80 }
							}
						}, {
							.item = {
								ID_HSLIDER_B, GUI_ITEM_HSLIDER, GUI_STATUS_VSDA, { 80, 200, 176, 32 },
								&(GUI_Slider) { .min = 0, .max = 255, .value = 0x80 }
							},
							.child_cnt = 2,
							.subtree = &(GUI_ItemTree[]) {
								{
									{
										ID_NONE, GUI_ITEM_BUTTON, GUI_STATUS_VSHA, { 2, 2, 28, 28 },
										&(GUI_Button) { .value = -10, .text = "<" }, ID_HSLIDER_B
									}
								}, {
									{
										ID_NONE, GUI_ITEM_BUTTON, GUI_STATUS_VSHA, { 146, 2, 28, 28 },
										&(GUI_Button) { .value = 10, .text = ">" }, ID_HSLIDER_B
									}
								}
							}[0]
						}
					}[0]
				}, {
					.item = {
						ID_NONE, GUI_ITEM_TAB, GUI_STATUS_VSA, { 0, 0, 0, 0 },
						&(GUI_GenericGroup) { "Tab 2", NULL }
					},
					.child_cnt = 1,
					.subtree = &(GUI_ItemTree) {
						.item = {
							999, GUI_ITEM_CONTENTPANE, GUI_STATUS_VSA, { 80, 80, 194, 140 },
							&(GUI_ContentPane) { { 0, 0, 0, 0 } }
						},
						.child_cnt = 2,
						.subtree = &(GUI_ItemTree[]) {
							{
								{
									ID_NONE, GUI_ITEM_BOUNDINGBOX, GUI_STATUS_VSA, { 0, 0, 240, 80 },
									NULL
								},
								3, &(GUI_ItemTree[]) {
									{
										{
											ID_NONE, GUI_ITEM_PLACEHOLDER, GUI_STATUS_VA, { 10, 10, 64, 64 },
											&(GUI_Placeholder) { .color = 0xff0000 }
										}
									}, {
											{
												ID_NONE, GUI_ITEM_PLACEHOLDER, GUI_STATUS_VA, { 90, 10, 64, 64 },
												&(GUI_Placeholder) { .color = 0x00ff00 }
											}
									}, {
										{
											ID_NONE, GUI_ITEM_PLACEHOLDER, GUI_STATUS_VA, { 170, 10, 64, 64 },
											&(GUI_Placeholder) { .color = 0x0000ff }
										}
									}
								}[0]
							}, {
								{
									ID_HSLIDERPANE, GUI_ITEM_HSLIDER, GUI_STATUS_VSDA, { 10, 100, 176, 32 },
									&(GUI_Slider) { .min = 0, .max = 255, .value = 0 }, 999
								},
								.child_cnt = 2,
								.subtree = &(GUI_ItemTree[]) {
									{
										{
											ID_NONE, GUI_ITEM_BUTTON, GUI_STATUS_VSHA, { 2, 2, 28, 28 },
											&(GUI_Button) { .value = -5, .text = "<" }, ID_HSLIDERPANE
										}
									}, {
										{
											ID_NONE, GUI_ITEM_BUTTON, GUI_STATUS_VSHA, { 146, 2, 28, 28 },
											&(GUI_Button) { .value = 5, .text = ">" }, ID_HSLIDERPANE
										}
									}
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
