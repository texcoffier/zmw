/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2003  Thierry EXCOFFIER, LIRIS

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
 * Cr�� le Sun Dec  2 20:46:42 2001 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_HOME_ZMW_ZMW_H
#define _HOME_EXCO_HOME_ZMW_ZMW_H

#define ZMW_USE_CACHE 1

#include <gdk/gdk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ZMW_BACKGROUND_NORMAL 0
#define ZMW_BACKGROUND_PUSHED 1
#define ZMW_BACKGROUND_POPED 2
#define ZMW_BORDER_LIGHT 3
#define ZMW_BORDER_DARK 4
#define ZMW_FOREGROUND 5

#define ZMW_VALUE_UNDEFINED (-32767)
#define ZMW_MAX_DEPTH 100
#define ZMW_MAX_NAME_LENGTH 1000

typedef struct
{
  int x, y, width, height ;
} Zmw_Rectangle ;

typedef char Zmw_Boolean ;

typedef struct
{
  char *name ;
  int index ;
  const char *why ;
} Zmw_Name ;

typedef struct
{
  Zmw_Rectangle min ;		/* Size needed for correct display */
  Zmw_Rectangle asked ;		/* Size asked by the user */
  Zmw_Rectangle required ;	/* It is needed overrided by asked */
  Zmw_Rectangle allocated ;	/* Size given by the parent */
  int index ;
  Zmw_Boolean horizontal_expand ;
  Zmw_Boolean vertical_expand ;
  Zmw_Boolean horizontal_alignment ;
  Zmw_Boolean vertical_alignment ;
  Zmw_Boolean used_to_compute_parent_size ;
  Zmw_Boolean event_in ;
  Zmw_Boolean invisible ;
  Zmw_Boolean sensible ;
} Zmw_Size ;

typedef enum
  {
    Zmw_Init,
    Zmw_Compute_Required_Size,
    Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing,
    Zmw_Compute_Children_Allocated_Size,
    Zmw_Post_Drawing,
    Zmw_Input_Event,
    Zmw_Search,
    Zmw_Accelerator,
    Zmw_Nothing,
    Zmw_Clean,			/* ZMW is shuting down, free resources */
  } Zmw_Subaction ;

enum { Zmw_False, Zmw_True } ;


#define Zmw_Debug_Window     1
#define Zmw_Debug_Draw_Cross 2
#define Zmw_Debug_Trace      4
#define Zmw_Debug_Event      8


typedef struct
{
  int (*action)() ;
  int debug ;
  int padding_width ;
  int border_width ;
  int focus_width ;
  Zmw_Name *focus ;
  GdkFont *font ;
  GdkWindow *window ;
  GdkGC *gc[6] ;
  Zmw_Boolean auto_resize ;
  Zmw_Boolean sensible ;
  Zmw_Boolean horizontal_expand ;
  Zmw_Boolean vertical_expand ;
  Zmw_Boolean horizontal_alignment ;
  Zmw_Boolean vertical_alignment ;
  int index ;
  /*
   * These value should not be inherited
   * But If it is not the case the "focus" is broken
   * Seems there is some bugs
   */
  Zmw_Boolean event_in_masked ;
  Zmw_Boolean event_in_focus ;
  Zmw_Boolean event_in ; /* rectangle or window created under */
} Zmw_Stackable_Inheritable ;

typedef struct
{
  Zmw_Size size ;
  int call_number ;		/* Number of call to zmw_begin */
  int nb_of_children, nb_of_children_max, nb_of_children_0 ;
  Zmw_Size *children ;
  Zmw_Boolean gc_copy_on_write[6] ;
  Zmw_Boolean font_copy_on_write ;
  char *name ;			/* Pointer on the last part of full_name */
  char *name_index ;		/* Pointer on the '.#' part of name */
  int name_separator ;		/* In case of ambiguity of the name */
  const char *type ;
  enum { Zmw_External_No, Zmw_External_Stop, Zmw_External_Continue } external_state ;
  Zmw_Boolean do_not_execute_pop ;
  Zmw_Subaction subaction ;	/* The action to process in the widget */
} Zmw_Stackable_Uninheritable ;

typedef struct
{
  Zmw_Stackable_Inheritable i ;
  Zmw_Stackable_Uninheritable u ;
} Zmw_State ;

