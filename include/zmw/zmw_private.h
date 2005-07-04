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

#ifndef _HOME_EXCO_HOME_WIDGET_INCLUDE_ZMW_ZMW_PRIVATE_H
#define _HOME_EXCO_HOME_WIDGET_INCLUDE_ZMW_ZMW_PRIVATE_H

#if ZMW_USE_BIT_RECORD
#define ZMW_BOOLEAN(X) unsigned int X:1
#else
#define ZMW_BOOLEAN(X) unsigned char X
#endif


struct zmw_name
{
  char *name ;
  const char *why ;
  void *value ;
  Zmw_Hash hash ;
  Zmw_Name_Type type ;
  struct zmw_name *next ;	// Collision in the hash table
} ;

#define zmw_name_registered(N) ( (N)->name )

/* Contains the part of the current state that is stored in
 * the children information because the parent need it
 * to compute allocated size.
 */
struct zmw_child_current_state
  {
    Zmw_Alignment horizontal_alignment ;
    Zmw_Alignment vertical_alignment ;
    Zmw_Width padding_width ; // Here because the container needs it
    ZMW_BOOLEAN(horizontal_expand) ;
    ZMW_BOOLEAN(vertical_expand) ;
} ;

/*
 * This structure contains:
 *   - Informations needed by the parent to computed children allocated size
 *   - And state information computed from previous loops
 * "expand" information is here twice (not nice at all):
 *   - The current expand state asked by the user
 *   - The expand state that apply to the widget.
 *     Because if the user ask a fixed width, the widget can't expand.
 *     (see zmw.c "zmw_action_compute_required_size")
 */
struct zmw_child
{
  Zmw_Child_Current_State current_state ;
  Zmw_Hash hash_key ;             // Key of the full widget name
  Zmw_Rectangle min ;		// Size needed for correct display
  Zmw_Rectangle required ;	// It is : needed overrided by asked
  Zmw_Rectangle allocated ;   // Size usable (_padded less padding)
  ZMW_BOOLEAN(used_by_parent) ;
  ZMW_BOOLEAN(event_in_rectangle) ;
  ZMW_BOOLEAN(event_in_children) ;
  ZMW_BOOLEAN(invisible) ;
  ZMW_BOOLEAN(sensitived) ;
  ZMW_BOOLEAN(do_not_map_window) ; /* for menu containing detached window */
  ZMW_BOOLEAN(activated) ;
  ZMW_BOOLEAN(changed) ;	        /* True if the widget has changed */
  ZMW_BOOLEAN(children_activated) ; /* True if a child was activated */
  ZMW_BOOLEAN(tip_visible) ;	/* True if it is a displayed tip */
  ZMW_BOOLEAN(horizontaly_expanded) ;
  ZMW_BOOLEAN(verticaly_expanded) ;
  ZMW_BOOLEAN(focused) ;
  /*
   * If true, the widget is transparent for size, sensible, activate, ...
   * requests. Because it is not this widget we want to test
   * but the previous one.
   * It is used for "tip" for example.
   */
  ZMW_BOOLEAN(pass_through) ;
} ;

struct zmw_font_description
{
  Zmw_Font_Family family ;
  Zmw_Font_Weight weight ; // 0 .. 1000
  Zmw_Font_Size size ; // in point
  Zmw_Font_Style style ;
} ;

typedef struct zmw_stackable_inheritable
{
  int (*action)(void) ;
  int debug ;
  int colors[Zmw_Color_Last] ;
  Zmw_Font_Description font ;
  Zmw_Name *focus ;
  Zmw_Width border_width ;
  Zmw_Width focus_width ;
  Zmw_Boolean auto_resize ;
  Zmw_Boolean sensitive ;
} Zmw_Stackable_Inheritable ;

  /*
   * These values are inherited from parent to child.
   * But each time a widget is initialised, the value
   * is retrieved from the parent.
   * So there is no propagation of value change between siblings .
   */
struct zmw_parent_to_child
{
  GdkWindow **window ;
  GdkGC *gc ;
  Zmw_Rectangle clipping ;
} ;

typedef struct zmw_stackable_uninheritable
{
  int call_number ;		/* Number of call to zmw_begin */
  int nb_of_children, nb_of_children_max ;
  Zmw_Child *children ;
  Zmw_Boolean do_not_execute_pop ;
  Zmw_Rectangle asked ;		// Size asked by the user
  char *name ;			/* Pointer on the last part of full_name */
  char *name_index ;		/* Pointer on the '.#' part of name */
  int name_separator ;		/* In case of ambiguity of the name */
  const char *type ;		/* Content of the ZMW(....) call */
  const char *file ;		/* File where ZMW was declared */
  int line ;			/* Line where ZMW was declared */
  Zmw_External_State external_state ;
  Zmw_Subaction subaction ;	/* The action to process in the widget */
  int *menu_state ;		/* For zmw_window_is_popped_with_detached */
  int child_number ;            /* 0 if the first child, 1 the second, ... */
  Zmw_Child *size ;		/* Fast access to size (-10% lib size) */
  Zmw_Parent_To_Child parent_to_child ;
} Zmw_Stackable_Uninheritable ;

struct zmw_state
{
  Zmw_Stackable_Inheritable i ;
  Zmw_Stackable_Uninheritable u ;
} ;

typedef struct zmw
{
  /*
   * Window list.
   * This window list is created by "zmw_window".
   * The windows in the GDK list and not in this one must be unmapped.
   * See zmw_main.c/zmw_draw
   */
  int nb_windows ;
  GdkWindow **windows ;
  /*
   * Pointer on the current informations.
   * It is a pointer in zmw_table.
   */
  Zmw_State *ptr ;
  int zmw_table_depth ;
  Zmw_State *table ;

  /*
   * These values apply to the widget in which we are
   */
  char full_name[ZMW_MAX_NAME_LENGTH] ; // The widget names are computed here
  /*
   * These values apply only once and are reseted
   */
  Zmw_Boolean external_do_not_make_init ; // For EXTERNAL
  /*
   * These values apply to the last widget (can't be used before first kid)
   */
  /*
   * Theses values are set by zmw_main.c and should not be modified
   */ 
  GdkEvent *event ;		/* Current event, erased when received */
  Zmw_Boolean event_removed ;   /* If true the event had been processed */
  Zmw_Position x, y, x_root, y_root ;	/* Pointer position */
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
  Zmw_Name *focus_with_cursor ; /* Focus group with the cursor (Zmw_Search) */

  struct
  {
    Zmw_Name name ;
    Zmw_Rectangle rectangle ;
    GdkWindow *window ;
  } near[4] ;		/* Left, Right, Up, Down */

  Zmw_Rectangle focused ;       /* The current focused widget position */  
  GdkWindow *raised ;           /* Last window raised */
  Zmw_Name tip_displayed ;	/* Name of the widget with a tip displayed */
  Zmw_Name widget_to_trace ;
#if ZMW_DEBUG_INSIDE_ZMW_PARAMETER
  Zmw_Boolean inside_zmw_parameter ;
#endif
  /*
   * Theses values are private to "zmw_main.c"
   */
  struct zmw_main *run ;         /* Hidden struct, private to zmw_main.c */
#if ZMW_PROFILING
  Zmw_Boolean profiling_displayed ;
#endif
  /*
   * Misc
   */
  char **font_families ;
  int nb_font_families ;
} Zmw ;

#define zMw zmw.ptr

#include <zmw/zmw_generated_private.h>  


#endif
