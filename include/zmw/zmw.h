/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2003 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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

/*
 * Cr?? le Sun Dec  2 20:46:42 2001 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_HOME_ZMW_ZMW_H
#define _HOME_EXCO_HOME_ZMW_ZMW_H

#include <gdk/gdk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

#define ZMW_VALUE_UNDEFINED (-32767)
#define ZMW_MAX_DEPTH 100
#define ZMW_MAX_NAME_LENGTH 1000

typedef struct zmw_rectangle
{
  int x, y, width, height ;
} Zmw_Rectangle ;

typedef char Zmw_Boolean ;

typedef float Zmw_Float_0_1 ; /* float number between 0 and 1 included */

typedef enum zmw_name_type
	 { Zmw_Is_A_Registration
	 , Zmw_Is_A_Resource_String
	 , Zmw_Is_A_Resource_Int
	 , Zmw_Is_A_Resource_Pointer
	 } Zmw_Name_Type ;

typedef struct zmw_name
{
  char *name ;
  int index ;
  const char *why ;
  void *value ;
  Zmw_Name_Type type ;
} Zmw_Name ;

typedef struct
{
  char *name ;
  int initial_value ;
  char *widget_name ;
  int old_value ;
  int value ;
  Zmw_Boolean initialized ;
  Zmw_Name_Type type ;
  char *widget_name_used_for_get ;
} Zmw_Resource ;

typedef struct zmw_size
{
  Zmw_Rectangle min ;		/* Size needed for correct display */
  Zmw_Rectangle asked ;		/* Size asked by the user */
  Zmw_Rectangle required ;	/* It is needed overrided by asked */
  Zmw_Rectangle allocated ;	/* Size given by the parent */
  int index ;
  Zmw_Boolean horizontal_expand ;
  Zmw_Boolean vertical_expand ;
  char horizontal_alignment ;
  char vertical_alignment ;
  Zmw_Boolean used_to_compute_parent_size ;
  Zmw_Boolean event_in ;
  Zmw_Boolean invisible ;
  Zmw_Boolean sensible ;
} Zmw_Size ;

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
  } Zmw_Subaction ;

enum zmw_boolean { Zmw_False, Zmw_True } ;


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

typedef struct zmw_stackable_inheritable
{
  int (*action)(void) ;
  int debug ;
  int padding_width ;
  int border_width ;
  int focus_width ;
  Zmw_Name *focus ;
  int colors[Zmw_Color_Last] ;
  GdkFont *font ;
  GdkWindow *window ;
  GdkGC *gc ;
  Zmw_Rectangle clipping ;
  Zmw_Boolean auto_resize ;
  Zmw_Boolean sensible ;
  Zmw_Boolean horizontal_expand ;
  Zmw_Boolean vertical_expand ;
  Zmw_Boolean horizontal_alignment ;
  Zmw_Boolean vertical_alignment ;
  int index ;
  /*
   * These values should not be inherited
   * But If it is not the case the "focus" is broken
   * Seems there is some bugs
   */
  Zmw_Boolean event_in_masked ;
  Zmw_Boolean event_in_focus ;
  Zmw_Boolean event_in ; /* rectangle or window created under */
} Zmw_Stackable_Inheritable ;

typedef struct zmw_stackable_uninheritable
{
  Zmw_Size size ;
  int call_number ;		/* Number of call to zmw_begin */
  int nb_of_children, nb_of_children_max, nb_of_children_0 ;
  Zmw_Size *children ;
  Zmw_Boolean font_copy_on_write ;
  char *name ;			/* Pointer on the last part of full_name */
  char *name_index ;		/* Pointer on the '.#' part of name */
  int name_separator ;		/* In case of ambiguity of the name */
  int transient_separator ;     /* In case of multiple transient */
  const char *type ;
  enum { Zmw_External_No, Zmw_External_Stop, Zmw_External_Continue } external_state ;
  Zmw_Boolean do_not_execute_pop ;
  Zmw_Subaction subaction ;	/* The action to process in the widget */
} Zmw_Stackable_Uninheritable ;

