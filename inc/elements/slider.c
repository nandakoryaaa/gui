typedef struct {
	int32_t value;
	int32_t min;
	int32_t max;
	uint16_t margin;
	GUI_CommandType command_type;
} GUI_Slider;

GUI_ItemTree GUI_slider_create(
	GUI_Dispatcher* dsp, GUI_ID id, GUI_Rect rect,
	int32_t min, int32_t max, int32_t value, GUI_CommandType command_type, int use_buttons
) {
	GUI_ItemTree* subtree;
	if (use_buttons) {
		uint16_t h = rect.h - 4;
		GUI_Button* btn1 = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_Button));
		*btn1 = (GUI_Button) { .text = "<", .command_type = GUI_CMD_INCVAL, .value = -10 };
		GUI_Button* btn2 = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_Button));
		*btn2 = (GUI_Button) { .text = ">", .command_type = GUI_CMD_INCVAL, .value = 10 };
		subtree = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_ItemTree) * 2);
		subtree[0] = (GUI_ItemTree) {
			.item = {
				.type = GUI_ITEM_BUTTON, .status = GUI_STATUS_VSHA,
				.rect = { 2, 2, h, h },
				.element = btn1, .return_state = GUI_EVENT_CMD
			}
		};
		subtree[1] = (GUI_ItemTree) {
			.item = {
				.type = GUI_ITEM_BUTTON, .status = GUI_STATUS_VSHA,
				.rect = { -rect.h + 2, 2, h, h },
				.element = btn2, .return_state = GUI_EVENT_CMD
			}
		};
	}

	GUI_Slider* slider = GUI_dispatcher_allocate_element(dsp, sizeof(GUI_Slider));
	*slider = (GUI_Slider) { .min = min, .max = max, .value = value, .command_type = command_type }; 

	return (GUI_ItemTree) {
		.item = {
			.id = id, .type = GUI_ITEM_HSLIDER, .status = GUI_STATUS_VSDA,
			.rect = rect,
			.element = slider
		},
		.child_cnt = use_buttons ? 2 : 0,
		.subtree = use_buttons ? subtree : NULL
	};
}

GUI_Rect GUI_slider_adjust_rect(GUI_Slider* slider, GUI_Rect rect)
{
	uint16_t margin = slider->margin;
	rect.x += margin >> 1;
	rect.h -= margin;
	rect.w -= margin + rect.h;

	return rect;
}

GUI_Rect GUI_slider_fix_rect(GUI_Context* ctx, GUI_Rect item_rect, GUI_Slider* slider)
{
	GUI_Shape* shape_bg = ctx->shape_button_down;
	slider->margin = (shape_bg->bump_out + shape_bg->border + shape_bg->bump_in) << 1;

	return item_rect;
}

GUI_ComboEvent GUI_dispatcher_process_target_hslider(
	GUI_Dispatcher* dsp, size_t index, GUI_ComboEvent cevt
) {
	GUI_ItemRecord* irec = &dsp->items[index];
	GUI_Slider* slider = irec->item.element;

	if (cevt.type == GUI_EVENT_CMD) {
		if (cevt.cmd.type == GUI_CMD_INCVAL) {
			slider->value += cevt.cmd.value;
			if (slider->value > slider->max) {
				slider->value = slider->max;
			} else if (slider->value < slider->min) {
				slider->value = slider->min;
			}
			cevt.result = GUI_OK;
			cevt.type = GUI_EVENT_NONE;
		}
	} else {
		GUI_Rect rect = GUI_slider_adjust_rect(slider, irec->rect);
		// does it have buttons?
		if (irec->child_cnt) {
			rect.x += rect.h;
			rect.w -= (rect.h << 1);
		}
		uint32_t range = slider->max - slider->min;
		// thumb position according to value
		uint16_t thumb_pos = rect.x + (slider->value - slider->min) * rect.w / range;
		uint16_t old_thumb_pos = thumb_pos;
		uint16_t x = dsp->origin_x;
		uint16_t local_x = 0;

		if (cevt.type == GUI_EVENT_DOWN) {
			// is thumb hit?
			if (x >= thumb_pos && x < thumb_pos + rect.h) {
				// do nothing
				return cevt;
			}
			// try positioning thumb at x as its center
			thumb_pos = x - (rect.h >> 1);

		} else if (cevt.type == GUI_EVENT_DRAG) {
			int16_t dx = cevt.evt.x - x;
			if (dx != 0) {
				// last cursor offset within thumb
				local_x = x - thumb_pos;
				// new thumb pos at x with same cursor offset
				thumb_pos = cevt.evt.x - local_x;
			}
		}

		if (thumb_pos < rect.x) {
			thumb_pos = rect.x;
		} else if (thumb_pos >= rect.x + rect.w) {
			thumb_pos = rect.x + rect.w;
		}

		if (thumb_pos != old_thumb_pos) {
			slider->value = slider->min + (thumb_pos - rect.x) * range / rect.w;
			// correct thumb pos against new value because they don't always match
			thumb_pos = rect.x + (slider->value - slider->min) * rect.w / range;
			dsp->origin_x = thumb_pos + local_x;
			cevt.result = GUI_OK;
		}
	}

	if (cevt.result == GUI_OK && slider->command_type) {
		cevt.type = GUI_EVENT_CMD;
		cevt.cmd.type = slider->command_type;
		cevt.cmd.value = slider->value;
	}

	return cevt;
}
