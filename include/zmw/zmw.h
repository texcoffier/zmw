/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2004 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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
  } Zmw_Subaction ;

typedef enum {
  Zmw_Font_Style_Normal,
  Zmw_Font_Style_Italic,
} Zmw_Font_Style ;

#define ZMW_VALUE_UNDEFINED (-32767)
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

#define ZMW_LEFT -1
#define ZMW_CENTER 0
#define ZMW_RIGHT 1
#define ZMW_TOP -1
#define ZMW_BOTTOM 1

typedef char Zmw_Boolean ;
enum zmw_boolean { Zmw_False, Zmw_True } ;

typedef float Zmw_Float_0_1 ; /* float number between 0 and 1 included */

typedef unsigned int Zmw_Hash ;

typedef struct zmw_rectangle
{
  int x, y, width, height ;
} Zmw_Rectangle ;


typedef struct zmw_name
{
  char *name ;
  const char *why ;
  void *value ;
  Zmw_Hash hash ;
  Zmw_Name_Type type ;
  struct zmw_name *next ;	// Collision in the hash table
} Zmw_Name ;

typedef struct zmw_size
{
  Zmw_Rectangle min ;		// Size needed for correct display
  Zmw_Rectangle required ;	// It is : needed overrided by asked
  Zmw_Rectangle allocated ;   // Size usable (_padded less padding)
  Zmw_Hash hash ;
  Zmw_Boolean used_to_compute_parent_size ;
  Zmw_Boolean event_in_rectangle ;
  Zmw_Boolean event_in_children ;
  Zmw_Boolean invisible ;
  Zmw_Boolean sensible ;
  Zmw_Boolean focused ;
  Zmw_Boolean do_not_map_window ; /* for menu containing detached window */
  Zmw_Boolean activated ;
  Zmw_Boolean child_activated ; /* True if a child was activated */
  Zmw_Boolean changed ;	        /* True if the widget has changed */
  Zmw_Boolean tip_visible ;	/* True if it is a displayed tip */
  Zmw_Boolean horizontal_expand ;
  Zmw_Boolean vertical_expand ;
  /*
   * If true, the widget is transparent for size, sensible, activate, ...
   * requests. Because it is not this widget we want to test
   * but the previous one.
   * It is used for "tip" for example.
   */
  Zmw_Boolean pass_through ;
  /*
   * These values are stored for each widgets in the child list.
   * But A subset of these values are the current state that
   * must be passed to the next sibling or to the first child.
   */
  struct
  {
    Zmw_Boolean horizontal_expand ;
    Zmw_Boolean vertical_expand ;
    char horizontal_alignment ;
    char vertical_alignment ;
    int padding_width ;
  } current_state ;
} Zmw_Size ;

typedef struct zmw_font_description
{
  char *family ;
  int weight ; // 0 .. 1000
  int size ; // in point
  Zmw_Font_Style style ;
} Zmw_Font_Description ;

typedef struct zmw_stackable_inheritable
{
  int (*action)(void) ;
  int debug ;
  int border_width ;
  int focus_width ;
  Zmw_Name *focus ;
  int colors[Zmw_Color_Last] ;
  Zmw_Font_Description font ;
  Zmw_Boolean auto_resize ;
  Zmw_Boolean sensible ;
} Zmw_Stackable_Inheritable ;

typedef struct zmw_stackable_uninheritable
{
  int call_number ;		/* Number of call to zmw_begin */
  int nb_of_children, nb_of_children_max ;
  Zmw_Size *children ;
  Zmw_Boolean do_not_execute_pop ;
  Zmw_Rectangle asked ;		// Size asked by the user
  char *name ;			/* Pointer on the last part of full_name */
  char *name_index ;		/* Pointer on the '.#' part of name */
  int name_separator ;		/* In case of ambiguity of the name */
  const char *type ;		/* Content of the ZMW(....) call */
  const char *file ;		/* File where ZMW was declared */
  int line ;			/* Line where ZMW was declared */
  enum { Zmw_External_No, Zmw_External_Stop, Zmw_External_Continue } external_state ;
  Zmw_Subaction subaction ;	/* The action to process in the widget */
  int *menu_state ;		/* For zmw_window_is_popped_with_detached */
  int child_number ;            /* 0 if the first child, 1 the second, ... */
  Zmw_Size *size ;		/* Fast access to size (-10% lib size) */
  /*
   * These values are inherited from parent to child.
   * But each time a widget is initialised, the value
   * is retrieved from the parent.
   * So there is no propagation of value change between siblings 
   */
  struct
  {
    GdkWindow **window ;
    GdkGC *gc ;
    Zmw_Rectangle clipping ;
  } parent_to_child ;
} Zmw_Stackable_Uninheritable ;

