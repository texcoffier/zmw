#include "zmw/zmw.h"

void hello_world(void)
{
  static GdkWindow *w = NULL ;

  ZMW(zmw_window_with_id(&w, "Title"))
    {
      zmw_label("Hello World! I am a label.") ;
    }
}

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ; // Take ZMW parameters
  zmw_main(hello_world) ;   // Launch the infinite loop
  return 0 ;
}

/* REGRESSION TEST

zmw_move_cursor_to 26 15
zmw_dump_screen 0

REGRESSION TEST */
