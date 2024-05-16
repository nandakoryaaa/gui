typedef struct {
	int32_t value;
	int32_t min;
	int32_t max;
	uint16_t margin;
} GUI_Slider;

typedef enum {
	GUI_SLIDER_NONE, GUI_SLIDER_BTN1, GUI_SLIDER_BTN2, GUI_SLIDER_THUMB, GUI_SLIDER_BAND
} GUI_SliderTargetType;

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

GUI_Result GUI_dispatcher_process_target_hslider(GUI_Dispatcher* dsp, size_t index, GUI_Event evt)
{
	GUI_ItemRecord* irec = &dsp->items[index];
	GUI_Slider* slider = irec->item.element;
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

	if (evt.type == GUI_EVENT_DOWN) {
		// is thumb hit?
		if (x >= thumb_pos && x < thumb_pos + rect.h) {
			// do nothing
			return GUI_NONE;
		}
		// try positioning thumb at x as its center
		thumb_pos = x - (rect.h >> 1);
		
	} else if (dsp->state == GUI_STATUS_DRAG) {
		int16_t dx = evt.x - x;
		if (dx != 0) {
			// last cursor offset within thumb
			local_x = x - thumb_pos;
			// new thumb pos at x with same cursor offset
			thumb_pos = evt.x - local_x;
		}
	} else {
		return GUI_NONE;
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
		return GUI_OK;
	}

	return GUI_NONE;
}