typedef struct zmw_state
{
  Zmw_Stackable_Inheritable i ;
  Zmw_Stackable_Uninheritable u ;
} Zmw_State ;

struct zmw_run ;

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
  int zmw_table_depth ;
  Zmw_State *zmw_table ;

  /*
   * These values apply to the widget in which we are
   */
  char full_name[ZMW_MAX_NAME_LENGTH] ; // The widget names are computed here
  /*
   * These values apply to the next widget and the other after that
   */
  int debug ;			        // Library Debug flags Zmw_Debug_...
  /*
   * These values apply only once and are reseted
   */
  Zmw_Boolean external_do_not_make_init ; // For EXTERNAL
  /*
   * These values apply to the last widget (can't be used before first kid)
   */
  /*
   * Theses values are set by zmw_run.c and should not be modified
   */ 
  GdkEvent *event ;		/* Current event, erased when received */
  Zmw_Boolean event_removed ;   /* If true the event had been processed */
  int x, y, x_root, y_root ;	/* Pointer position */
  GdkWindow *window ;           /* Window of the last event received */
  GdkEvent event_key ;          /* The pressed key */
  Zmw_Boolean key_pressed ;     /* True if a key is pressed */
  Zmw_Boolean button_pressed ;  /* True if a button is pressed */
  Zmw_Boolean still_yet_displayed ;
  Zmw_Boolean tips_yet_displayed ;
  /*
   * Theses values are set while traversing the widgets tree
   */
  Zmw_Name found ;		/* Widget found by "Zmw_Search"*/
  Zmw_Name *focus ;             /* Focus group with the cursor (Zmw_Search) */
  Zmw_Name tip_displayed ;	/* Name of the widget with a tip displayed */
  Zmw_Name widget_to_trace ;
#if ZMW_DEBUG_INSIDE_ZMW_PARAMETER
  Zmw_Boolean inside_zmw_parameter ;
#endif
  /*
   * Theses values are private to "zmw_run.c"
   */
  struct zmw_run *run ;         /* Hidden struct, private to zmw_run.c */
} Zmw ;

extern Zmw zmw ;

#define zMw zmw.ptr

#define ZMW_ACTION                    	(zMw->i.action                       )
#define ZMW_DEBUG                     	(zMw->i.debug                        )
#define ZMW_BORDER_WIDTH              	(zMw->i.border_width                 )
#define ZMW_FOCUS_WIDTH               	(zMw->i.focus_width                  )
#define ZMW_AUTO_RESIZE               	(zMw->i.auto_resize                  )
#define ZMW_SENSIBLE                  	(zMw->i.sensible                     )
#define ZMW_FOCUS                     	(zMw->i.focus                        )
#define ZMW_EVENT                     	(zMw->i.event                        )
#define ZMW_COLORS                    	(zMw->i.colors                       )
#define ZMW_FONT               	        (zMw->i.font                         )
#define ZMW_FONT_FAMILY               	(zMw->i.font.family                  )
#define ZMW_FONT_SIZE               	(zMw->i.font.size                    )
#define ZMW_FONT_WEIGHT               	(zMw->i.font.weight                  )
#define ZMW_FONT_STYLE               	(zMw->i.font.style                   )

#define ZMW_CALL_NUMBER               	(zMw->u.call_number                  )
#define ZMW_NAME                      	(zMw->u.name                         )
#define ZMW_NAME_INDEX                	(zMw->u.name_index                   )
#define ZMW_NAME_SEPARATOR            	(zMw->u.name_separator               )
#define ZMW_NB_OF_CHILDREN            	(zMw->u.nb_of_children               )
#define ZMW_CHILDREN                  	(zMw->u.children                     )
#define ZMW_DO_NOT_EXECUTE_POP        	(zMw->u.do_not_execute_pop           )
#define ZMW_EXTERNAL_STATE            	(zMw->u.external_state               )
#define ZMW_ASKED                     	(zMw->u.asked                        )
#define ZMW_TYPE                      	(zMw->u.type                         )
#define ZMW_FILE                      	(zMw->u.file                         )
#define ZMW_LINE                      	(zMw->u.line                         )
#define ZMW_SUBACTION                 	(zMw->u.subaction                    )
#define ZMW_MENU_STATE                	(zMw->u.menu_state                   )
#define ZMW_CHILD_NUMBER              	(zMw->u.child_number                 )
#define ZMW_WINDOW                    	(zMw->u.parent_to_child.window       )
#define ZMW_GC                        	(zMw->u.parent_to_child.gc           )
#define ZMW_CLIPPING                  	(zMw->u.parent_to_child.clipping     )

