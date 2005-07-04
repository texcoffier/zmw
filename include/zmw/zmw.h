/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2005 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact: Thierry.EXCOFFIER@liris.univ-lyon1.fr
*/


#ifndef _HOME_EXCO_HOME_ZMW_ZMW_H
#define _HOME_EXCO_HOME_ZMW_ZMW_H

/*
 * For a faster library, set these debug flags to 0 and recompile all.
 * 
 * ZMW_DEBUG_NAME allows some checking in "zmw_name.c"
 * if it is disabled here, it is not enablable interactively
 * 
 * ZMW_DEBUG_ASSERT is true, the assertion system is enabled
 * 
 * ZMW_DEBUG_INSIDE_ZMW_PARAMETER allows to verify if only the good
 * functions are called inside the parameter of ZMW macro.
 * 
 * ZMW_DEBUG_STORE_WIDGET_TYPE allows to have leaner executable
 * but you lose much information when web browsing the widget tree.
 * 
 * ZMW_PROFILING displays real execution times (not cpu) per source ZMW bloc
 */
 
#define ZMW_DEBUG_OPTIONS_DEFAULT 0

#define ZMW_DEBUG_NAME                 ZMW_DEBUG_OPTIONS_DEFAULT
#define ZMW_DEBUG_ASSERT               ZMW_DEBUG_OPTIONS_DEFAULT
#define ZMW_DEBUG_INSIDE_ZMW_PARAMETER ZMW_DEBUG_OPTIONS_DEFAULT
#define ZMW_DEBUG_STORE_WIDGET_TYPE    ZMW_DEBUG_OPTIONS_DEFAULT
#define ZMW_PROFILING                  ZMW_DEBUG_OPTIONS_DEFAULT

/*
 * If true store bits in bit record and not "char"
 * See applications/benchmarks/README-bit 
 */
#define ZMW_USE_BIT_RECORD 1




#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <gdk/gdk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>


typedef struct zmw_font_description    Zmw_Font_Description ;
typedef struct zmw_state               Zmw_State ;
typedef struct zmw_child               Zmw_Child ;
typedef struct zmw_name                Zmw_Name ;
typedef struct zmw_rectangle           Zmw_Rectangle ;
typedef struct zmw_child_current_state Zmw_Child_Current_State ;
typedef struct zmw_parent_to_child     Zmw_Parent_To_Child ;

typedef unsigned int   Zmw_Hash ;
typedef short          Zmw_Position ;
typedef unsigned short Zmw_Size ;
typedef unsigned char  Zmw_Width ;
typedef unsigned short Zmw_Font_Family ;
typedef unsigned short Zmw_Font_Size ;
typedef unsigned short Zmw_Font_Weight ;
typedef char           Zmw_Boolean ;
typedef char           Zmw_Alignment ;
typedef float          Zmw_Float_0_1 ; /* Number between 0 and 1 included */

typedef enum
  {
    Zmw_Color_Background_Normal,
    Zmw_Color_Background_Pushed,
    Zmw_Color_Background_Poped,
    Zmw_Color_Border_Light,
    Zmw_Color_Border_Dark,
    Zmw_Color_Foreground,
    Zmw_Color_Last
  } Zmw_Color ;

typedef enum zmw_name_type
	 { Zmw_Is_A_Registration
	 , Zmw_Is_A_Resource_String
	 , Zmw_Is_A_Resource_Int
	 , Zmw_Is_A_Resource_Pointer
	 } Zmw_Name_Type ;

typedef enum zmw_subaction
  {
    Zmw_Init,
    Zmw_Compute_Required_Size,
    Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing,
    Zmw_Compute_Children_Allocated_Size,
    Zmw_Post_Drawing,
    Zmw_Input_Event,
    Zmw_Nothing,
    Zmw_Clean,			/* ZMW is shuting down, free resources */
    Zmw_Debug_Message,          /* Display warnings and debugging messages */

    Zmw_Subaction_Last          /* Only mark the table end */
  } Zmw_Subaction ;

typedef enum {
  Zmw_Font_Style_Normal,
  Zmw_Font_Style_Italic,
} Zmw_Font_Style ;

typedef enum {
  Zmw_External_No,
  Zmw_External_Stop,
  Zmw_External_Continue
} Zmw_External_State ;

enum zmw_boolean { Zmw_False, Zmw_True } ;

