/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "gx_display.h"
#include "gx_multi_line_text_view.h"
#include "all_widgets_8bpp_resources.h"
#include "all_widgets_8bpp_specifications.h"

#define DROP_LIST_VISIBLE_ROWS 5
#define NUMBER_ANIMATION_TIMER 2
#define NESTED_WINDOW_WIDTH  94
#define NESTED_WINDOW_HEIGHT  73

#define            SCRATCHPAD_PIXELS (MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_X_RESOLUTION)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

GX_WINDOW *pTextScreen;
GX_WINDOW *pWindowScreen;
GX_WINDOW *pButtonScreen;
GX_WINDOW *pIndicatorScreen;
GX_WINDOW *pScrollWheelScreen;

extern MENU_SCREEN_CONTROL_BLOCK menu_screen;

GX_SCROLLBAR list_scroll;
TX_BYTE_POOL mem_pool;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW          NestedWindow;
GX_WINDOW_ROOT     nested_root;
GX_BOOL            day_wheel_selected_row_changed = GX_FALSE;/* Used for testing*/
GX_BOOL            month_wheel_selected_row_changed = GX_FALSE;/* Used for testing*/

GX_COLOR           composite_canvas_memory[MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_Y_RESOLUTION];
GX_COLOR           animation_canvas_memory[NESTED_WINDOW_WIDTH * NESTED_WINDOW_HEIGHT];

GX_CANVAS          composite_canvas;
GX_CANVAS          animation_canvas;

extern GX_STUDIO_DISPLAY_INFO all_widgets_8bpp_display_table[];
INT numeric_prompt_value = 10000;

GX_WINDOW_ROOT    *root;
typedef struct {
    GX_PROMPT prompt;
    CHAR text[80];
} DROP_LIST_WIDGET;

DROP_LIST_WIDGET drop_list_widgets[DROP_LIST_VISIBLE_ROWS + 1];

/* Define prototypes.   */
VOID PopulateDropList();
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_8bit_palette(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&mem_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

VOID memory_free(VOID *mem)
{
    tx_byte_release(mem);
}


VOID tx_application_define(void *first_unused_memory)
{
    /* create byte pool for rotate to use */
    tx_byte_pool_create(&mem_pool, "scratchpad", scratchpad,
                        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "GUIX Demo Thread", demo_thread_entry, 
                     0,  demo_thread_stack, sizeof(demo_thread_stack), 
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}


GX_STUDIO_DISPLAY_INFO display_info;

VOID  demo_thread_entry(ULONG thread_input)
{
    GX_RECTANGLE winsize;

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_8bit_palette, 
                                LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);

    /* create the button screen */
    gx_studio_named_widget_create("button_screen", (GX_WIDGET *) root, (GX_WIDGET **) &pButtonScreen);

    /* create the text widget screen */
    gx_studio_named_widget_create("text_screen", GX_NULL, (GX_WIDGET **) &pTextScreen);

    /* create the windows screen */
    gx_studio_named_widget_create("window_screen", GX_NULL, (GX_WIDGET **) &pWindowScreen);
    PopulateDropList();

    /* create the indicator screen */
    gx_studio_named_widget_create("indicator_screen", GX_NULL, (GX_WIDGET **) &pIndicatorScreen);

    /* create menu screen */
    gx_studio_named_widget_create("menu_screen", GX_NULL, GX_NULL);

    gx_studio_named_widget_create("rotate_screen", GX_NULL, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* create the composite canvas used during animations */
    gx_canvas_create(&composite_canvas, GX_NULL,
        all_widgets_8bpp_display_table[0].display,
        GX_CANVAS_COMPOSITE,
        root->gx_widget_size.gx_rectangle_right - root->gx_widget_size.gx_rectangle_left + 1,
        root->gx_widget_size.gx_rectangle_bottom - root->gx_widget_size.gx_rectangle_top + 1,
        composite_canvas_memory, MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_Y_RESOLUTION * sizeof(GX_COLOR));
    gx_canvas_alpha_set(&composite_canvas, 255);

    /* create the nested window shown on the window screen */

    winsize.gx_rectangle_left = winsize.gx_rectangle_top = 0;
    winsize.gx_rectangle_right = NESTED_WINDOW_WIDTH - 1;
    winsize.gx_rectangle_bottom = NESTED_WINDOW_HEIGHT - 1;
    gx_window_create(&NestedWindow, GX_NULL, GX_NULL, GX_STYLE_BORDER_THIN, 0, &winsize);
    gx_widget_fill_color_set(&NestedWindow, GX_COLOR_ID_BLUE, GX_COLOR_ID_BLUE, GX_COLOR_ID_BLUE);

    /* create the root window used for window blend animation */
    gx_window_root_create(&nested_root, "nested_root", &animation_canvas, GX_STYLE_BORDER_NONE, GX_NULL, &NestedWindow.gx_widget_size);
    gx_widget_attach((GX_WIDGET *)&nested_root, (GX_WIDGET *)&NestedWindow);

    /* let GUIX run */
    gx_system_start();
}

void ToggleScreen(GX_WINDOW *new_win, GX_WINDOW *old_win)
{
    if (!new_win->gx_widget_parent)
    {
        gx_widget_attach(root, (GX_WIDGET *) new_win);
    }
    else
    {
        gx_widget_show((GX_WIDGET *) new_win);
    }
    gx_widget_hide((GX_WIDGET *) old_win);
}

UINT next_button_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);
        if (window->gx_widget_id == ID_WINDOW_SCREEN)
        {
            /* Create new canvas to save the semi-transparent window,
            Then set alpha to canvas and shift it.                      */
            gx_canvas_create(&animation_canvas, GX_NULL,
                all_widgets_8bpp_display_table[0].display,
                GX_CANVAS_MANAGED_VISIBLE,
                NESTED_WINDOW_WIDTH, NESTED_WINDOW_HEIGHT,
                animation_canvas_memory, NESTED_WINDOW_WIDTH * NESTED_WINDOW_HEIGHT * sizeof(GX_COLOR));
            gx_canvas_alpha_set(&animation_canvas, 255);
            gx_canvas_offset_set(&animation_canvas, 94, 67);
            gx_widget_show((GX_WIDGET *)&nested_root);

            gx_window_client_scroll(&window_screen.window_screen_scroll_frame_1, 1, 1);
        }
        break;

    case GX_EVENT_HIDE:
        if (window->gx_widget_id == ID_WINDOW_SCREEN)
        {
            gx_widget_hide((GX_WIDGET *)&nested_root);
            gx_canvas_delete(&animation_canvas);
        }
        gx_window_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(IDB_NEXT, GX_EVENT_CLICKED):
        switch(window ->gx_widget_id)
        {
        case ID_BUTTON_SCREEN:
            /* we are on button window, switch to window */
            ToggleScreen(pWindowScreen, window);
            break;

        case ID_WINDOW_SCREEN:
            ToggleScreen(pTextScreen, window);
            gx_system_timer_start(pTextScreen, NUMBER_ANIMATION_TIMER, 20, 20);
            break; 

        case ID_TEXT_SCREEN:
            ToggleScreen(pIndicatorScreen, window);
            gx_system_timer_stop(window, NUMBER_ANIMATION_TIMER);
            break; 

        case ID_INDICATOR_SCREEN:
            gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, (GX_WIDGET **)&pScrollWheelScreen);
            ToggleScreen((GX_WINDOW *)pScrollWheelScreen, window);
            break;

        case ID_SCROLL_WHEEL_SCREEN:
            ToggleScreen((GX_WINDOW *)&menu_screen, window);
            gx_tree_view_selected_set(&menu_screen.menu_screen_tree_view, (GX_WIDGET *)&menu_screen.menu_screen_tree_menu_2_3);
            gx_widget_delete(window);
            break;

        case ID_MENU_SCREEN:
            ToggleScreen((GX_WINDOW *)&rotate_screen, window);
            break;

        case ID_ROTATE_SCREEN:
            ToggleScreen(pButtonScreen, window);
            break;
        }
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == NUMBER_ANIMATION_TIMER)
        {
            numeric_prompt_value++;

            if (numeric_prompt_value > 19999)
            {
                numeric_prompt_value = 10000;
            }
            gx_numeric_prompt_value_set(&text_screen.text_screen_numeric_prompt_1, numeric_prompt_value);
            gx_numeric_pixelmap_prompt_value_set(&text_screen.text_screen_numeric_pixelmap_prompt_2, numeric_prompt_value);
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return 0;
}


