/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_drop_list_24xrgb", /* Test name */
    173, 288, 441, 460  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap slider
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_DROP_LIST     *drop_list;
int               frame_id = 1;
GX_EVENT          my_event;
GX_POINT          click_pos;
int               x_pos;
int               y_pos;
int               open_height;

    ToggleScreen(pWindowScreen, pButtonScreen);
    gx_widget_find((GX_WIDGET *) pWindowScreen, ID_DROP_LIST, GX_SEARCH_DEPTH_INFINITE, (GX_WIDGET **) &drop_list);

    click_pos.gx_point_x = drop_list->gx_widget_size.gx_rectangle_right - 10;
    click_pos.gx_point_y = drop_list->gx_widget_size.gx_rectangle_top + 10;

    memset(&my_event, 0, sizeof(GX_EVENT));

    open_height = drop_list->gx_drop_list_open_height;
    x_pos = drop_list->gx_widget_size.gx_rectangle_left + 10;
    y_pos = drop_list->gx_widget_size.gx_rectangle_bottom - 10;

    /* Show popup list first and then select an item from popup list.  */
    for (; y_pos < drop_list->gx_widget_size.gx_rectangle_bottom + open_height; y_pos += 28)
    {
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = click_pos.gx_point_y;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = click_pos.gx_point_x;
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_display_handle = 1;
        gx_system_event_send(&my_event);
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);

        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "click on drop list button.");
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
        
        sprintf(test_comment, "selected an item");
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y_pos;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = x_pos;
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_display_handle = 1;
        gx_system_event_send(&my_event);
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);

        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "click on point(%d, %d) to select an item", x_pos, y_pos);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}