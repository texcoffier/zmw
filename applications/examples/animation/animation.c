/* DO NOT DISPLAY */
#include <sys/time.h>
#include "zmw/zmw.h"

// Return the real time in seconds from the first call.
float current_time()
{
  struct timeval tv ;
  static int start_sec = 0 ;

  if ( start_sec == 0 )
    {
      gettimeofday(&tv, NULL);
      start_sec = tv.tv_sec ;
    }
  gettimeofday(&tv, NULL);
  return (tv.tv_sec - start_sec) + tv.tv_usec/1000000. ;
}

/* DO NOT DISPLAY */
struct button
{
  char *label ;             // Button label
  int x1, y1, x2, y2 ;      // Position 1 and 2 
  float movement_duration ; // Duration of the translation

  int x,y ;                 // Current button position
  int x_new, y_new ;        // New button position
  int normal ;              // if 1: move from position 1 to 2
  int moving ;              // if 1: the button is moving
  float start_time ;        // Time of the start of the movement  
} ;

void animate(struct button *b)
{
  float t ;

  // If it is the first call, initialise the button
  if ( b->start_time == 0 )
    {
      b->x = b->x_new = b->x1 ;
      b->y = b->y_new = b->y1 ;
      b->start_time = -1 ;
    }
  zmw_x(b->x) ;
  zmw_y(b->y) ;
  zmw_button(b->label) ;
  if ( zmw_activated() )
    {
      b->normal = !b->normal ; // Change the travel direction
      if ( b->moving )
	{
	  b->start_time = 2*current_time() - b->start_time
	    - b->movement_duration ;
	}
      else
	{
	  b->start_time = current_time() ;
	  b->moving = 1 ;
	}
    }
  if ( zmw_drawing() && b->moving )
    {
      t = ( current_time() - b->start_time) / b->movement_duration ;
      if ( t > 1 )
	{
	  t = 1 ;
	  b->moving = 0 ;
	}
      if ( !b->normal )
	t = 1 - t ;
      // do not modify x,y here because it change program state.
      b->x_new = (b->x2 - b->x1) * t + b->x1 ;
      b->y_new = (b->y2 - b->y1) * t + b->y1 ;
    }
}

void animation(void)
{
  static struct button b[] =
    {
      { "leftright"    ,   0,  0, 100, 0,  1. },
      { "slowleftright",   0, 30, 100,30,  5. },
      { "downtop"      ,   0, 75,   0, 0,  3. },
      { "diagonal"     , 100, 75,   0, 0,  2. },
    } ;
  int i ;
  
  ZMW(zmw_window("Animation"))
    {
      zmw_width(200) ;
      zmw_height(100) ;
      ZMW(zmw_fixed())
	{
	  for(i=0; i<ZMW_TABLE_SIZE(b); i++)
	    animate(&b[i]) ;
	}
    }
  // Here a program state change is allowed even it is not
  // an event handling because it is outside any widget.
  for(i=0; i<ZMW_TABLE_SIZE(b); i++)
    {
      b[i].x = b[i].x_new ;
      b[i].y = b[i].y_new ;
    }

  for(i=0; i<ZMW_TABLE_SIZE(b); i++)
    if ( b[i].moving )
      {
	zmw_need_repaint() ;
	break ;
      }
}
/* DO NOT DISPLAY */

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(animation) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

# DO_NOT_MAKE_REGRESSION_TEST

zmw_move_cursor_to 41 38          # On leftright
zmw_dump_screen 0                 # Dump screen

zmw_button_click
zmw_move_cursor_to 41 68          # On slowleftright
zmw_button_click
zmw_move_cursor_to 41 114         # On downtop
zmw_button_click
zmw_move_cursor_to 141 112        # On diagonal
zmw_button_click

zmw_dump_screen 1                 # Dump screen
zmw_dump_screen 2                 # Dump screen
zmw_dump_screen 3                 # Dump screen



REGRESSION TEST */