typedef struct zmw_state
{
  Zmw_Stackable_Inheritable i ;
  Zmw_Stackable_Uninheritable u ;
} Zmw_State ;

typedef struct zmw
{
  /*
   * Window list.
   * This window list is created by "zmw_window".
   * The windows in the GDK list and not in this one must be unmapped.
   * See zmw_run.c/zmw_draw
   */
  int nb_windows ;
  struct zmw_windows
  {
    GdkWindow *window ;
  } *windows ;
  /*
   * Pointer on the current informations.
   * It is a pointer in zmw_table.
   */
  Zmw_State *ptr ;
  Zmw_State zmw_table[ZMW_MAX_DEPTH] ;

  int debug ;			/* Debug flags for the library Zmw_Debug_... */
  GdkEvent *event ;		/* Current event, erased when received */
  GdkEvent event_saved ;	/* Copy of the last event received */
  int x, y, x_root, y_root ;	/* Pointer position */
  Zmw_Boolean activated ;	/* True if the last widget was activated */
  Zmw_Boolean child_activated ; /* True if a child was activated */
  Zmw_Boolean changed ;	        /* True if the last widget has changed */
  Zmw_Boolean dragged ;		/* True if being dragged */
  Zmw_Boolean remove_event ;    /* Remove event before entering next widget */
  Zmw_Boolean next_is_transient;/* for tips, menu, ... */
  Zmw_Boolean key_pressed ;     /* True if a key is pressed */
  GdkEvent event_key ;          /* The pressed key */
  
  struct timeval last_cursor_move ; /* To use for shortcut popup */
  Zmw_Boolean still_yet_displayed ;
  int still_time ;              /* Time since last pointer motion */
  
  struct timeval last_user_action ; /* To use for tips */
  Zmw_Name tip_displayed ;	/* Name of the widget with a tip displayed */
  Zmw_Boolean tips_yet_displayed ;
  int idle_time ;               /* Time since last user interaction */

  Zmw_Name found ;		/* Widget found by "Zmw_Search"*/
  int need_repaint ;
  int need_dispatch ;           /* for drag and drop */
  int index_last ;		/* Last value used as widget index */

  Zmw_Name widget_to_trace ;

  char full_name[ZMW_MAX_NAME_LENGTH] ;
} Zmw ;

extern Zmw zmw ;

/* The ! in the comment indicates that you can change
 * the state of the program
 */
typedef enum zmw_drag_from
  {   Zmw_Drag_From_No          /* You are not dragged */
    , Zmw_Drag_From_Begin       /* ! Start of your drag, call zmw_drag_data_set() */
    , Zmw_Drag_From_End         /* ! End of your drag, call zmw_drag_is_accepted() */
    , Zmw_Drag_From_Running     /* You MUST display a drag window, you may call zmw_drag_is_accepted() */
  } Zmw_Drag_From ;

typedef enum zmw_drag_to
  { Zmw_Drag_To_No_Change     /* No state change */
    , Zmw_Drag_To_Enter       /* ! A drag is now over you, call zmw_drag_to_accept */
    , Zmw_Drag_To_Leave       /* ! A drag leave you */
    , Zmw_Drag_To_End         /* ! The drag is deposited on you */
  } Zmw_Drag_To ;




#define zMw zmw.ptr