/*
 * This structure is not in the private area
 * because it is very common to declare variable
 * with this type in the stack.
 * It will be cumbersome to allocate and free rectangles.
 */
struct zmw_rectangle
{
  Zmw_Position x, y ;
  Zmw_Size width, height ;
} ;




#define ZMW_LEFT -1
#define ZMW_CENTER 0
#define ZMW_RIGHT 1
#define ZMW_TOP -1
#define ZMW_BOTTOM 1

#define ZMW_VALUE_UNDEFINED (32767)
#define ZMW_MAX_NAME_LENGTH 1000

#define Zmw_Debug_Window              1
#define Zmw_Debug_Draw_Cross          2
#define Zmw_Debug_Trace               4
#define Zmw_Debug_Event               8
#define Zmw_Debug_Widget             16
#define Zmw_Debug_Draw               32 // Output # of draw in "xxx.nb_draw"
#define Zmw_Debug_Drag               64
#define Zmw_Debug_Window_Auto_Resize 128
#define Zmw_Debug_Cache_Fast         256
#define Zmw_Debug_Cache_Slow         512
#define Zmw_Debug_Name              1024
#define Zmw_Debug_Profiling         2048
#define Zmw_Debug_Navigation        4096

#include <zmw/zmw_generated_public.h>

#if ZMW_DEBUG_INSIDE_ZMW_PARAMETER
#define ZMW_FUNCTION_CALLED_OUTSIDE_ZMW_PARAMETER if ( zmw_zmw_inside_zmw_parameter_get()) ZMW_ABORT
#define ZMW_FUNCTION_CALLED_INSIDE_ZMW_PARAMETER if ( !zmw_zmw_inside_zmw_parameter_get()) ZMW_ABORT
#else
#define ZMW_FUNCTION_CALLED_OUTSIDE_ZMW_PARAMETER
#define ZMW_FUNCTION_CALLED_INSIDE_ZMW_PARAMETER
#endif

#if ZMW_DEBUG_STORE_WIDGET_TYPE
#define ZMW_STORE_WIDGET_TYPE(TYPE) zmw_type_set(#TYPE),zmw_file_set(__FILE__),zmw_line_set(__LINE__),
#else
#define ZMW_STORE_WIDGET_TYPE(TYPE)
#endif



#define ZMW(TYPE) for(							\
		      ZMW_STORE_WIDGET_TYPE(TYPE)			\
			zmw_debug_in_zmw_loop(0,0,__LINE__,__FILE__),	\
			zmw_init_widget(),				\
			zmw_debug_in_zmw_loop(1,0,__LINE__,__FILE__)	\
			;						\
		      zmw_debug_in_zmw_loop(2,0,__LINE__,__FILE__),	\
			(TYPE),						\
			zmw_debug_in_zmw_loop(3,0,__LINE__,__FILE__),	\
			zmw_debug_in_zmw_loop(4, (*zmw_action_get())(),__LINE__,__FILE__) \
			;						\
		      zmw_debug_in_zmw_loop(5,0,__LINE__,__FILE__),	\
			zmw_state_pop()					\
		       )

/*
 * See zmw_viewport.c for example of use
 */

#define ZMW_PFE if (0) zmw_printf

#define ZMW_EXTERNAL_RESTART						\
  ZMW_PFE("ZMW_EXTERNAL_RESTART : begin\n") ;				\
  if ( zmw_call_number_get() )						\
     {									\
       ZMW_PFE("ZMW_EXTERNAL_RESTART : restart internal loop\n") ;	\
       goto restart ;							\
     }									\
 ZMW_PFE("ZMW_EXTERNAL_RESTART : first time\n") ;			\
  zmw_zmw_external_do_not_make_init_set(Zmw_True)


#define ZMW_EXTERNAL_RETURNS(X)				\
{							\
  ZMW_PFE("ZMW_EXTERNAL_RETURN\n") ;			\
  zmw_external_state_set(Zmw_External_Continue) ;		\
  return X ;						\
 restart:						\
  ZMW_PFE("ZMW_EXTERNAL_RETURN : restarted\n") ;	\
  zmw_do_not_execute_pop_set(Zmw_False) ;		\
}

#define ZMW_EXTERNAL ZMW_EXTERNAL_RETURNS()

