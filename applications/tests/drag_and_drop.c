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


void test_drag_and_drop(const char *title)
{
  static int numbers[3][100] = { {0,1,2,3,4,5,6,7, -1}, {8,10,-1}, {9,12,-1} }  ;
  static char *name[] = { "Integers", "x%2=0 (even)", "x%3=0 (3 multiples)" } ;
  int i, j, k ;
  static int drag_from_j = -1, drag_from_i = -1, drag_to = -1, drag_to_k ;
  char buf[999], buf2[999], buf3[999] ;

  ZMW(zmw_window(title))
    {
      zmw_name("VerticalBox") ;
      zmw_vertical_alignment(-1) ;
      ZMW(zmw_hbox())
	{
	  zmw_name("draginfo") ;
	  ZMW(zmw_vbox())
	    {
	      zmw_label("drag_from_j") ;
	      zmw_int_editable(&drag_from_j) ;
	      zmw_label("drag_from_i") ;
	      zmw_int_editable(&drag_from_i) ;
	      zmw_label("drag_to") ;
	      zmw_int_editable(&drag_to) ;
	      zmw_label("drag_to_k") ;
	      zmw_int_editable(&drag_to_k) ;
	    }
	  for(j=0 ;j<3; j++)
	    {
	      sprintf(buf3, "set%d", j) ;
	      zmw_name(buf3) ;
	      ZMW(zmw_vbox())
		{
		  if ( j == drag_to )
		    {
		      if ( drag_to_k )
			zmw_color(Zmw_Color_Foreground, 0x0000ff00) ;
		      else
			zmw_color(Zmw_Color_Foreground, 0x00ff0000) ;
		    }

		  zmw_name("Title") ;
		  zmw_label(name[j]) ;
		  for(i=0; numbers[j][i]>=0; i++)
		    {
		      sprintf(buf, "L%d", i) ;
		      zmw_name(buf) ;
		      sprintf(buf2, "%d%s", numbers[j][i]
			      , (j==drag_from_j && i==drag_from_i) ? "*" : ""
			      ) ;
		      zmw_label(buf2) ;
		      sprintf(buf, "%d", numbers[j][i]) ;

		      /*
		       * The "from" drag state of this widget.
		       * This widget can be dragged
		       */
		      if ( zmw_drag_from_started() )
			{
			  zmw_drag_data_set(buf) ;	/* Set drag data */
			  drag_from_j = j ;
			  drag_from_i = i ;
			}
		      if ( zmw_drag_from_stopped() )
			{
			  drag_from_j = -1 ;
			  drag_from_i = -1 ;
			}
		      ZMW( zmw_drag_from_running() )
			{
			  sprintf(buf, "%d is %s", numbers[j][i]
				  , zmw_drag_accept_get()
				  ? "accepted"
				  : "not accepted") ;
			  ZMW(zmw_window_drag())
			    {
			      ZMW(zmw_vbox())
				{
				  zmw_label(buf) ;
				}
			      zmw_border_embossed_in_draw() ;
			    }
			}
		    }
		}
	      zmw_border_embossed_out_draw() ;
	      /*
	       * Does a widget is being dragged into this widget
	       */
	      if ( zmw_drag_to_enter() )
		{
		  k = j==0
		    || (j==1 && (atoi(zmw_drag_data_get())%2) == 0)
		    || (j==2 && (atoi(zmw_drag_data_get())%3) == 0)
		    ;
		  drag_to = j ;
		  drag_to_k = k ;
		  zmw_drag_accept_set(k) ;
		}
	      if ( zmw_drag_to_dropped() )
		{
		  if ( zmw_drag_accept_get() )
                    {
                      for(i=0; numbers[j][i]>=0; i++)
                        {
                        }
                      numbers[j][i++] = atoi(zmw_drag_data_get()) ;
                      numbers[j][i++] = -1 ;
                    }
		  drag_to = -1 ;
		}
	      if ( zmw_drag_to_leave() )
		  drag_to = -1 ;
	    }
	}
    }
}
