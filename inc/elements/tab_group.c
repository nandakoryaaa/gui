typedef struct {
	uint16_t current;
	uint16_t head_h;
} GUI_TabGroup;

GUI_Rect GUI_tabgroup_fix_rect(GUI_Context* ctx, GUI_Rect rect, GUI_TabGroup* tg)
{
	rect.y += tg->head_h;
	rect.h -= tg->head_h;
	return rect;
}