#define ZMW_SIZE_OF(Z)  ((Z)[-1].u.children[(Z)->u.child_number])
#define ZMW_SIZE                      	(*zMw->u.size)
#define ZMW_SIZE_CS                   	(ZMW_SIZE.current_state              )

#define ZMW_SIZE_INVISIBLE            	(ZMW_SIZE.invisible                  )
#define ZMW_SIZE_SENSIBLE             	(ZMW_SIZE.sensible                   )
#define ZMW_SIZE_PASS_THROUGH         	(ZMW_SIZE.pass_through               )
#define ZMW_SIZE_EVENT_IN_RECTANGLE   	(ZMW_SIZE.event_in_rectangle         )
#define ZMW_SIZE_EVENT_IN_CHILDREN    	(ZMW_SIZE.event_in_children          )
#define ZMW_SIZE_ALLOCATED            	(ZMW_SIZE.allocated                  )
#define ZMW_SIZE_REQUIRED             	(ZMW_SIZE.required                   )
#define ZMW_SIZE_MIN                  	(ZMW_SIZE.min                        )
#define ZMW_SIZE_FOCUSED              	(ZMW_SIZE.focused                    )
#define ZMW_SIZE_ACTIVATED            	(ZMW_SIZE.activated                  )
#define ZMW_SIZE_CHILD_ACTIVATED      	(ZMW_SIZE.child_activated            )
#define ZMW_SIZE_CHANGED              	(ZMW_SIZE.changed                    )
#define ZMW_SIZE_TIP_VISIBLE           	(ZMW_SIZE.tip_visible                )
#define ZMW_SIZE_HASH                	(ZMW_SIZE.hash                       )
#define ZMW_SIZE_DO_NOT_MAP_WINDOW    	(ZMW_SIZE.do_not_map_window          )
#define ZMW_USED_TO_COMPUTE_PARENT_SIZE (ZMW_SIZE.used_to_compute_parent_size)
#define ZMW_SIZE_HORIZONTAL_EXPAND    	(ZMW_SIZE.horizontal_expand          )
#define ZMW_SIZE_VERTICAL_EXPAND      	(ZMW_SIZE.vertical_expand            )
#define ZMW_HORIZONTAL_EXPAND         	(ZMW_SIZE_CS.horizontal_expand       )
#define ZMW_SIZE_HORIZONTAL_ALIGNMENT 	(ZMW_SIZE_CS.horizontal_alignment    )
#define ZMW_VERTICAL_EXPAND           	(ZMW_SIZE_CS.vertical_expand         )
#define ZMW_SIZE_VERTICAL_ALIGNMENT   	(ZMW_SIZE_CS.vertical_alignment      )
#define ZMW_SIZE_PADDING_WIDTH        	(ZMW_SIZE_CS.padding_width           )

#define ZMW_CHILD_REQUIRED_PADDED_WIDTH(I) (ZMW_CHILDREN[I].required.width + 2*ZMW_CHILDREN[I].current_state.padding_width)
#define ZMW_CHILD_REQUIRED_PADDED_HEIGHT(I) (ZMW_CHILDREN[I].required.height + 2*ZMW_CHILDREN[I].current_state.padding_width)
#define ZMW_CHILD_REQUIRED_PADDED_RIGHT(I) (ZMW_CHILDREN[I].required.x + ZMW_CHILDREN[I].required.width + 2*ZMW_CHILDREN[I].current_state.padding_width)
#define ZMW_CHILD_REQUIRED_PADDED_LEFT(I) (ZMW_CHILDREN[I].required.x + ZMW_CHILDREN[I].current_state.padding_width)
#define ZMW_CHILD_REQUIRED_PADDED_TOP(I) (ZMW_CHILDREN[I].required.y + ZMW_CHILDREN[I].required.height + 2*ZMW_CHILDREN[I].current_state.padding_width)


#define ZMW_PARENT_SIZE  ZMW_SIZE_OF(zMw-1)
#define ZMW_WIDGET_TOP (zMw <= &zmw.zmw_table[1])

#define ZMW1(X) do { X } while(0)


#define ZMW_SIZE_CURRENT_STATE (zMw[-1].u.children[ZMW_CHILD_NUMBER+1].current_state)

