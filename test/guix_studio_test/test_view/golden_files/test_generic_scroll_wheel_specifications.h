/*******************************************************************************/
/*  This file is auto-generated by Azure RTOS GUIX Studio. Do not edit this    */
/*  file by hand. Modifications to this file should only be made by running    */
/*  the Azure RTOS GUIX Studio application and re-generating the application   */
/*  specification file(s). For more information please refer to the Azure RTOS */
/*  GUIX Studio User Guide, or visit our web site at azure.com/rtos            */
/*                                                                             */
/*  GUIX Studio Revision 6.1.6.1                                               */
/*  Date (dd.mm.yyyy): 11. 5.2021   Time (hh:mm): 13:56                        */
/*******************************************************************************/


#ifndef _TEST_GENERIC_SCROLL_WHEEL_SPECIFICATIONS_H_
#define _TEST_GENERIC_SCROLL_WHEEL_SPECIFICATIONS_H_

#include "gx_api.h"

/* Determine if C++ compiler is being used, if so use standard C.              */
#ifdef __cplusplus
extern   "C" {
#endif

/* Define widget ids                                                           */



/* Define animation ids                                                        */

#define GX_NEXT_ANIMATION_ID 1


/* Define user event ids                                                       */

#define GX_NEXT_USER_EVENT_ID GX_FIRST_USER_EVENT


/* Declare properties structures for each utilized widget type                 */

typedef struct GX_STUDIO_WIDGET_STRUCT
{
   GX_CHAR *widget_name;
   USHORT  widget_type;
   USHORT  widget_id;
   #if defined(GX_WIDGET_USER_DATA)
   INT   user_data;
   #endif
   ULONG style;
   ULONG status;
   ULONG control_block_size;
   GX_RESOURCE_ID normal_fill_color_id;
   GX_RESOURCE_ID selected_fill_color_id;
   GX_RESOURCE_ID disabled_fill_color_id;
   UINT (*create_function) (GX_CONST struct GX_STUDIO_WIDGET_STRUCT *, GX_WIDGET *, GX_WIDGET *);
   void (*draw_function) (GX_WIDGET *);
   UINT (*event_function) (GX_WIDGET *, GX_EVENT *);
   GX_RECTANGLE size;
   GX_CONST struct GX_STUDIO_WIDGET_STRUCT *next_widget;
   GX_CONST struct GX_STUDIO_WIDGET_STRUCT *child_widget;
   ULONG control_block_offset;
   GX_CONST void *properties;
} GX_STUDIO_WIDGET;

typedef struct
{
    GX_CONST GX_STUDIO_WIDGET *widget_information;
    GX_WIDGET        *widget;
} GX_STUDIO_WIDGET_ENTRY;

typedef struct
{
    GX_RESOURCE_ID string_id;
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
    VOID (*format_func)(GX_NUMERIC_PROMPT *, INT);
    INT            numeric_prompt_value;
} GX_NUMERIC_PROMPT_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID wallpaper_id;
} GX_WINDOW_PROPERTIES;

typedef struct
{
    int             total_rows;
    int             selected_row;
    GX_VALUE        row_height;
    GX_UBYTE        start_alpha;
    GX_UBYTE        end_alpha;
    GX_RESOURCE_ID  wallpaper_id;
    GX_RESOURCE_ID  selected_background;
    VOID          (*callback)(GX_GENERIC_SCROLL_WHEEL *, GX_WIDGET *, INT);

} GX_GENERIC_SCROLL_WHEEL_PROPERTIES;


/* Declare top-level control blocks                                            */

typedef struct MAIN_SCREEN_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    GX_GENERIC_SCROLL_WHEEL main_screen_generic_scroll_wheel;
    GX_NUMERIC_PROMPT main_screen_prompt;
    GX_NUMERIC_PROMPT main_screen_prompt_1;
    GX_NUMERIC_PROMPT main_screen_prompt_2;
    GX_NUMERIC_PROMPT main_screen_prompt_3;
    GX_NUMERIC_PROMPT main_screen_prompt_4;
    GX_NUMERIC_PROMPT main_screen_prompt_5;
    GX_NUMERIC_PROMPT main_screen_prompt_6;
    GX_NUMERIC_PROMPT main_screen_prompt_7;
    GX_GENERIC_SCROLL_WHEEL main_screen_generic_scroll_wheel_1;
    GX_NUMERIC_PROMPT main_screen_prompt_8;
    GX_NUMERIC_PROMPT main_screen_prompt_9;
    GX_NUMERIC_PROMPT main_screen_prompt_10;
    GX_NUMERIC_PROMPT main_screen_prompt_11;
    GX_NUMERIC_PROMPT main_screen_prompt_12;
    GX_NUMERIC_PROMPT main_screen_prompt_13;
    GX_NUMERIC_PROMPT main_screen_prompt_14;
    GX_NUMERIC_PROMPT main_screen_prompt_15;
    GX_GENERIC_SCROLL_WHEEL main_screen_generic_scroll_wheel_nowrap;
    GX_GENERIC_SCROLL_WHEEL main_screen_generic_scroll_wheel_nowrap_1;
} MAIN_SCREEN_CONTROL_BLOCK;


/* extern statically defined control blocks                                    */

#ifndef GUIX_STUDIO_GENERATED_FILE
extern MAIN_SCREEN_CONTROL_BLOCK main_screen;
#endif

/* Declare event process functions, draw functions, and callback functions     */

VOID generic_scroll_callback(GX_GENERIC_SCROLL_WHEEL *wheel, GX_WIDGET *widget, INT);

/* Declare the GX_STUDIO_DISPLAY_INFO structure                                */


typedef struct GX_STUDIO_DISPLAY_INFO_STRUCT 
{
    GX_CONST GX_CHAR *name;
    GX_CONST GX_CHAR *canvas_name;
    GX_CONST GX_THEME **theme_table;
    GX_CONST GX_STRING **language_table;
    USHORT   theme_table_size;
    USHORT   language_table_size;
    UINT     string_table_size;
    UINT     x_resolution;
    UINT     y_resolution;
    GX_DISPLAY *display;
    GX_CANVAS  *canvas;
    GX_WINDOW_ROOT *root_window;
    GX_COLOR   *canvas_memory;
    ULONG      canvas_memory_size;
    USHORT     rotation_angle;
} GX_STUDIO_DISPLAY_INFO;


/* Declare Studio-generated functions for creating top-level widgets           */

UINT gx_studio_numeric_prompt_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_window_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_generic_scroll_wheel_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
GX_WIDGET *gx_studio_widget_create(GX_BYTE *storage, GX_CONST GX_STUDIO_WIDGET *definition, GX_WIDGET *parent);
UINT gx_studio_named_widget_create(char *name, GX_WIDGET *parent, GX_WIDGET **new_widget);
UINT gx_studio_display_configure(USHORT display, UINT (*driver)(GX_DISPLAY *), GX_UBYTE language, USHORT theme, GX_WINDOW_ROOT **return_root);

/* Determine if a C++ compiler is being used.  If so, complete the standard
  C conditional started above.                                                 */
#ifdef __cplusplus
}
#endif

#endif                                       /* sentry                         */