#define ZMW_INDEX                     (zMw->i.index                     )
#define ZMW_WINDOW                    (zMw->i.window                    )
#define ZMW_ACTION                    (zMw->i.action                    )
#define ZMW_PADDING_WIDTH             (zMw->i.padding_width             )
#define ZMW_DEBUG                     (zMw->i.debug                     )
#define ZMW_BORDER_WIDTH              (zMw->i.border_width              )
#define ZMW_FOCUS_WIDTH               (zMw->i.focus_width               )
#define ZMW_AUTO_RESIZE               (zMw->i.auto_resize               )
#define ZMW_SENSIBLE                  (zMw->i.sensible                  )
#define ZMW_FOCUS                     (zMw->i.focus                     )
#define ZMW_EVENT_IN_FOCUS            (zMw->i.event_in_focus            )
#define ZMW_EVENT                     (zMw->i.event                     )
#define ZMW_EVENT_IN                  (zMw->i.event_in                  )
#define ZMW_FONT                      (zMw->i.font                      )
#define ZMW_COLORS                    (zMw->i.colors                    )
#define ZMW_GC                        (zMw->i.gc                        )
#define ZMW_CLIPPING                  (zMw->i.clipping                  )
#define ZMW_HORIZONTAL_EXPAND         (zMw->i.horizontal_expand         )
#define ZMW_HORIZONTAL_ALIGNMENT      (zMw->i.horizontal_alignment      )
#define ZMW_VERTICAL_EXPAND           (zMw->i.vertical_expand           )
#define ZMW_VERTICAL_ALIGNMENT        (zMw->i.vertical_alignment        )
#define ZMW_EVENT_IN_MASKED           (zMw->i.event_in_masked           )
#define ZMW_SIZE_ALLOCATED            (zMw->u.size.allocated            )
#define ZMW_SIZE_ASKED                (zMw->u.size.asked                )
#define ZMW_SIZE_REQUIRED             (zMw->u.size.required             )
#define ZMW_SIZE_MIN                  (zMw->u.size.min                  )
#define ZMW_SIZE                      (zMw->u.size                      )
#define ZMW_CALL_NUMBER               (zMw->u.call_number               )
#define ZMW_NAME                      (zMw->u.name                      )
#define ZMW_NAME_INDEX                (zMw->u.name_index                )
#define ZMW_NAME_SEPARATOR            (zMw->u.name_separator            )
#define ZMW_TRANSIENT_SEPARATOR       (zMw->u.transient_separator       )
#define ZMW_SIZE_INDEX                (zMw->u.size.index                )
#define ZMW_SIZE_HORIZONTAL_EXPAND    (zMw->u.size.horizontal_expand    )
#define ZMW_SIZE_HORIZONTAL_ALIGNMENT (zMw->u.size.horizontal_alignment )
#define ZMW_SIZE_VERTICAL_EXPAND      (zMw->u.size.vertical_expand      )
#define ZMW_SIZE_VERTICAL_ALIGNMENT   (zMw->u.size.vertical_alignment   )
#define ZMW_NB_OF_CHILDREN            (zMw->u.nb_of_children            )
#define ZMW_CHILDREN                  (zMw->u.children                  )
#define ZMW_FONT_COPY_ON_WRITE        (zMw->u.font_copy_on_write        )
#define ZMW_DO_NOT_EXECUTE_POP        (zMw->u.do_not_execute_pop        )
#define ZMW_EXTERNAL_STATE            (zMw->u.external_state            )
#define ZMW_INVISIBLE                 (zMw->u.size.invisible            )
#define ZMW_SIZE_SENSIBLE             (zMw->u.size.sensible             )
#define ZMW_SIZE_EVENT_IN             (zMw->u.size.event_in             )
#define ZMW_TYPE                      (zMw->u.type                      )
#define ZMW_SUBACTION                 (zMw->u.subaction                 )
#define ZMW_USED_TO_COMPUTE_PARENT_SIZE (zMw->u.size.used_to_compute_parent_size )



#define ZMW1(X) do { X } while(0)

GdkFont *zmw_font_load(const char *name) ;
void zmw_font_free(void) ;
/* Liberer les precedents comme pour le GC avec copy on write */
#define zmw_font(F) ZMW1(ZMW_FONT = zmw_font_load(F) ; ZMW_FONT_COPY_ON_WRITE = 0 ;)

