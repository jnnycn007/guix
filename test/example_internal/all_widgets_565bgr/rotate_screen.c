/****************************************************************/
/* This file tests gx_utility_4444argb_roate and                */
/*                 _gx_display_driver_16bpp_pixelmap_rotate     */
/*                                                              */
/****************************************************************/
#include <stdio.h>
#include "gx_api.h"
#include "all_widgets_565bgr_resources.h"
#include "all_widgets_565bgr_specifications.h"

extern GX_WINDOW_ROOT *root;
extern VOID memory_free(VOID *mem);

INT                   angle = 90;
INT                   rotate_pixelmap_index = 0;
GX_RESOURCE_ID        rotate_pixelmap_id[] = { GX_PIXELMAP_ID_ROTATE_FOOT, GX_PIXELMAP_ID_ROTATE_APPLE, GX_PIXELMAP_ID_ROTATE_FISH, GX_PIXELMAP_ID_BLACK_PAUSE_ALPHAMAP};
GX_BOOL               direct = GX_FALSE;

VOID update_angle_prompt()
{
    GX_PROMPT *pp;
    static GX_CHAR rotate_angle[10];
    GX_STRING string;

    gx_widget_find(&rotate_screen, ID_ANGLE_DISPLAY, 0, &pp);

    if (pp)
    {
        gx_utility_ltoa(angle, rotate_angle, 10);
        string.gx_string_ptr = rotate_angle;
        string.gx_string_length = strnlen(rotate_angle, sizeof(rotate_angle));
        gx_prompt_text_set_ext(pp, &string);
    }
}

UINT rotate_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
UINT status = 0;

    switch (myevent->gx_event_type)
    {
    case GX_SIGNAL(ID_SLIDER_ANGLE, GX_EVENT_SLIDER_VALUE):
        angle = myevent->gx_event_payload.gx_event_longdata;
        update_angle_prompt();
        gx_system_dirty_mark(&rotate_screen);
        break;

    case GX_SIGNAL(ID_RADIO_FOOT, GX_EVENT_RADIO_SELECT):
        rotate_pixelmap_index = 0;
        gx_system_dirty_mark(&rotate_screen);
        break;

    case GX_SIGNAL(ID_RADIO_APPLE, GX_EVENT_RADIO_SELECT):
        rotate_pixelmap_index = 1;
        gx_system_dirty_mark(&rotate_screen);
        break;

    case GX_SIGNAL(ID_RADIO_FISH, GX_EVENT_RADIO_SELECT):
        rotate_pixelmap_index = 2;
        gx_system_dirty_mark(&rotate_screen);
        break;
        
    case GX_SIGNAL(ID_DIRECT_ROTATE, GX_EVENT_TOGGLE_ON):
        direct = GX_TRUE;
        gx_system_dirty_mark(&rotate_screen);
        break;

    case GX_SIGNAL(ID_DIRECT_ROTATE, GX_EVENT_TOGGLE_OFF):
        direct = GX_FALSE;
        gx_system_dirty_mark(&rotate_screen);
        break;

    default:
        return next_button_handler(window, myevent);
        break;
    }
    return status;
}

VOID pixelmap_draw(GX_WINDOW *window)
{
INT  xpos;
INT  ypos;
GX_PIXELMAP *pixelmap;
GX_RECTANGLE win_size;
GX_PIXELMAP destination;
INT x_axis_old;
INT y_axis_old;
INT x_axis;
INT y_axis;

    gx_window_draw((GX_WINDOW*)window);

    gx_widget_pixelmap_get(window, rotate_pixelmap_id[rotate_pixelmap_index], &pixelmap);
    win_size = rotate_screen.rotate_screen_pixelmap_window.gx_widget_size;

    xpos = (win_size.gx_rectangle_right + win_size.gx_rectangle_left - pixelmap->gx_pixelmap_width) >> 1;
    ypos = (win_size.gx_rectangle_top + win_size.gx_rectangle_bottom - pixelmap->gx_pixelmap_height) >> 1;

    if (direct)
    {
        gx_canvas_pixelmap_rotate(xpos, ypos, pixelmap, angle, -1, -1);
    }
    else
    {
        x_axis_old = (pixelmap->gx_pixelmap_width >> 1);
        y_axis_old = (pixelmap->gx_pixelmap_height >> 1);

        x_axis = x_axis_old;
        y_axis = y_axis_old;

        if (gx_utility_pixelmap_rotate(pixelmap, angle, &destination, &x_axis, &y_axis) == GX_SUCCESS)
        {
            xpos += x_axis_old - x_axis;
            ypos += y_axis_old - y_axis;

            /* Draw rotated pixelmap.  */
            gx_canvas_pixelmap_draw(xpos, ypos, &destination);

            /* free memory of the rotated pixelmap */
            if (destination.gx_pixelmap_data)
            {
                memory_free((VOID *)destination.gx_pixelmap_data);
            }
            if (destination.gx_pixelmap_aux_data)
            {
                memory_free((VOID *)destination.gx_pixelmap_aux_data);
            }
        }
        else
        {
            gx_canvas_pixelmap_draw(xpos, ypos, pixelmap);
        }
    }
}
