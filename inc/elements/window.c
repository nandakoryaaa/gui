typedef struct {
	GUI_Color* color_active;
	GUI_Color* color_passive;
	GUI_Shape* shape;
	void* font;
	char* title;
} GUI_Window;

GUI_Rect GUI_window_fix_rect(GUI_Context* ctx, GUI_Rect rect, GUI_Window* win)
{
	GUI_Shape* shape = win->shape == NULL ? ctx->shape_body : win->shape;
	uint16_t bump = shape->bump_out + shape->bump_in + shape->border;
	rect.x += bump;
	rect.y += bump;
	rect.w -= bump << 1;
	rect.h -= bump << 1;
	return rect;
}

GUI_ComboEvent GUI_dispatcher_process_target_window(
	GUI_Dispatcher* dsp, size_t index, GUI_ComboEvent cevt
) {
	if (cevt.type == GUI_EVENT_DRAG) {
		int16_t dx = cevt.evt.x - dsp->origin_x;
		int16_t dy = cevt.evt.y - dsp->origin_y;
		uint16_t cnt = dsp->items[index].subtree_cnt + 1;
		while (cnt--) {
			dsp->items[index].rect.x += dx;
			dsp->items[index].rect.y += dy;
			index++;
		}
		dsp->origin_x = cevt.evt.x;
		dsp->origin_y = cevt.evt.y;
		cevt.type = GUI_EVENT_NONE;
		cevt.result = GUI_OK;
	}

	return cevt;
}