#define ZMW_EXTERNAL_STOP			\
ZMW_PFE("ZMW_EXTERNAL_STOP\n") ;		\
zmw_external_state_set(Zmw_External_Stop) ;	\
zmw_zmw_external_do_not_make_init_set(Zmw_False) ;


/* It is a macro because it contains a return.... */
/* The children are not executed because they had been
   yet displayed by ZMW_EXTERNAL */
#define ZMW_EXTERNAL_HANDLING					\
  ZMW_PFE("ZMW_EXTERNAL_HANDLING\n") ;				\
  if ( zmw_external_state_get() != Zmw_External_No )			\
    {								\
      int zmw_i_ ;						\
      zmw_i_ = (zmw_external_state_get() == Zmw_External_Continue) ;	\
      ZMW_PFE("ZMW_EXTERNAL_HANDLING : state = %d\n", zmw_i_) ;	\
      zmw_external_state_set(Zmw_External_No) ;			\
      if ( zmw_i_ )						\
               zmw_do_not_execute_pop_set(Zmw_True) ;		\
      return( zmw_i_  ) ;					\
    }

/*
 *
 */

#define ZMW_MALLOC(P,N) P = malloc((N) * sizeof(*(P)))
#define ZMW_MALLOC_0(P,N) P = calloc(N,sizeof(*(P)))
#define ZMW_REALLOC(P,N) P = realloc(P, (N) * sizeof(*(P)))
#define ZMW_FREE(P) ZMW1( free(P) ; (P)=NULL ; )
#define ZMW_TABLE_SIZE(T) (sizeof(T)/sizeof((T)[0]))
#define ZMW_HERE zmw_printf("%s:%s:%d %s\n", __FILE__, __FUNCTION__, __LINE__, zmw_action_name()) 
#define ZMW_ABORT do { ZMW_HERE ; zmw_stack_print() ; abort() ; } while(0)

#define ZMW_MIN(A,B) ((A)<(B)?(A):(B))
#define ZMW_MAX(A,B) ((A)<(B)?(B):(A))
#define ZMW_ABS(A)   ((A)<0?-(A):(A))
#define ZMW_CLAMP(X, MIN, MAX) ZMW1( if ((X)<(MIN)) (X)=MIN ; else if ((X)>(MAX)) (X)=MAX ; )

#if ZMW_DEBUG_ASSERT
#define ZMW_ASSERT(X) ZMW1(				        	\
    if ( !(X) )								\
       {								\
         zmw_printf("ASSERT %s FAILED in %s\n",#X, __FUNCTION__) ;	\
         ZMW_ABORT ;							\
       }								\
    )
#else
#define ZMW_ASSERT(X)
#endif


#define ZMW1(X) do { X } while(0)

#define zmw_name_full zmw_zmw_full_name_get()
#define zmw_font_family(B) zmw_font_family_set(zmw_font_family_index(B))
#define zmw_color(T,P) (zmw_colors_get()[T] = P)
#define zmw_color_get(T) (zmw_colors_get()[T])
#define zmw_is_widget_top() (zmw_state_get() <= zmw_zmw_table_get() + 1)
#define zmw_child__required_padded_width(I) (zmw_child__required_width_get(I) + 2*zmw_child__cs_padding_width_get(I))
#define zmw_child__required_padded_height(I) (zmw_child__required_height_get(I) + 2*zmw_child__cs_padding_width_get(I))
#define zmw_debug(X) zmw_debug_set(X)
#define zmw_horizontal_expand(X) zmw_cs_horizontal_expand_set(X)
#define zmw_vertical_expand(X) zmw_cs_vertical_expand_set(X)
#define zmw_auto_resize(X) zmw_auto_resize_set(X)
#define zmw_horizontal_alignment(X) zmw_cs_horizontal_alignment_set(X)
#define zmw_vertical_alignment(X) zmw_cs_vertical_alignment_set(X)
#define zmw_font_size(X) zmw_font_size_set(X)
#define zmw_font_weight(X) zmw_font_weight_set(X)
#define zmw_font_style(X) zmw_font_style_set(X)
#define zmw_border_width(X) zmw_border_width_set(X)
#define zmw_focus_width(X) zmw_focus_width_set(X)
#define zmw_padding_width(X) zmw_cs_padding_width_set(X)
#define zmw_sensitive(X) zmw_sensitive_set(X)
#define zmw_x(X) zmw_asked_x_set(X)
#define zmw_y(X) zmw_asked_y_set(X)
#define zmw_width(X) zmw_asked_width_set(X)
#define zmw_height(X) zmw_asked_height_set(X)
#define zmw_focused(X) zmw_focused_get(X)
#define zmw_window_id() (*zmw_window_get())