#define                zmw_width(X) ZMW_SIZE_ASKED.width      = X
#define               zmw_height(X) ZMW_SIZE_ASKED.height     = X
#define                    zmw_x(X) ZMW_SIZE_ASKED.x          = X
#define                    zmw_y(X) ZMW_SIZE_ASKED.y          = X
#define                zmw_debug(D) ZMW_DEBUG                 = D
#define    zmw_horizontal_expand(B) ZMW_HORIZONTAL_EXPAND     = B
#define      zmw_vertical_expand(B) ZMW_VERTICAL_EXPAND       = B
#define zmw_horizontal_alignment(B) ZMW_HORIZONTAL_ALIGNMENT  = B
#define   zmw_vertical_alignment(B) ZMW_VERTICAL_ALIGNMENT    = B
#define        zmw_padding_width(B) ZMW_PADDING_WIDTH         = B
#define         zmw_border_width(B) ZMW_BORDER_WIDTH          = B
#define          zmw_focus_width(B) ZMW_FOCUS_WIDTH           = B
#define          zmw_auto_resize(B) ZMW_AUTO_RESIZE           = B
#define             zmw_sensible(B) ZMW_SENSIBLE              = B

#define   zmw_focus(X) ZMW1(ZMW_FOCUS = X; ZMW_EVENT_IN_FOCUS = ZMW_EVENT_IN ;)
#define zmw_name_full (zmw.full_name)

#define zmw_color(T,P) ZMW_COLORS[T] = P


#if ZMW_DEBUG_STORE_WIDGET_TYPE
#define ZMW(TYPE) for(ZMW_TYPE=#TYPE,zmw_init_widget(); \
                      (zmw.debug&Zmw_Debug_Trace)?zmw_debug_trace():0,(TYPE),(*ZMW_ACTION)() ; \
                      zmw_state_pop())
#else
#define ZMW(TYPE) for(zmw_init_widget(); \
                      (zmw.debug&Zmw_Debug_Trace)?zmw_debug_trace():0,(TYPE),(*ZMW_ACTION)() ; \
                      zmw_state_pop())

#endif

/*
 * See zmw_viewport.c for example of use
 */

#define ZMW_EXTERNAL_RESTART  if ( ZMW_CALL_NUMBER ) goto restart

#define ZMW_EXTERNAL  { ZMW_EXTERNAL_STATE = Zmw_External_Continue ; return ; restart: ZMW_DO_NOT_EXECUTE_POP = Zmw_False ; }

#define ZMW_EXTERNAL_STOP  ZMW_EXTERNAL_STATE = Zmw_External_Stop

/* It is a macro because it contains a return.... */
/* The children are not executed because they had been
   yet displayed by ZMW_EXTERNAL */
#define ZMW_EXTERNAL_HANDLING					\
  if ( ZMW_EXTERNAL_STATE != Zmw_External_No )			\
    {								\
      int zmw_i_ ;						\
      zmw_i_ = (ZMW_EXTERNAL_STATE == Zmw_External_Continue) ;	\
      ZMW_EXTERNAL_STATE = Zmw_External_No ;			\
      if ( zmw_i_ )						\
               ZMW_DO_NOT_EXECUTE_POP = Zmw_True ;		\
      return( zmw_i_  ) ;					\
    }


/*
 *
 */

#define ZMW_MALLOC(P,N) P = malloc((N) * sizeof(*(P)))
#define ZMW_REALLOC(P,N) P = realloc(P, (N) * sizeof(*(P)))
#define ZMW_FREE(P) ZMW1( free(P) ; (P)=NULL ; )
#define ZMW_TABLE_SIZE(T) (sizeof(T)/sizeof((T)[0]))
#define ZMW_HERE zmw_printf("%s:%s:%d %s\n", __FILE__, __FUNCTION__, __LINE__, zmw_action_name()) 
#define ZMW_ABORT do { ZMW_HERE ; abort() ; } while(0)

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


