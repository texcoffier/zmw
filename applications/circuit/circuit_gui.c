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

#include <errno.h>
#include "zmw/zmw.h"
#include "circuit_application.h"

static int gate_width = 10, gate_height = 15 ;

static GdkPixbuf *pb[Gate_EOT+1] = { NULL } ; // Pixmap pointer


void gate_input(int *input, int gate_and_port)
{
  zmw_width(gate_width) ;
  zmw_height(gate_height) ;
  zmw_padding_width(0) ;
  zmw_radio_button(input, gate_and_port) ;
  zmw_width(ZMW_VALUE_UNDEFINED) ;
  zmw_height(ZMW_VALUE_UNDEFINED) ;
}

void gate_output(int *output, int gate)
{
  gate_input(output, gate) ;
}

void gate_output_coordinates(const Gate *g, int *x, int *y)
{
  *x = g->x + g->w + zmw_parent__allocated_x_get() ;
  if ( number_of_inputs[g->type] )
    *x += gate_width ;
  *y = g->y + g->h/2 + zmw_parent__allocated_y_get() ;
}

void gate_input_coordinates(const Gate *g, int port, int *x, int *y)
{
  *x = g->x + zmw_parent__allocated_x_get() ;
  *y = g->y + zmw_parent__allocated_y_get() ;
  if ( number_of_inputs[g->type] == 1 )
    *y += g->h/2 ;
  else
    {
      if ( port )
	*y += g->h - gate_height/2 ;
      else
	*y += gate_height / 2 ;
    }
}

void gate_draw(Circuit *c, Type s, int i, int *input, int *output)
{
  char buf[99] ;
  int icone_w, icone_h ;

  ZMW(zmw_fixed())
    {
      if ( number_of_inputs[c->gates[i].type] )
	zmw_x(gate_width - zmw_border_width_get()) ;
      else
	zmw_x(0) ;
      zmw_y(0) ;
      zmw_border_width(0) ;
      zmw_image_from_file_activable_with_pixbuf(icones[c->gates[i].type]
	  					, &pb[c->gates[i].type]) ;
      icone_w = zmw_min_width_get() ;
      icone_h = zmw_min_height_get() ;

      if ( s == Change_Input && c->gates[i].type == Gate_Value
	   && zmw_activated() )
	circuit_gate_switch_state(c, i) ;
      if ( zmw_drawing() )
	{
	  c->gates[i].w = zmw_allocated_width_get() ;
	  c->gates[i].h = zmw_allocated_height_get() ;
	}

      zmw_border_width(2) ;
      switch( number_of_inputs[c->gates[i].type] )
	{
	case 2:
	  zmw_x(0) ;
	  zmw_y(0) ;
	  gate_input(input, 2*i) ;
	  zmw_x(0) ;
	  zmw_y(icone_h - gate_height) ;
	  gate_input(input, 2*i+1) ;
	  break ;
	case 1:
	  zmw_x(0) ;
	  zmw_y((icone_h - gate_height)/2) ;
	  gate_input(input, 2*i) ;
	  break ;
	}
      if ( number_of_inputs[c->gates[i].type] )
	zmw_x(icone_w + gate_width - 2*zmw_border_width_get()) ;
      else
	zmw_x(icone_w - 2*zmw_border_width_get()) ;
      zmw_y((icone_h - gate_height)/2) ;
      gate_output(output, i) ;

      zmw_horizontal_alignment(1) ;
      zmw_vertical_alignment(1) ;
      zmw_x(icone_w + gate_width/2) ;
      zmw_y(0) ;
      sprintf(buf, "%d", c->gates[i].value) ;
      zmw_label(buf) ;		      
    }
}

void gate_draw_input_arcs(Circuit *c, int i)
{
  int x1, y1, x2, y2, j ;
	
  if ( zmw_drawing() )
    {
      for(j=0; j<number_of_inputs[c->gates[i].type]; j++)
	if ( c->gates[i].input[j] >= 0 )
	  {
	    gate_output_coordinates(&c->gates[c->gates[i].input[j]]
				    , &x1, &y1) ;
	    gate_input_coordinates(&c->gates[i], j, &x2, &y2) ;
	    zmw_draw_line(Zmw_Color_Foreground, x1, y1, x2, y2) ;
	  }
    }
}

void gate_debug(Circuit *c, int i)
{
  char buf[999] ;

  ZMW ( zmw_tip() )
    {
      ZMW(zmw_window_popup_right())
	{      	   
	  zmw_horizontal_alignment(-1) ;	   
	  ZMW(zmw_vbox())
	    {
	      sprintf(buf, "index=%d", i) ;
	      zmw_label(buf) ;
	      sprintf(buf, "type=%d", c->gates[i].type) ;
	      zmw_label(buf) ;
	      sprintf(buf, "pos=(%d,%d)"
		      , c->gates[i].x, c->gates[i].y);
	      zmw_label(buf) ;
	    }
	} 
    }
}

