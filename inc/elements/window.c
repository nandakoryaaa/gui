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
