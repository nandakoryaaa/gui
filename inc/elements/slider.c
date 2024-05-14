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
