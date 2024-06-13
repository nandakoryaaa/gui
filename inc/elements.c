void GUI_dispatcher_list_items(GUI_Dispatcher* dsp);
void* GUI_dispatcher_allocate_element(GUI_Dispatcher* dsp, uint16_t size);
GUI_UID GUI_dispatcher_push_tree(GUI_Dispatcher* dsp, GUI_ItemTree* it);
void GUI_dispatcher_remove_volatile(GUI_Dispatcher* dsp);
GUI_ComboEvent GUI_dispatcher_process_target_combobox(GUI_Dispatcher* dsp, size_t index, GUI_ComboEvent cevt);
void GUI_dispatcher_set_tree_status(GUI_Dispatcher* dsp, uint16_t index, GUI_ItemStatus status);
void GUI_dispatcher_clear_tree_status(GUI_Dispatcher* dsp, uint16_t index, GUI_ItemStatus status);

#include "elements/window.c"
#include "elements/caption.c"
#include "elements/button.c"
#include "elements/tab_group.c"
#include "elements/generic_group.c"
#include "elements/checkbox.c"
#include "elements/display_panel.c"
#include "elements/slider.c"
#include "elements/content_pane.c"
#include "elements/placeholder.c"
#include "elements/itemlist.c"
#include "elements/combobox.c"