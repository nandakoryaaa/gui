GUI_ItemTree layout = {
	.item = {
		ID_WIN1, GUI_ITEM_WINDOW, GUI_STATUS_VSA, { 100, 100, 400, 400 },
		&(GUI_Window) {
			&color_main, &color_main, &color_title_main, &color_title_main,
			NULL, NULL, NULL, NULL,
			&shape_main, &shape_main,
			"Hello GUI"
		}
	},
	.child_cnt = 1,
	.subtree = &(GUI_ItemTree) {
		.item = {
			ID_TABGROUP, GUI_ITEM_TABGROUP, GUI_STATUS_VSA, { 0, 32, 0, 0 },
			&(GUI_TabGroup) { 1, 32 }
		},
		.child_cnt = 3,
		.subtree = &(GUI_ItemTree[]) {
			{
				.item = {
					ID_TAB1, GUI_ITEM_TAB, GUI_STATUS_NONE, { 0, 32, 0, 0 },
					&(GUI_GenericGroup) { "Tab 1", NULL }
				},
				.child_cnt = 1,
				.subtree = &(GUI_ItemTree) {
					.item = {
						ID_T1G1, GUI_ITEM_GROUP, GUI_STATUS_VSA, { 80, 80, 200, 200 },
						&(GUI_GenericGroup) { "Group 1", NULL }
					},
					.child_cnt = 3,
					.subtree = &(GUI_ItemTree[]) {
						{
							{
								ID_T1G1_CB1, GUI_ITEM_CHECKBOX, GUI_STATUS_VSA, { 10, 10, 16, 16 },
								&(GUI_Checkbox) { 1, "checkbox t1-g1-1", 1, NULL }
							},
							0, NULL
						},
						{
							{
								ID_T1G1_CB2, GUI_ITEM_CHECKBOX, GUI_STATUS_VSA, { 10, 30, 16, 16 },
								&(GUI_Checkbox) { 2, "checkbox t1-g1-2", 2, NULL }
							},
							0, NULL
						},
						{
							{
								ID_T1G1_CB3, GUI_ITEM_CHECKBOX, GUI_STATUS_VSA, { 10, 50, 16, 16 },
								&(GUI_Checkbox) { 3, "checkbox t1-g1-3", 3, NULL }
							},
							0, NULL
						},
					}[0]
				}
			},
			{
				.item = {
					ID_TAB2, GUI_ITEM_TAB, GUI_STATUS_VSA, { 0, 32, 0, 0 },
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
								&(GUI_Checkbox) { 1, "checkbox t2-g2-1", 1, NULL }
							},
							0, NULL
						},
						{
							{
								ID_T2G1_CB2, GUI_ITEM_CHECKBOX, GUI_STATUS_VSA, { 10, 30, 16, 16 },
								&(GUI_Checkbox) { 2, "checkbox t2-g2-2", 2, NULL }
							},
							0, NULL
						},
						{
							{
								ID_T2G1_CB3, GUI_ITEM_CHECKBOX, GUI_STATUS_VSA, { 10, 50, 16, 16 },
								&(GUI_Checkbox) { 3, "checkbox t2-g2-3", 3, NULL }
							},
							0, NULL
						},
					}[0]
				}
			},
			{
				.item = {
					ID_TAB3, GUI_ITEM_TAB, GUI_STATUS_NONE, { 0, 32, 0, 0 },
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
								&(GUI_Checkbox) { 1, "checkbox t3-g3-1", 1, NULL }
							},
							0, NULL
						},
						{
							{
								ID_T3G1_CB2, GUI_ITEM_CHECKBOX, GUI_STATUS_VSA, { 10, 30, 16, 16 },
								&(GUI_Checkbox) { 2, "checkbox t3-g3-2", 2, NULL }
							},
							0, NULL
						},
						{
							{
								ID_T3G1_CB3, GUI_ITEM_CHECKBOX, GUI_STATUS_VSA, { 10, 50, 16, 16 },
								&(GUI_Checkbox) { 3, "checkbox t3-g3-3", 3, NULL }
							},
							0, NULL
						},
					}[0]
				}
			}
		}[0]
	}
};
