/* DO NOT DISPLAY */
#include "zmw/zmw.h"
void create_circle(GdkPixbuf *pb, int radius, int width, int height)
{
  int x, y ;
  guchar *pixels, c ;

  pixels = gdk_pixbuf_get_pixels(pb) ;
  for(x=0; x<width; x++)
    for(y=0; y<height; y++)
      {
	if ( (x-width/2)*(x-width/2) + (y-height/2)*(y-height/2)
	     < radius*radius )
	  c = 255 ;
	else
	  c = 0 ;
	pixels[(x + y*width)*3] = c ;
	pixels[(x + y*width)*3+1] = c ;
	pixels[(x + y*width)*3+2] = c ;
      }
}
/* DO NOT DISPLAY */
void image(void)
{
  static GdkPixbuf *pb = NULL ;
  static Zmw_Float_0_1 radius = 0.5, old_radius = -1 ;
  const int width = 100, height = 100 ;

  if ( pb == NULL )
      pb = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, width, height) ;

  if ( radius != old_radius )
    {
      // Create the image in the pixbuf
      create_circle(pb, radius*100, width, height) ;
      old_radius = radius ;
    }

  ZMW(zmw_window("Image"))
    {
      ZMW(zmw_vbox())
	{
	  // If the big disc button is clicked, radius is set to 50
	  zmw_image_from_file_activable("button.png") ;
	  if ( zmw_activated() )
	      radius = 0.5 ;
	  // Non activable image
	  zmw_image_from_file("img.png") ;
	  // A scrollbar to modify the radius
	  zmw_hscrollbar(&radius, 0.1) ;
	  // The image with the disc picture computed.
	  zmw_image(pb) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(image) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 37 104 # On scrollbar
zmw_dump_screen 0

zmw_button_press
zmw_move_cursor_to 25 60
zmw_dump_screen 1

zmw_button_release
zmw_move_cursor_to 29 46
zmw_button_click
zmw_dump_screen 2

REGRESSION TEST */