/* We do not test zmw_subaction_get() because :
 * - It doesn't work if tested before the first widget container
 * - It doesn't work if tested after a zmw_if(0)
 */
#define zmw_drawing() ( zmw_parent__subaction_get() == Zmw_Post_Drawing )
#define zmw_debug_message() (zmw_subaction_get() == Zmw_Debug_Message \
			&& (http_printf("<B><U>%s:%s:%d</U></B>", __FILE__, __FUNCTION__, __LINE__),1) )

/*
 * zmw_main.c
 */
void zmw_init(int *argc, char ***argv) ;
void zmw_main(void (*fct)(void)) ;
void zmw_main_quit(int r) ;
void zmw_do_nothing(void) ;
void zmw_need_repaint(void) ;
void zmw_need_dispatch(void) ;
void zmw_draw(void (*fct)(void)) ;
void zmw_call_widget(void (*fct)(void), int (*action)(void)) ;
void zmw_use_window_from_button_press(Zmw_Boolean b) ;
Zmw_Boolean zmw_use_window_from_button_press_get(void) ;
/*
 * zmw.c
 */
int zmw_printf(const char *format, ...) ;
const char *zmw_action_name(void) ;
const char *zmw_action_name_fct(void) ;
void zmw_name(const char *s) ;
Zmw_Font_Family zmw_font_family_index(const char *s) ;
void zmw_string_copy(char **to, const char *from) ;
Zmw_Boolean zmw_font_description_equals(const Zmw_Font_Description*,const Zmw_Font_Description*) ;
void zmw_state_init(Zmw_State *s) ; /* Used only by zmw_main.c */
void zmw_state_pop(void) ;
Zmw_Hash zmw_hash(Zmw_Hash, const char*) ;
Zmw_Child* zmw_widget_previous_size(void) ;
void zmw_state_push(void) ;
void zmw_init_widget(void) ;
const char *zmw_action_name_(Zmw_Subaction sa) ;
int zmw_action_compute_required_size(void) ;
int zmw_action_draw(void) ;
int zmw_action_dispatch_event(void) ;
int zmw_action_search(void) ;
int zmw_action_dispatch_accelerator(void) ;
int zmw_action_first_pass(void) ;
int zmw_action_clean(void) ;
void zmw_action_second_pass(void) ;
void zmw_debug_trace(void) ;
void zmw_stack_print(void) ;
int zmw_event_in(void) ;
void zmw_focus(Zmw_Name *focus) ;



/*
 * zmw_name.c
 */
void zmw_name_free(void) ;
void zmw_name_debug_window(void) ;
void zmw_name_register(Zmw_Name *n) ;
char* zmw_name_registered(const Zmw_Name *n) ;
void zmw_name_register_with_name(Zmw_Name *n, const char *name) ;
void zmw_name_unregister(Zmw_Name *n) ;
void zmw_name_unregister_value(const char *name, const char *why) ;
void zmw_name_unregister_value_by_pointer(const char *why, void *p) ;
Zmw_Boolean zmw_name_get_value_pointer(const char *why, void **value);
Zmw_Boolean zmw_name_get_value_int(const char *why, int *value) ;
void zmw_name_initialize(Zmw_Name **name, char *why) ;
void zmw_name_set_value_pointer(const char *why, void *value);
void zmw_name_set_value_int(const char *why, int value) ;
void *zmw_name_get_pointer_on_resource_with_name_and_type(const char *name, const char *why, Zmw_Name_Type nt) ;
int *zmw_name_get_pointer_on_int_resource_with_name(const char *name, const char *why) ;
int *zmw_name_get_pointer_on_int_resource(const char *why) ;
Zmw_Boolean zmw_name_get_value_pointer_with_name(const char *name, const char *why, void **value);
Zmw_Boolean zmw_name_get_value_int_with_name(const char *name, const char *why, int *value) ;
void zmw_name_set_value_pointer_with_name(const char *name, const char *why, void *value);
void zmw_name_set_value_int_with_name(const char *name, const char *why, int value) ;
void zmw_name_dump(FILE *f) ;
Zmw_Boolean zmw_name_is(const Zmw_Name *n) ;
Zmw_Boolean zmw_name_contains(const Zmw_Name *n) ;
Zmw_Boolean zmw_name_is_inside(const Zmw_Name *n) ;