#define zmw_drawing() (ZMW_SUBACTION == Zmw_Post_Drawing)
#define zmw_debug_message() (ZMW_SUBACTION == Zmw_Debug_Message \
			&& (http_printf("<B><U>%s:%s:%d</U></B>", __FILE__, __FUNCTION__, __LINE__),1) )

/*
 * zmw_run.c
 */
void zmw_init(gint *argc, gchar ***argv) ;
void zmw_run(void (*fct)(void)) ;
void zmw_exit(int r) ;
void zmw_do_nothing(void) ;
void zmw_need_repaint(void) ;
void zmw_draw(void (*fct)(void)) ;
void zmw_call_widget(void (*fct)(void), int (*action)(void)) ;
/*
 * zmw.c
 */
int zmw_printf(const char *format, ...) ;
const char *zmw_action_name(void) ;
const char *zmw_action_name_fct(void) ;
void zmw_name(const char *s) ;
void zmw_name_of_the_transient_begin(void) ;
void zmw_name_of_the_transient_end(void) ;
void zmw_state_pop(void) ;
Zmw_Size* zmw_widget_previous_size(void) ;
void zmw_next_widget_could_be_transient(void) ;
void zmw_state_push(void) ;
void zmw_init_widget(void) ;
const char *zmw_action_name_(Zmw_Subaction sa) ;
const char* zmw_size_string(const Zmw_Size *s) ;
int zmw_action_compute_required_size(void) ;
int zmw_action_draw(void) ;
int zmw_action_dispatch_event(void) ;
int zmw_action_search(void) ;
int zmw_action_dispatch_accelerator(void) ;
int zmw_action_first_pass(void) ;
int zmw_action_clean(void) ;
void zmw_action_second_pass(void) ;
void zmw_action_do_not_enter(void) ;
void zmw_debug_trace(void) ;
/*
 * zmw_name.c
 */
// const char* zmw_name_full(Zmw_State *ws) ;
// const char* zmw_name_local(Zmw_State *ws) ;
char * zmw_name_copy(void) ;
void zmw_name_init(void) ;
void zmw_name_free(void) ;
void zmw_name_update(void) ;
void zmw_name_debug_window(void) ;
void zmw_name_register(Zmw_Name *n) ;
void zmw_name_register_with_name(Zmw_Name *n, const char *name) ;
void zmw_name_unregister(Zmw_Name *n) ;
void zmw_name_unregister_value_by_pointer(const char *why, void *p) ;
Zmw_Boolean zmw_name_get_value_pointer(const char *why, void **value);
Zmw_Boolean zmw_name_get_value_int(const char *why, int *value) ;
void zmw_name_set_value_pointer(const char *why, void *value);
void zmw_name_set_value_int(const char *why, int value) ;
Zmw_Boolean zmw_name_get_value_pointer_with_name(const char *name, const char *why, void **value);
Zmw_Boolean zmw_name_get_value_int_with_name(const char *name, const char *why, int *value) ;
void zmw_name_set_value_pointer_with_name(const char *name, const char *why, void *value);
void zmw_name_set_value_int_with_name(const char *name, const char *why, int value) ;
void zmw_name_dump(FILE *f) ;
Zmw_Boolean zmw_name_is(const Zmw_Name *n) ;
Zmw_Boolean zmw_name_contains(const Zmw_Name *n) ;
Zmw_Boolean zmw_name_is_inside(const Zmw_Name *n) ;
Zmw_Boolean zmw_name_next_contains(const Zmw_Name *n) ;

void zmw_resource_int_get(int **pointer_value, Zmw_Resource *r) ;
void zmw_resource_pointer_get(void **pointer_value, Zmw_Resource *r) ;
void zmw_resource_set(Zmw_Resource *r) ;

#define zmw_name_registered(N) ( (N)->name )

#define ZMW_NAME_UNREGISTERED(N) {NULL, 0, N}

/*
 * zmw_size.c
 */
/* void zmw_compute_box_size(int allocated) ; */
void zmw_compute_no_size(void) ;
void zmw_asked_size_set_required_size(void) ;
Zmw_Rectangle zmw_rectangle_max(Zmw_Rectangle *a, Zmw_Rectangle *b) ;
void zmw_swap_x_y(void) ;