typedef struct
{
  /*
   * Window list.
   * This window list is created by "zmw_window".
   * The windows in the GDK list and not in this one must be unmapped.
   * See zmw_run.c/zmw_draw
   */
  int nb_windows ;
  struct
  {
    GdkWindow **window ;
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
  int x, y ;			/* Pointer position */
  Zmw_Boolean activated ;	/* True if the last widget was activated */
  Zmw_Boolean dragged ;		/* True if being dragged */
  struct timeval last_cursor_move ; /* To use for tips */
  Zmw_Name tip_displayed ;	/* Name of the widget with a tip displayed */
  Zmw_Name found ;		/* Widget found by "Zmw_Search"*/
  int need_repaint ;

  Zmw_Name widget_to_trace ;

  char full_name[ZMW_MAX_NAME_LENGTH] ;
} Zmw ;

extern Zmw zmw ;

enum Zmw_Drag
  { Zmw_Drag_No
    , Zmw_Drag_Begin
    , Zmw_Drag_Accepted
    , Zmw_Drag_Refused
    , Zmw_Drag_Running
    , Zmw_Drag_Ending
  } ;




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
#define ZMW_GC                        (zMw->i.gc                        )
#define ZMW_FONT                      (zMw->i.font                      )
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
#define ZMW_NB_CHILDREN               (zMw->u.nb_children               )
#define ZMW_CALL_NUMBER               (zMw->u.call_number               )
#define ZMW_NAME                      (zMw->u.name                      )
#define ZMW_NAME_INDEX                (zMw->u.name_index                )
#define ZMW_NAME_SEPARATOR            (zMw->u.name_separator            )
#define ZMW_SIZE_INDEX                (zMw->u.size.index                )
#define ZMW_SIZE_HORIZONTAL_EXPAND    (zMw->u.size.horizontal_expand    )
#define ZMW_SIZE_HORIZONTAL_ALIGNMENT (zMw->u.size.horizontal_alignment )
#define ZMW_SIZE_VERTICAL_EXPAND      (zMw->u.size.vertical_expand      )
#define ZMW_SIZE_VERTICAL_ALIGNMENT   (zMw->u.size.vertical_alignment   )
#define ZMW_NB_OF_CHILDREN            (zMw->u.nb_of_children            )
#define ZMW_CHILDREN                  (zMw->u.children                  )
#define ZMW_GC_COPY_ON_WRITE          (zMw->u.gc_copy_on_write          )
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
void zmw_font_free() ;
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


#define zmw_modify_gc(X,I)			\
do						\
{						\
  if ( ZMW_GC_COPY_ON_WRITE[I] )		\
    {						\
      ZMW_GC_COPY_ON_WRITE[I] = 0 ;		\
      ZMW_GC[I] = gdk_gc_new(ZMW_WINDOW) ;	\
    }						\
  X						\
}						\
while(0)

#define zmw_modify_gc_color(I,X,P)					   \
zmw_modify_gc(								   \
		 if ( ZMW_ACTION == zmw_action_draw )                      \
                     {							   \
		       GdkColor __color__ ;				   \
		       __color__.pixel = P ;				   \
		       X(ZMW_GC[I], &__color__) ;			   \
		     }							   \
                 , I							   \
		 )

#define zmw_foreground(T,P) zmw_modify_gc_color(T,gdk_gc_set_foreground,P)
#define zmw_background(T,P) zmw_modify_gc_color(T,gdk_gc_set_background,P)



#define ZMW(TYPE) for(ZMW_TYPE=#TYPE,zmw_init_widget(); \
                      (zmw.debug&Zmw_Debug_Trace)?zmw_debug_trace():0,(TYPE),(*ZMW_ACTION)() ; \
                      zmw_state_pop())

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

#define ZMW_REALLOC(P,N) P = realloc(P, (N) * sizeof(*(P)))
#define ZMW_FREE(P) ZMW1( free(P) ; (P)=NULL ; )
#define ZMW_TABLE_SIZE(T) (sizeof(T)/sizeof((T)[0]))
#define ZMW_HERE zmw_printf("%s:%s:%d %s\n", __FILE__, __FUNCTION__, __LINE__, zmw_action_name()) 
#define ZMW_ABORT do { ZMW_HERE ; abort() ; } while(0)

#define ZMW_MIN(A,B) ((A)<(B)?(A):(B))
#define ZMW_MAX(A,B) ((A)<(B)?(B):(A))


#define zmw_drawing() (ZMW_ACTION == zmw_action_draw && ZMW_CALL_NUMBER == 3)

/*
 * zmw_run.c
 */
void zmw_run(gint *argc, gchar ***argv, void (*fct)()) ;
void zmw_exit(int r) ;
void zmw_do_nothing() ;
void zmw_need_repaint() ;
void zmw_draw(void (*fct)()) ;
void zmw_call_widget(void (*fct)(), int (*action)()) ;
/*
 * zmw.c
 */