void gate_draw_interactive_arc(Circuit *c, int *input, int *output)
{
  int x1, y1 ;
	
  if ( *input >= 0 )
    {
      if ( *output >= 0 )
	{
	  circuit_arc_add(c, *input/2, *input%2, *output) ;
	  *input = -1 ;
	  *output = -1 ;
	}
      else
	{
	  if ( zmw_drawing() )
	    {
	      gate_input_coordinates(&c->gates[*input/2], *input%2, &x1, &y1) ;
		  
	      zmw_draw_line(Zmw_Color_Foreground, x1, y1
			    , zmw_zmw_x_get(), zmw_zmw_y_get()) ;
	    }
	}
    }
  else
    if ( *output >= 0 )
      {
	if ( zmw_drawing() )
	  {
	    gate_output_coordinates(&c->gates[*output], &x1, &y1) ;
		  
	    zmw_draw_line(Zmw_Color_Foreground, x1, y1, zmw_zmw_x_get(), zmw_zmw_y_get()) ;
	  }
      }
}

void gate_area(Circuit *c, Type s)
{
  int i ;
  static int input = -1 ;
  static int output = -1 ;
  static int x, y ;
  Gate ng ;

  zmw_horizontal_alignment(0) ;
  zmw_vertical_alignment(0) ;
  zmw_horizontal_expand(1) ;
  zmw_vertical_expand(1) ;
  zmw_padding_width(0) ;

  ZMW(zmw_fixed())
    {
 
      for(i=0; c->gates[i].type != Gate_EOT; i++)
	{
	  if ( c->gates[i].type == Gate_Deleted )
	    continue ;
	  ZMW(zmw_anchor_move_with_init_values(&c->gates[i].x, &c->gates[i].y
					       , &x, &y)
					       )
	    {
	      gate_draw(c, s, i, &input, &output) ;
	    }
	  if ( ( c->gates[i].x != x || c->gates[i].y != y ) && zmw_activated() )
	    {
	      ng = c->gates[i] ;
	      c->gates[i].x = x ;
	      c->gates[i].y = y ;
	      circuit_add_to_history(c, i,&ng,"Move %d to %dx%d",i,ng.x,ng.y) ;
	    }
          if ( s == Gate_Delete && zmw_button_pressed() )
	    {
	      circuit_gate_delete(c, i) ;
	      input = -1 ;
	      output = -1 ;
	      zmw_event_remove() ;
	      zmw_focus_remove() ;
	    }
	  if ( zmw_selected() )
	      circuit_eval(c) ;	/* Recompute circuit size */
	  gate_debug(c, i) ;
	  gate_draw_input_arcs(c, i) ;
	}
      gate_draw_interactive_arc(c, &input, &output) ;
    }
  if ( s < Change_Input && zmw_activated() )
    {	
      circuit_gate_add(c, s, zmw_zmw_x_get() - zmw_allocated_x_get()
		       , zmw_zmw_y_get() - zmw_allocated_y_get()) ;
      zmw_need_repaint() ;
    }
}

void circuit_history(Circuit *c)
{
  int i ;
  char buf[99] ;

  zmw_auto_resize(1) ;
  ZMW(zmw_window("Command History"))
    {
      ZMW(zmw_vbox())
	{
	  for(i=0; i<c->history_size; i++)
	    {
	      sprintf(buf, "%c%s%s"
		      , i == c->history_position ? '*' : ' '
		      , c->history[i].real ? "" : "    "
		      , c->history[i].comment
		      ) ;
	      zmw_label(buf) ;
	    }
	}
    }
}