#define                zmw_width(X) ZMW_ASKED.width      = X
#define               zmw_height(X) ZMW_ASKED.height     = X
#define                    zmw_x(X) ZMW_ASKED.x          = X
#define                    zmw_y(X) ZMW_ASKED.y          = X
#define                zmw_debug(D) ZMW_DEBUG                 = D
#define    zmw_horizontal_expand(B) ZMW_SIZE_CURRENT_STATE.horizontal_expand     = B
#define      zmw_vertical_expand(B) ZMW_SIZE_CURRENT_STATE.vertical_expand       = B
#define zmw_horizontal_alignment(B) ZMW_SIZE_CURRENT_STATE.horizontal_alignment  = B
#define   zmw_vertical_alignment(B) ZMW_SIZE_CURRENT_STATE.vertical_alignment    = B
#define        zmw_padding_width(B) ZMW_SIZE_CURRENT_STATE.padding_width         = B
#define         zmw_border_width(B) ZMW_BORDER_WIDTH          = B
#define          zmw_focus_width(B) ZMW_FOCUS_WIDTH           = B
#define          zmw_auto_resize(B) ZMW_AUTO_RESIZE           = B
#define             zmw_sensible(B) ZMW_SENSIBLE              = B

#define  zmw_focused() ZMW_SIZE_FOCUSED

#define zmw_name_full (zmw.full_name)


#if ZMW_DEBUG_INSIDE_ZMW_PARAMETER
#define ZMW_GO_INSIDE   zmw.inside_zmw_parameter = Zmw_True,
#define ZMW_GO_OUTSIDE  zmw.inside_zmw_parameter = Zmw_False,
#define ZMW_FUNCTION_CALLED_OUTSIDE_ZMW_PARAMETER if ( zmw.inside_zmw_parameter) ZMW_ABORT
#define ZMW_FUNCTION_CALLED_INSIDE_ZMW_PARAMETER if ( !zmw.inside_zmw_parameter) ZMW_ABORT
#else
#define ZMW_GO_INSIDE
#define ZMW_GO_OUTSIDE
#define ZMW_FUNCTION_CALLED_OUTSIDE_ZMW_PARAMETER
#define ZMW_FUNCTION_CALLED_INSIDE_ZMW_PARAMETER
#endif

#if ZMW_DEBUG_STORE_WIDGET_TYPE
#define ZMW_STORE_WIDGET_TYPE(TYPE) ZMW_TYPE=#TYPE,ZMW_FILE=__FILE__,ZMW_LINE=__LINE__,
#else
#define ZMW_STORE_WIDGET_TYPE(TYPE)
#endif

#define ZMW(TYPE) for(ZMW_STORE_WIDGET_TYPE(TYPE) zmw_init_widget(),zmw_debug_in_zmw_loop(-1); \
                      ZMW_GO_INSIDE zmw_debug_in_zmw_loop(0),(TYPE),zmw_debug_in_zmw_loop(1),ZMW_GO_OUTSIDE (*ZMW_ACTION)() ; \
                      zmw_state_pop())

/*
 * See zmw_viewport.c for example of use
 */

#define ZMW_EXTERNAL_RESTART  if ( ZMW_CALL_NUMBER ) goto restart ; zmw.external_do_not_make_init = Zmw_True


#define ZMW_EXTERNAL_RETURNS(X) { ZMW_EXTERNAL_STATE = Zmw_External_Continue ; return X ; restart: ZMW_DO_NOT_EXECUTE_POP = Zmw_False ; }

#define ZMW_EXTERNAL ZMW_EXTERNAL_RETURNS( )

#define ZMW_EXTERNAL_STOP  ZMW_EXTERNAL_STATE = Zmw_External_Stop ; zmw.external_do_not_make_init = Zmw_False ;


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


/* We do not test ZMW_SUBACTION because :
 * - It doesn't work if tested before the first widget container
 * - It doesn't work if tested after a zmw_if(0)
 */
#define zmw_drawing() ( zMw[-1].u.subaction == Zmw_Post_Drawing )
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
void zmw_need_dispatch(void) ;
void zmw_draw(void (*fct)(void)) ;
void zmw_call_widget(void (*fct)(void), int (*action)(void)) ;
void zmw_use_window_from_button_press(Zmw_Boolean b) ;
Zmw_Boolean zmw_use_window_from_button_press_get() ;
/*
 * zmw.c
 */