void zmw_resource_get(void **pointer_value, const char *resource
		      , void *default_value, Zmw_Name_Type nt) ;
void zmw_resource_int_get(int **pointer_value, const char *resource
			  , int default_value) ;
void zmw_resource_pointer_get(void **pointer_value, const char *resource
			      , void *default_value) ;



/*
 * zmw_rectangle.c
 */
/* void zmw_compute_box_size(int allocated) ; */
Zmw_Rectangle zmw_rectangle_max(const Zmw_Rectangle *a, const Zmw_Rectangle *b) ;
Zmw_Rectangle zmw_rectangle_min(const Zmw_Rectangle *a, const Zmw_Rectangle *b) ;
char *zmw_rectangle_string(const Zmw_Rectangle *a) ;
void zmw_rectangle_void(Zmw_Rectangle *a) ;
void zmw_padding_add(Zmw_Rectangle *r, Zmw_Width padding) ;
void zmw_padding_remove(Zmw_Rectangle *r, Zmw_Width padding) ;

/*
 * zmw_device.c
 */
int zmw_rgb_to_int(Zmw_Float_0_1 r, Zmw_Float_0_1 g, Zmw_Float_0_1 b) ;
void zmw_int_to_rgb(int c,Zmw_Float_0_1 *r, Zmw_Float_0_1 *g, Zmw_Float_0_1*b);
void zmw_draw_line(Zmw_Color c, Zmw_Position x1, Zmw_Position y1
		   , Zmw_Position x2, Zmw_Position y2) ;
void zmw_draw_rectangle(Zmw_Color c, Zmw_Boolean filled
			, Zmw_Position x, Zmw_Position y
			, Zmw_Size width, Zmw_Size height) ;
void zmw_pixbuf_render_to_drawable(GdkPixbuf *pb
				   , Zmw_Position x, Zmw_Position y) ;

void zmw_draw_clip_set(void) ;
#define zmw_foreground(R,G,B) zmw_color(Zmw_Color_Foreground, zmw_rgb_to_int(R,G,B))
#define zmw_background(R,G,B) zmw_color(Zmw_Color_Background_Normal, zmw_rgb_to_int(R,G,B))
void zmw_text_init(int pango_cache_size) ;
void zmw_text_exit(void) ;
void zmw_text_set_text(char *text, const Zmw_Font_Description *fd) ;
void zmw_text_render(Zmw_Color c, Zmw_Position xx, Zmw_Position yy) ;
void zmw_text_get_size(Zmw_Size *width, Zmw_Size *height) ;
int zmw_text_xy_to_index(Zmw_Position x, Zmw_Position y) ;
void zmw_text_get_grapheme_rectangle(int index, Zmw_Rectangle *rect) ;
void zmw_text_cursor_position(int cursor_pos, Zmw_Rectangle *r) ;
int zmw_text_next_char(int cursor) ;
int zmw_text_previous_char(int cursor) ;
int zmw_text_delete_char(int cursor) ;
int zmw_text_up_char(int cursor) ;
int zmw_text_down_char(int cursor) ;
int zmw_text_home_char(int cursor) ;
int zmw_text_end_char(int cursor) ;

/*
 * zmw_graphic.c
 */
#define Zmw_Border_Draw_Focus  1
#define Zmw_Border_Focusable   2
#define Zmw_Border_Solid       4
#define Zmw_Border_Embossed    8
#define Zmw_Border_Relief      16
#define Zmw_Border_Background  32
#define Zmw_Border_In          64
#define Zmw_Border_Out         128
#define Zmw_Border_Relief_In   (Zmw_Border_Relief | Zmw_Border_In)
#define Zmw_Border_Relief_Out   Zmw_Border_Relief
#define Zmw_Border_Embossed_In (Zmw_Border_Embossed | Zmw_Border_In)
#define Zmw_Border_Embossed_Out Zmw_Border_Embossed
void zmw_border_draw(int options) ;
void zmw_border_draw_with_rectangle(int options, const Zmw_Rectangle *r) ;
void zmw_border_solid_draw(void) ;
void zmw_border_relief_in_draw(void) ;
void zmw_border_relief_out_draw(void) ;
void zmw_border_embossed_in_draw(void) ;
void zmw_border_embossed_out_draw(void) ;
void zmw_cross_draw(void) ;
void zmw_rgb(Zmw_Float_0_1 r, Zmw_Float_0_1 g, Zmw_Float_0_1 b) ;
/*
 * zmw_event.c
 */