void circuit_menu_bar(Circuit *c)
{
  static int display_history = 0 ;
  static Zmw_Boolean filename_to_open = 0, filename_to_save = 0 ;
  static char *alert_message = NULL ;

  zmw_horizontal_alignment(-1) ;
  zmw_vertical_expand(0) ;
  ZMW(zmw_hbox())
    {
      zmw_horizontal_expand(0) ;
      zmw_button("File") ;
      ZMW( zmw_menu() )
	ZMW(zmw_window_popup_bottom())
	{
	  ZMW(zmw_vbox())
	    {
	      zmw_tearoff() ;
	      zmw_button_with_accelerator("Save",GDK_CONTROL_MASK, 'S') ;
	      if ( zmw_activated() )
		circuit_save(c) ;
	      zmw_button("Save As") ;
	      if ( zmw_activated() )
		filename_to_save = 1 ;
	      zmw_button_with_accelerator("Open",GDK_CONTROL_MASK, 'O') ;
	      if ( zmw_activated() )
		filename_to_open = 1 ;
	      zmw_button_with_accelerator("Quit",GDK_CONTROL_MASK, 'Q') ;
	      if ( zmw_activated() )
		zmw_main_quit(0) ;
	    }
	}
      zmw_button("Edit") ;
      ZMW( zmw_menu() )
	ZMW(zmw_window_popup_bottom())
	{
	  ZMW(zmw_vbox())
	    {
	      zmw_tearoff() ;
	      zmw_button("Snap to grid") ;
	      if ( zmw_activated() )
		circuit_snap_to_grid(c) ;
	      zmw_button_with_accelerator("Undo",GDK_CONTROL_MASK, 'Z') ;
	      if ( c->history_position > 0 && zmw_activated() )
		circuit_do(c, -1) ;
	      zmw_button_with_accelerator("Redo",GDK_CONTROL_MASK, 'R') ;
	      if ( c->history_position < c->history_size && zmw_activated() )
		circuit_do(c, 1) ;
	      zmw_check_button_int_with_label(&display_history, "Undo/Redo Logs") ;
	    }
	}
    }
  if ( display_history )
    circuit_history(c) ;

  zmw_auto_resize(Zmw_False) ;
  zmw_file_selection(&filename_to_save
		  , &c->filename, "Circuit Salvator", "Save circuit") ;
  if ( zmw_activated() )
    if ( circuit_save(c) )
      {
	filename_to_save = 1 ;
	alert_message = strerror(errno) ;
      }

  zmw_file_selection(&filename_to_open
		  , &c->filename, "Circuit Resurector", "Load circuit") ;
  if ( zmw_activated() )
    if ( circuit_load(c) )
      alert_message = strerror(errno) ;

  zmw_alert(&alert_message) ;

}

void circuit_mode(const Circuit *c, Type *s)
{
  int i ;
  char buf[999] ;
 
  zmw_color(Zmw_Color_Background_Poped, 0x00D0D0D0) ;
  zmw_horizontal_expand(0) ;
  ZMW(zmw_vbox())
    {
      for(i=0; names[i]; i++)
	{
	  if ( i == *s )
	    zmw_color(Zmw_Color_Background_Poped, 0x0000FF00) ;
	  if ( icones[i][0] )
	    zmw_image_from_file_activable_with_pixbuf(icones[i], &pb[i]) ;
	  else
	    zmw_button(names[i]) ;
	  if ( i == *s )
	    zmw_color(Zmw_Color_Background_Poped, 0x00D0D0D0) ;
	  if ( zmw_activated() )
	    *s = i ;
	}
      sprintf(buf, "#gates = %d", c->nb) ;
      zmw_label(buf) ;
      sprintf(buf, "(%dx%d)", c->w, c->h) ;
      zmw_label(buf) ;
    }
  zmw_border_embossed_in_draw() ;
}

void circuit()
{
  static Circuit c = { NULL } ;
  static Type s = Change_Input ;
  static Zmw_Float_0_1 x=0, y=0 ;
  char tmp[999] ;

  if ( c.gates == NULL )
    {
      circuit_init(&c) ;
      circuit_load(&c) ;
    }

  zmw_name("?") ;
  sprintf(tmp, "Circuit Editor: %s%s", c.filename
	  , c.history_position ? " (Modified)":"") ;
  ZMW(zmw_window(tmp))
    {
      zmw_width(500) ;
      zmw_height(500) ;
      zmw_vertical_alignment(-1) ;
      ZMW(zmw_vbox())
	{
	  circuit_menu_bar(&c) ;
	  zmw_vertical_expand(1) ;
	  ZMW(zmw_hbox())
	    {
	      zmw_vertical_expand(0) ;
	      circuit_mode(&c, &s) ;
	      zmw_horizontal_expand(1) ;
	      zmw_vertical_expand(1) ;

	      ZMW(zmw_viewport_with_scrollbar(&x, &y))
		{
		  zmw_width(c.w) ;
		  zmw_height(c.h) ;
		  gate_area(&c, s) ;
		  /*
  ZMW ( zmw_tip() )
    {
      ZMW(zmw_window_popup_right())
	{      	   
	  zmw_label("X") ;
	}
    }
		  */
		}
	    }
	}
      if ( zmw_debug_message() )
      	http_printf("<B STYLE='background: yellow;'>s = %s</B><BR>"
		    , names[s]) ;
    }
}

int main(int argc, char **argv)
{
  zmw_init(&argc, &argv) ;
  zmw_main(circuit) ;
  return(0) ;
}