int zmw_printf(const char *format, ...) ;
const char *zmw_action_name() ;
void zmw_name(const char *s) ;
void zmw_increment_index() ;
void zmw_decrement_index() ;
void zmw_state_pop() ;
void zmw_state_push() ;
void zmw_init_widget() ;
int zmw_action_compute_required_size() ;
int zmw_action_draw() ;
int zmw_action_dispatch_event() ;
int zmw_action_search() ;
int zmw_action_dispatch_accelerator() ;
int zmw_action_first_pass() ;
int zmw_action_clean() ;
void zmw_action_second_pass() ;
void zmw_action_do_not_enter() ;
void zmw_debug_trace() ;
/*
 * zmw_name.c
 */
// const char* zmw_name_full(Zmw_State *ws) ;
// const char* zmw_name_local(Zmw_State *ws) ;
char * zmw_name_copy() ;
void zmw_name_init() ;
void zmw_name_free() ;
void zmw_name_update() ;
void zmw_name_register(Zmw_Name *n) ;
void zmw_name_unregister(Zmw_Name *n) ;
void zmw_name_dump(FILE *f) ;
Zmw_Boolean zmw_name_is(const Zmw_Name *n) ;
Zmw_Boolean zmw_name_contains(const Zmw_Name *n) ;
Zmw_Boolean zmw_name_next_contains(const Zmw_Name *n) ;
#define zmw_name_registered(N) ( (N)->name )

#define ZMW_NAME_UNREGISTERED(N) {NULL, 0, N}

/*
 * zmw_size.c
 */
/* void zmw_compute_box_size(int allocated) ; */
void zmw_compute_no_size() ;
void zmw_asked_size_set_required_size() ;
Zmw_Rectangle zmw_rectangle_max(Zmw_Rectangle *a, Zmw_Rectangle *b) ;
void zmw_swap_x_y() ;
/*
 * zmw_border.c
 */
#define Zmw_Border_Draw_Focus  1
#define Zmw_Border_Focusable   2
#define Zmw_Border_Solid       4
#define Zmw_Border_Embossed    8
#define Zmw_Border_Relief      16
#define Zmw_Border_Background  32
#define Zmw_Border_In          64
#define Zmw_Border_Relief_In   (Zmw_Border_Relief | Zmw_Border_In)
#define Zmw_Border_Relief_Out   Zmw_Border_Relief
#define Zmw_Border_Embossed_In (Zmw_Border_Embossed | Zmw_Border_In)
#define Zmw_Border_Embossed_Out Zmw_Border_Embossed
void zmw_border_draw(int options) ;
void zmw_border_solid_draw() ;
void zmw_border_relief_in_draw() ;
void zmw_border_relief_out_draw() ;
void zmw_border_embossed_in_draw() ;
void zmw_border_embossed_out_draw() ;
void zmw_cross_draw() ;
/*
 * zmw_event.c
 */
Zmw_Boolean zmw_activated() ;
Zmw_Boolean zmw_dragged() ;
Zmw_Boolean zmw_button_pressed() ;
Zmw_Boolean zmw_button_released() ;
Zmw_Boolean zmw_button_released_anywhere() ;
Zmw_Boolean zmw_focus_in() ;
Zmw_Boolean zmw_key_pressed() ;
Zmw_Boolean zmw_key_string() ;
Zmw_Boolean zmw_selected() ;
Zmw_Boolean zmw_accelerator(GdkModifierType state, int character) ;
Zmw_Boolean zmw_focused() ;
Zmw_Boolean zmw_tip_visible() ;
void zmw_event_remove() ;
Zmw_Boolean zmw_window_is_popped(int detached) ;
void zmw_window_unpop_all() ;
void zmw_widget_is_tested() ;

void zmw_focusable() ;
void zmw_activable() ;

void zmw_event_button_release() ;
Zmw_Boolean zmw_event_to_process() ;

void zmw_event_debug_window() ;

/*
 * zmw_drag.c
 */
enum Zmw_Drag zmw_drag_from_state() ;
void zmw_drag_from_data_set(const char *data) ;
enum Zmw_Drag  zmw_drag_to_state() ;
void zmw_drag_to_accept(int bool) ;
char *zmw_drag_to_data_get() ;
void zmw_drag_cancel() ;
void zmw_drag_clear_to_widget() ;
void zmw_drag_debug(FILE *f) ;
void zmw_drag_debug_window() ;
/*
 * zmw_cache.c
 */
#if ZMW_USE_CACHE
void zmw_cache_init(int size) ;
int zmw_cache_get_size(Zmw_Size *r) ;
void zmw_cache_set_size(const Zmw_Size *r) ;
#else
#define zmw_cache_init(S)
#define zmw_cache_get_size(R) 1
#define zmw_cache_set_size(R)
#endif



#include "../widgets/zmw_widgets.h"

#endif