Zmw_Boolean zmw_activated(void) ;
Zmw_Boolean zmw_activated_previous(void) ;
Zmw_Boolean zmw_changed(void) ;
Zmw_Boolean zmw_button_pressed(void) ;
Zmw_Boolean zmw_button_released(void) ;
Zmw_Boolean zmw_button_released_anywhere(void) ;
Zmw_Boolean zmw_focus_in(void) ;
Zmw_Boolean zmw_key_pressed_unsensitive(void) ;
Zmw_Boolean zmw_key_pressed(void) ;
Zmw_Boolean zmw_key_string_unsensitive(void) ;
Zmw_Boolean zmw_selected(void) ;
Zmw_Boolean zmw_selected_by_its_parents(void) ;
Zmw_Boolean zmw_accelerator(GdkModifierType state, int character) ;
Zmw_Boolean zmw_focused_by_its_parents(void) ;
Zmw_Boolean zmw_cursor_enter(void) ;
Zmw_Boolean zmw_cursor_leave(void) ;
void zmw_cursor_set(char *name) ;
void zmw_focus_remove(void) ;
Zmw_Boolean zmw_tip_visible(void) ;
Zmw_Boolean zmw_cursor_in_and_pressed(void) ;
Zmw_Boolean zmw_cursor_in(void) ;
void zmw_event_remove(void) ;
void zmw_window_update_uppers(int action) ;
void zmw_window_unpop_all(void) ;
void zmw_widget_is_tested(void) ;
Zmw_Boolean zmw_selection_have(void) ;
void zmw_selection_clear(void) ;
void zmw_selection_take(void) ;
void zmw_focusable(void) ;
void zmw_activable(void) ;
void zmw_event_button_release(void) ;
Zmw_Boolean zmw_event_to_process(void) ;
Zmw_Boolean zmw_state_change_allowed(void) ;
void zmw_event_debug_window(void) ;
void zmw_event_dump(void) ;
void zmw_event_initialize(void) ;

#define Zmw_Menu_Is_Detached         1        
#define Zmw_Menu_Is_Poped            2           
#define Zmw_Menu_Contains_A_Detached 4
#define Zmw_Menu_State_New           8        
Zmw_Boolean zmw_window_is_popped_with_detached(int *detached) ;
Zmw_Boolean zmw_window_is_popped(void) ;


/*
 * zmw_drag.c
 */
Zmw_Boolean zmw_drag_from_started(void) ;
Zmw_Boolean zmw_drag_from_stopped(void) ;
Zmw_Boolean zmw_drag_from_running(void) ;

Zmw_Boolean zmw_drag_to_enter(void) ;
Zmw_Boolean zmw_drag_to_leave(void) ;
Zmw_Boolean zmw_drag_to_dropped(void) ;

Zmw_Boolean zmw_drag_accept_get(void) ;
void zmw_drag_accept_set(Zmw_Boolean bool) ;

void zmw_drag_data_set(const char *data) ;
char *zmw_drag_data_get(void) ;

void zmw_drag_to_nothing(void) ;
void zmw_drag_cancel(void) ;

void zmw_drag_swap(int *p, int **current) ;

void zmw_drag_debug(FILE *f) ;
void zmw_drag_debug_window(void) ;
void zmw_drag_initialize(void) ;

/*
 * zmw_cache.c
 */
const char* zmw_child_string(const Zmw_Child *s) ;
void zmw_child_cache_init(int size) ;
void zmw_child_cache_free(void) ;
int zmw_child_cache_get_size(Zmw_Child *r) ;
void zmw_child_cache_set_size(const Zmw_Child *r) ;
int zmw_child_cache_size(void) ;


/*
 * This function is inline static to optimize time.
 *
 * A side effect is that there is a table per file to collect profiling.
 * So only the line numbers are needed :-)
 */