VOID drop_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
GX_BOOL created;
GX_RECTANGLE size;
GX_CHAR temp[10];

DROP_LIST_WIDGET *entry = (DROP_LIST_WIDGET *) widget;
GX_STRING string;

    GX_STRCPY(entry->text, "List Entry #");
    gx_utility_ltoa(index + 1, temp, 10);
    GX_STRCAT(entry->text, (char*)temp);

    gx_widget_created_test(widget, &created);

    if (!created)
    {
        gx_utility_rectangle_define(&size, 0, 0, 100, 30);
        gx_prompt_create(&entry->prompt, entry->text, list, 0, GX_STYLE_ENABLED|GX_STYLE_TEXT_LEFT|GX_STYLE_BORDER_NONE, 0, &size);
        gx_widget_fill_color_set(&entry->prompt, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLUE, GX_COLOR_ID_BLACK);
        gx_prompt_text_color_set(&entry->prompt, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
    }
  
    string.gx_string_ptr = entry->text;
    string.gx_string_length = strnlen(entry->text, sizeof(entry->text));
    gx_prompt_text_set_ext(&entry->prompt, &string);
}

VOID PopulateDropList(void)
{
    int index;
    GX_VERTICAL_LIST *list;

    WINDOW_SCREEN_CONTROL_BLOCK *control = (WINDOW_SCREEN_CONTROL_BLOCK *) pWindowScreen;
    GX_DROP_LIST *drop = &control->window_screen_drop_list;
    gx_drop_list_popup_get(drop, &list);

    for (index = 0; index < DROP_LIST_VISIBLE_ROWS; index++)
    {
        drop_list_row_create(list, (GX_WIDGET *) &drop_list_widgets[index], index);
    }

    gx_vertical_scrollbar_create(&list_scroll, "list_scroll", list, GX_NULL,
        GX_SCROLLBAR_VERTICAL | GX_SCROLLBAR_RELATIVE_THUMB | GX_SCROLLBAR_END_BUTTONS | GX_STYLE_ENABLED);

}


VOID format_func(GX_NUMERIC_PROMPT *prompt, INT value)
{
    gx_utility_ltoa(value % 1000, prompt->gx_numeric_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE);
}

VOID numeric_pixelmap_format_func(GX_NUMERIC_PIXELMAP_PROMPT *prompt, INT value)
{
    gx_utility_ltoa(value % 1000, prompt->gx_numeric_pixelmap_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE);
}