int zmw_printf(const char *format, ...) ;
const char *zmw_action_name(void) ;
const char *zmw_action_name_fct(void) ;
void zmw_name(const char *s) ;
void zmw_font_family(const char *s) ;
#define zmw_font_size(s) ZMW_FONT_SIZE = s
#define zmw_font_weight(w) ZMW_FONT_WEIGHT = w
#define zmw_font_style(s) ZMW_FONT_STYLE = s
#define zmw_color(T,P) ZMW_COLORS[T] = P
void zmw_state_init(Zmw_State *s) ; /* Used only by zmw_run.c */
void zmw_state_pop(void) ;
Zmw_Hash zmw_hash(Zmw_Hash, const char*) ;
Zmw_Size* zmw_widget_previous_size(void) ;
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
void zmw_debug_trace(void) ;
void zmw_stack_print(void) ;
int zmw_event_in(void) ;
void zmw_focus(Zmw_Name *focus) ;


static inline int zmw_debug_in_zmw_loop(int i)
{
  if ( i==0 && (zmw.debug & Zmw_Debug_Trace) )
    zmw_debug_trace() ;
  return 0 ;
}



/*
 * zmw_name.c
 */
void zmw_name_free(void) ;
void zmw_name_debug_window(void) ;
void zmw_name_register(Zmw_Name *n) ;
void zmw_name_register_with_name(Zmw_Name *n, const char *name) ;
void zmw_name_unregister(Zmw_Name *n) ;
void zmw_name_unregister_value(const char *name, const char *why) ;
void zmw_name_unregister_value_by_pointer(const char *why, void *p) ;
Zmw_Boolean zmw_name_get_value_pointer(const char *why, void **value);
Zmw_Boolean zmw_name_get_value_int(const char *why, int *value) ;
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


#define zmw_name_registered(N) ( (N)->name )

#define ZMW_NAME_UNREGISTERED(N) {NULL, N}

/*
 * zmw_size.c
 */
/* void zmw_compute_box_size(int allocated) ; */
Zmw_Rectangle zmw_rectangle_max(Zmw_Rectangle *a, Zmw_Rectangle *b) ;
void zmw_rectangle_void(Zmw_Rectangle *a) ;
void zmw_swap_x_y(void) ;
void zmw_padding_add(Zmw_Rectangle *r, int padding) ;
void zmw_padding_remove(Zmw_Rectangle *r, int padding) ;

/*
 * zmw_device.c
 */
int zmw_rgb_to_int(Zmw_Float_0_1 r, Zmw_Float_0_1 g, Zmw_Float_0_1 b) ;
void zmw_int_to_rgb(int c,Zmw_Float_0_1 *r, Zmw_Float_0_1 *g, Zmw_Float_0_1*b);
void zmw_draw_line(Zmw_Color c, int x1, int y1, int x2, int y2) ;
void zmw_draw_rectangle(Zmw_Color c, Zmw_Boolean filled
			, int x, int y, int width, int height) ;
void zmw_pixbuf_render_to_drawable(GdkPixbuf *pb, int x, int y) ;

void zmw_draw_clip_push_inside(const Zmw_Rectangle *r) ;
void zmw_draw_clip_pop() ;
#define zmw_foreground(R,G,B) zmw_color(Zmw_Color_Foreground, zmw_rgb_to_int(R,G,B))
#define zmw_background(R,G,B) zmw_color(Zmw_Color_Background_Normal, zmw_rgb_to_int(R,G,B))
void zmw_text_init() ;
void zmw_text_set_text(const char *text, const Zmw_Font_Description *fd) ;
void zmw_text_render(Zmw_Color c, int xx, int yy) ;
void zmw_text_get_size(int *width, int *height) ;
int zmw_text_xy_to_index(int x, int y) ;
void zmw_text_get_grapheme_rectangle(int index, Zmw_Rectangle *rect) ;
void zmw_text_cursor_position(int cursor_pos, Zmw_Rectangle *r) ;
int zmw_text_next_char(const char *text, int cursor) ;
int zmw_text_previous_char(const char *text, int cursor) ;
int zmw_text_delete_char(char *text, int cursor) ;

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

#define Zmw_Menu_Is_Detached         1        
#define Zmw_Menu_Is_Poped            2           
#define Zmw_Menu_Contains_A_Detached 4
#define Zmw_Menu_State_New           8        

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
Zmw_Boolean zmw_window_is_popped_with_detached(int *detached) ;
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
/*
 * zmw_cache.c
 */
void zmw_cache_init(int size) ;
void zmw_cache_free(void) ;
int zmw_cache_get_size(Zmw_Size *r) ;
void zmw_cache_set_size(const Zmw_Size *r) ;
int zmw_cache_size() ;


#include "zmw/zmw_widgets.h"
#include "zmw/http.h"

#endif
