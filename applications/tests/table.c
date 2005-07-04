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

void test_table(const char *title)
{
  int i ;
  char buf[99], tip[99] ;

  ZMW(zmw_window(title))
    {
      ZMW(zmw_table(11))
	{
	  for(i=0; i<144; i++)
	    {
	      sprintf(buf, "%si=%d", (i/2)%2 ? "######" : "", i) ;
	      zmw_horizontal_alignment(((i/4)%3)-1) ;
	      zmw_vertical_alignment  (((i/12)%3)-1) ;
	      zmw_horizontal_expand   ((i/36)%2) ;
	      zmw_vertical_expand     ((i/72)%2) ;
	      
	      if ( i&1 )
	      	zmw_button(buf) ;
	      	else
	      	{
	      	ZMW(zmw_vbox())
	      		{
		      	zmw_label("over") ;
		      	zmw_button(buf) ;
		      	zmw_label("under") ;
	      		}
	      	zmw_border_embossed_in_draw() ;
	      	}
	      	
	      ZMW ( zmw_tip() )
	      	{
	      	   ZMW(zmw_window_popup_right())
	      	   	{
		        zmw_color(Zmw_Color_Foreground, 0x00ff0000) ;
		        zmw_color(Zmw_Color_Border_Light, 0x00FF8080) ;
		        zmw_color(Zmw_Color_Border_Dark, 0x00900000) ;
	      	   	ZMW(zmw_vbox())
	      	   		{
	      	   		sprintf(tip, "Text=%s", buf) ;
	      	   		zmw_label(tip) ;
	      	   		sprintf(tip, "Horizontal alignment = %d"
	      	   			,zmw_cs_horizontal_alignment_get()) ;
	      	   		zmw_label(tip) ;
	      	   		sprintf(tip, "Horizontal expand = %d"
	      	   			,zmw_horizontaly_expanded_get()) ;
	      	   		zmw_label(tip) ;
	      	   		sprintf(tip, "Vertical alignment = %d"
	      	   			, zmw_cs_vertical_alignment_get()) ;
	      	   		zmw_label(tip) ;
	      	   		sprintf(tip, "Vertical expand = %d"
	      	   			, zmw_verticaly_expanded_get()) ;
	      	   		zmw_label(tip) ;
	      	   		if ( ! (i&1) )
	      	   			zmw_label("With 'under' and 'over'") ;
	      	   		}
	      	   	zmw_border_embossed_out_draw() ;	
	      	   	}	
	      	}
	    }
	}
    }
}

void test_table_width(const char *title)
{
  int i ;
  char buf[99] ;
  static int width[99] = { -1 } ;
  static Zmw_Boolean selection[100] = { Zmw_False } ;

  if ( width[0] == -1 )
  	for(i=0; i<7; i++)
  		width[i] = rand()%30 + 70 ;

  ZMW(zmw_window(title))
    {
      ZMW(zmw_table_with_widths_and_selection(7, width, selection, Zmw_False))
	{
		
	  for(i=' '; i<127; i++)
	    {
	      zmw_horizontal_alignment(((i)%3)-1) ;
	      zmw_vertical_alignment  (((i/3)%3)-1) ;
	      zmw_horizontal_expand   ((i/9)%2) ;
	      zmw_vertical_expand     ((i/18)%2) ;
	      ZMW(zmw_hbox())
	      	{
	      		zmw_label("{{{") ;
		        sprintf(buf, "%c", i) ;
	      		zmw_button(buf) ;
	      		if ( zmw_activated() )
	      			zmw_printf("%s activated\n", buf) ;
	      		zmw_label("}}}") ;
	      	}
	    }
	}
    }
}	
		
		
