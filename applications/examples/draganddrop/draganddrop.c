/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

#define NUMBER_OF_BOXES 3
#define BOX_MAX_CONTENT 99

void box_item_remove(int *box, int item)
{
  for(box = &box[item]; *box != -1; box++)
    box[0] = box[1] ;
}

void box_item_add(int *box, int value)
{
  while( *box != -1 )
    box++ ;
  box[0] = value ;
  box[1] = -1 ;
}

void box_item_draggable(int *box, int item)
{
  char message[99] ;
 
  if ( zmw_drag_from_started() )
    {
      sprintf(message, "%d value is dragged", box[item]) ;
      zmw_drag_data_set(message) ;
    }
  if ( zmw_drag_from_stopped() )
    {
      if ( zmw_drag_accept_get() )
	{
	  box_item_remove(box, item) ;
	}
    }
  ZMW( zmw_if( zmw_drag_from_running() ) )
    {
      ZMW(zmw_window_drag())
	{
	  sprintf(message, "%d %s accepted", box[item],
		  zmw_drag_accept_get() ? " is" : " not"
		  ) ;
	  zmw_label(message) ;
	}
    }
}

void box_take_drop(int *box, char **message, int multiple)
{
  int value ;

  if ( zmw_drag_to_enter() )
    {
      value = atoi(zmw_drag_data_get()) ;
      zmw_drag_accept_set( (value % multiple) == 0 ) ;
      *message = zmw_drag_accept_get() ? "I Accept" : "I Refuse" ;
    }
  if ( zmw_drag_to_leave() )
    {
      *message = NULL ;
    }
  if ( zmw_drag_to_dropped() )
    {
      if ( zmw_drag_accept_get() )
	box_item_add(box, atoi(zmw_drag_data_get()) ) ;
      *message = NULL ;
    }
}

void draganddrop(void)
{
  static int boxes[NUMBER_OF_BOXES][BOX_MAX_CONTENT]
    = { {1, 6, -1}, {2, 4, 6, -1}, {3, 6, 9, -1} } ;
  static char *messages[NUMBER_OF_BOXES] = { NULL } ;
  int i, j ;
  char title[99] ;

  for(i=0; i<NUMBER_OF_BOXES; i++)
    {
      sprintf(title, "%d Multiples", i+1) ;
      ZMW(zmw_window(title))
	{
	  zmw_height(150) ;
	  zmw_vertical_expand(Zmw_False) ;
	  ZMW(zmw_vbox())
	    {
	      zmw_label(title) ;
	      for(j=0; boxes[i][j] != -1; j++)
		{
		  ZMW(zmw_decorator(Zmw_Decorator_Border_Solid))
		    zmw_int(boxes[i][j]) ;
		  box_item_draggable(boxes[i], j) ;
		}
	      zmw_label(messages[i] ? messages[i] : "Nothing") ;
	    }
	  box_take_drop(boxes[i], &messages[i], i+1) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(draganddrop) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

# There is a bug in drag and drop.
# A motion is required to update the receiver state
zmw_move_cursor_to_() {
zmw_move_cursor_to `expr $1 + 1` `expr $2 + 1`
zmw_move_cursor_to $1 $2
}

zmw_move_cursor_to 74 49  # box0[0] = 1
zmw_dump_screen 0

zmw_button_press          #
zmw_dump_screen 1

zmw_move_cursor_to_ 117 52 # box1[0] = 2 It is not accepted
zmw_dump_screen 2

zmw_move_cursor_to_ 205 49 # box2[0] = 3 It is not accepted
zmw_dump_screen 3

zmw_button_release        # DROP not accepted
zmw_dump_screen 4

zmw_move_cursor_to 37 71  # box1[1] = 6
zmw_button_press          #
zmw_dump_screen 5

zmw_move_cursor_to_ 117 52 # box1[0] = 2 It is accepted
zmw_dump_screen 6

zmw_move_cursor_to_ 205 49 # box2[0] = 3 It is accepted
zmw_dump_screen 7

zmw_move_cursor_to 400 100 # Outside the windows and it is not accepted
zmw_dump_screen 8

zmw_button_release        # DROP not accepted
zmw_dump_screen 9

zmw_move_cursor_to 74 49  # box0[0] = 1
zmw_button_press
zmw_move_cursor_to 60 50  # box0[0] = 1
zmw_button_release        # DRAG/DROP, this change item order
zmw_dump_screen 10

zmw_move_cursor_to 205 49 # box2[0] = 3
zmw_button_press           # START DRAG
zmw_move_cursor_to_ 60 49  # box0[0] = 6
zmw_button_release
zmw_dump_screen 11

zmw_move_cursor_to 205 49 # box2[0] = 6
zmw_button_press          # START DRAG
zmw_move_cursor_to_ 117 52 # box1[0] = 2 It is accepted
zmw_button_release
zmw_dump_screen 12

zmw_move_cursor_to 117 52 # box1[0] = 2
zmw_button_press          # START DRAG
zmw_move_cursor_to_ 74 49  # box0[0] = 6 It is accepted
zmw_button_release
zmw_dump_screen 13

zmw_move_cursor_to 136 70 # box1[1] = 6
zmw_button_press          # START DRAG
zmw_move_cursor_to_ 208 49 # box2[0] = 9 It is accepted
zmw_button_release
zmw_dump_screen 14

REGRESSION TEST */