/*
 * zmw_device.c
 */
void zmw_draw_line(Zmw_Color c, int x1, int y1, int x2, int y2) ;
void zmw_draw_rectangle(Zmw_Color c, Zmw_Boolean filled
			, int x, int y, int width, int height) ;
void zmw_draw_string(Zmw_Color c, int x, int y, const char *text) ;
void zmw_pixbuf_render_to_drawable(GdkPixbuf *pb, int x, int y) ;

void zmw_draw_clip_push_inside(const Zmw_Rectangle *r) ;
void zmw_draw_clip_pop() ;
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
void zmw_border_draw_with_rectangle(int options, Zmw_Rectangle *r) ;
void zmw_border_solid_draw(void) ;
void zmw_border_relief_in_draw(void) ;
void zmw_border_relief_out_draw(void) ;
void zmw_border_embossed_in_draw(void) ;
void zmw_border_embossed_out_draw(void) ;
void zmw_cross_draw(void) ;
void zmw_draw_clip_push(const Zmw_Rectangle *r) ;
void zmw_rgb(Zmw_Float_0_1 r, Zmw_Float_0_1 g, Zmw_Float_0_1 b) ;
/*
 * zmw_event.c
 */
Zmw_Boolean zmw_activated(void) ;
Zmw_Boolean zmw_changed(void) ;
Zmw_Boolean zmw_dragged(void) ;
Zmw_Boolean zmw_button_pressed(void) ;
Zmw_Boolean zmw_button_released(void) ;
Zmw_Boolean zmw_button_released_anywhere(void) ;
Zmw_Boolean zmw_focus_in(void) ;
Zmw_Boolean zmw_key_pressed(void) ;
Zmw_Boolean zmw_key_string(void) ;
Zmw_Boolean zmw_selected(void) ;
Zmw_Boolean zmw_selected_by_its_parents() ;
Zmw_Boolean zmw_selected_next_by_its_parents() ;
Zmw_Boolean zmw_accelerator(GdkModifierType state, int character) ;
Zmw_Boolean zmw_focused(void) ;
Zmw_Boolean zmw_focused_by_its_parents(void) ;
Zmw_Boolean zmw_cursor_enter(void) ;
Zmw_Boolean zmw_cursor_leave(void) ;
void zmw_cursor_set(char *name) ;
void zmw_focus_remove(void) ;
Zmw_Boolean zmw_tip_visible(void) ;
Zmw_Boolean zmw_cursor_in_and_pressed(void) ;
Zmw_Boolean zmw_cursor_in(void) ;
void zmw_event_remove(void) ;
Zmw_Boolean zmw_window_is_popped_with_detached(const int *detached) ;
Zmw_Boolean zmw_window_is_popped(void) ;
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
void zmw_event_dump() ;

/*
 * zmw_drag.c
 */
Zmw_Drag_From zmw_drag_from_state(void) ;
Zmw_Drag_To  zmw_drag_to_state(void) ;

Zmw_Boolean zmw_drag_accept_get(void) ;
void zmw_drag_accept_set(Zmw_Boolean bool) ;

void zmw_drag_data_set(const char *data) ;
char *zmw_drag_data_get(void) ;

void zmw_drag_to_nothing(void) ;
void zmw_drag_cancel(void) ;

Zmw_Boolean zmw_drag_swap(int p, int *order, int *current, int *old_current) ;

void zmw_drag_debug(FILE *f) ;
void zmw_drag_debug_window(void) ;
/*
 * zmw_cache.c
 */
void zmw_cache_init(int size) ;
void zmw_cache_free(void) ;
int zmw_cache_get_size(Zmw_Size *r) ;
void zmw_cache_set_size(const Zmw_Size *r) ;


#include "zmw/zmw_widgets.h"
#include "zmw/http.h"

#endif
