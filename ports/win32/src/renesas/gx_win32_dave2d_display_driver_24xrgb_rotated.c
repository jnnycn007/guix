/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Win32 Display Management (Display)                                  */
/**                                                                       */
/**************************************************************************/
#ifdef WIN32
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "windows.h"
#include "gx_win32_display_driver.h"
#include "gx_dave2d_simulation_display_driver.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    win32_dave2d_graphics_driver_setup_24xrgb_rotated  PORTABLE C       */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a Windows specific 24xrgb rotated synergy     */
/*    simulation display driver.                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Pointer to GX_DISPLAY         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_dave2d_simulation_display_driver_24xrgb_rotated_setup           */
/*                                          guix display setup funciton.  */
/*    win32_dave2d_simulation_24xrgb_bitmap_header_create                 */
/*                                          Create bitmap header info     */
/*    gx_win32_get_free_data_instance       Get display data instance     */
/*    GX_WIN32_EVENT_THREAD_CREATE          Create event thread           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2023     Ting Zhu                 Initial Version 6.4.0         */
/*                                                                        */
/**************************************************************************/
UINT win32_dave2d_graphics_driver_setup_24xrgb_rotated(GX_DISPLAY* display)
{
    GX_WIN32_DISPLAY_DRIVER_DATA* data;

    /* Initialize the low-level drawing function pointers.

       For windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware.  */

    data = gx_win32_get_free_data_instance();

    if (!data)
    {
        /* We don't have any free display data instance.  */
        return(GX_FAILURE);
    }

    /* Save off the format of this display.  */
    data->win32_driver_type = GX_COLOR_FORMAT_24XRGB;

    _gx_dave2d_simulation_display_driver_24xrgb_rotated_setup(display, data, gx_win32_display_buffer_toggle);

    win32_dave2d_simulation_24xrgb_bitmap_header_create(display);

    /* Create the GUIX / Windows event thread
       This thread is a substitute for a touch screen
       or keyboard driver thread that would be running
       on embedded hardware.  */
    GX_WIN32_EVENT_THREAD_CREATE(data, "GUI-WIN32-24xrgb");

    return(GX_SUCCESS);
}
#endif /* WIN32 */