static inline int zmw_debug_in_zmw_loop(int i, int j, int line, char *file)
{
  if ( i==2 )
    {
      if (zmw_debug_get() & Zmw_Debug_Trace)
	zmw_debug_trace() ;

      zmw_zmw_inside_zmw_parameter_set(Zmw_True) ;
    }

  if ( i==4 )
    zmw_zmw_inside_zmw_parameter_set(Zmw_False) ;

#if ZMW_PROFILING 
  if ( zmw_debug_get() & Zmw_Debug_Profiling )
    {
      int k, n, m, s, total ;
      long diff ;
      struct timeval tv ;
      static struct
      {
	int line ;
	int nb_init, nb_loop ;
	struct timeval tv ;
	int microsec[3+Zmw_Subaction_Last] ;
      } *stat = NULL ;
      static int stat_nb = 0 ;
      static Zmw_Boolean terminating = Zmw_False ;

      for (k=0; k<stat_nb; k++)
	if ( line == stat[k].line )
	  break ;
      if ( k == stat_nb )
	{
	  if ( terminating )
	    return j ;

	  ZMW_REALLOC(stat, stat_nb+1) ;
	  memset(&stat[k], 0, sizeof(stat[k])) ;
	  {
	    int ii ;
	    for(ii=0; ii<3+Zmw_Subaction_Last; ii++)
	      stat[k].microsec[ii] = 0 ;
	  }
	  stat[k].line = line ;
	  stat_nb++ ;
	  ZMW_ASSERT( i == 0 ) ;
	}

      gettimeofday(&tv, NULL) ;
      diff = (tv.tv_sec - stat[k].tv.tv_sec)*1000000
	+  tv.tv_usec - stat[k].tv.tv_usec ;
      switch(i)
	{
	case 1: // zmw_init_widget() time
	  stat[k].microsec[0] += diff ;
	  stat[k].nb_init++ ;
	  break ;
	case 3: // (TYPE) time
	  if ( zmw_external_state_get() != Zmw_External_No )
	    break ;

	  stat[k].nb_loop++ ;
	  stat[k].microsec[3+zmw_subaction_get()] += diff ;
	  break ;
	case 4: // (*zmw_action_get())() time
	  stat[k].microsec[1] += diff ;
	  break ;
	case 5: // loop time
	  stat[k].microsec[2] += diff ;
	  break ;
	}
      stat[k].tv = tv ;


      if ( i == 4 && j == 0 && zmw_action_get() == zmw_action_clean )
	{
	  terminating = Zmw_True ;
	  if ( ! zmw_zmw_profiling_displayed_get() )
	    {
	      zmw_zmw_profiling_displayed_set(1) ;
	      fprintf(stderr,
		      "Time are in microseconds of real time (not CPU).\n"
		      "LOOP time contains the children times.\n"
		      ) ;
	      fprintf(stderr,
		      "  TOTAL ;In;Ac;Lo;In;Re;Al;Al;Po;In;No;Cl;Me;TY; #init  ; #loop  ;File name\n"
		      "        ;it;ti;op;it;Si;Dr;Si;Dr;Ev;th;ea;ss;PE;        ;        ;\n"
		      "        ;  ;on;  ;  ;ze;aw;ze;aw;en;in;n ;ag;  ;        ;        ;\n"
			) ;
	    }

	  total = 0 ;
	  for(m=0; m<3+Zmw_Subaction_Last; m++)
	    {
	      total += stat[k].microsec[m] ;
	    }
	  fprintf(stderr,"%8d;", total) ;

	  for(n=0; n<4; n++)
	    {
	      if ( n == 3 )
		{
		  s = 0 ;
		  for(m=0; m<Zmw_Subaction_Last; m++)
		    {
		      s += stat[k].microsec[n+m] ;
		      fprintf(stderr, "%2d;"
			      , (int)((100.*stat[k].microsec[n+m]) / total)) ;
		    }
		  fprintf(stderr, "%2d;", (int)((100.*s)/total)) ;
		}
	      else
		fprintf(stderr, "%2d;", (int)((100.*stat[k].microsec[n])/total)) ;
	    }
	  fprintf(stderr, "%8d;%8d;"
		  , stat[k].nb_init
		  , stat[k].nb_loop
		  ) ;
	  fprintf(stderr, "%s:%d\n", file, line) ;

	  memmove(&stat[k], &stat[k+1], (stat_nb-k-1)*sizeof(stat[k])) ;
	  stat_nb-- ;
	  if ( stat_nb == 0 )
	    free(stat) ;
	}
    }
#endif

    

  return j ;
}


#include "zmw/zmw_widgets.h"
#include "zmw/http.h"

#endif
