/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_1555xrgb_image_convert_screen", /* Test name */
    60, 75, 590, 365  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap
                         drawing area.  */
};

extern void mark_dirty();
extern void convert_start();

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
#ifdef win32_graphics_driver_setup_1555xrgb
#undef win32_graphics_driver_setup_1555xrgb
#endif
#define win32_graphics_driver_setup_1555xrgb  gx_validation_graphics_driver_setup_1555xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_1555xrgb.c"

char test_comment[256];

extern void convert_start();
extern GX_UBYTE image_reader_mode;
extern GX_UBYTE output_color_format;
extern GX_BOOL  do_color_convert;

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int      frame_id = 1;
GX_EVENT my_event;
int      alpha;
int      compress;
int      dither;
int      alphamap;

    memset(&my_event, 0, sizeof(GX_EVENT));

    /* Toggle to Image Convert Screen. */
    ToggleScreen((GX_WINDOW *)&image_convert_screen, (GX_WINDOW *)&rotate_screen);

    do_color_convert = GX_FALSE;
    mark_dirty();
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Draw raw png/jpeg data.");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    do_color_convert = GX_TRUE;

    for(alphamap = 0; alphamap < 2; alphamap++)
    {
        if(alphamap)
        {
            output_color_format = GX_COLOR_FORMAT_8BIT_ALPHAMAP;
        }
        else
        {
            output_color_format = GX_COLOR_FORMAT_1555XRGB;
        }

        for(alpha = 0; alpha < 2; alpha++)
        {
            if(alpha)
            {
                image_reader_mode |= GX_IMAGE_READER_MODE_ALPHA;
            }
            else
            {
                image_reader_mode &= ~GX_IMAGE_READER_MODE_ALPHA;
            }

            for(compress = 0; compress < 2; compress++)
            {
                if(compress)
                {
                    image_reader_mode |= GX_IMAGE_READER_MODE_COMPRESS;
                }
                else
                {
                    image_reader_mode &= ~GX_IMAGE_READER_MODE_COMPRESS;
                }

                for(dither = 0; dither < 2; dither++)
                {
                    if(dither)
                    {
                        image_reader_mode |= GX_IMAGE_READER_MODE_DITHER;
                    }
                    else
                    {
                        image_reader_mode &= ~GX_IMAGE_READER_MODE_DITHER;
                    }
GX_ENTER_CRITICAL
                    convert_start();
GX_EXIT_CRITICAL
                    /* Capture a frame. */
                    gx_validation_set_frame_id(frame_id++);
                    sprintf(test_comment, "do convert, alphamap = %d, alpha = %d, compress = %d, dither = %d", alphamap, alpha, compress, dither);
                    gx_validation_set_frame_comment(test_comment);
                    gx_validation_screen_refresh();
                }
            }
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
