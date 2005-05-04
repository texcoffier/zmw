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

/*

Beware : cursor move not yet UTF8 compatible

*/

#include <gdk/gdkkeysyms.h>
#include "zmw/zmw.h"
#include "zmw/selection.h"
// #include <glib/gunicode.h>


void zmw_text_insert(char **text, int *cursor_pos, int *start_pos
		     , const char *insert)
{
  int len ;

  len = strlen(insert) ;
	
  if ( start_pos && *start_pos != -1
       && cursor_pos && *start_pos > *cursor_pos )
    *start_pos += len ;

  ZMW_REALLOC(*text, strlen(*text)+ len + 1) ;

  memmove(*text + *cursor_pos + len
	  ,*text + *cursor_pos
	  ,strlen(*text) - *cursor_pos + 1
	  ) ;
  strncpy(*text + *cursor_pos, insert, len) ;
  *cursor_pos += len ;
}

void zmw_text_backspace(char **text, int *cursor_pos, int *start_pos)
{
  if ( *start_pos != -1  && *start_pos > *cursor_pos )
    *start_pos = zmw_text_previous_char(*start_pos) ;

  *cursor_pos = zmw_text_delete_char(*cursor_pos) ;
}

void zmw_text_draw_selection(int start_pos, int cursor_pos)
{
  Zmw_Rectangle first, last, tmp ;
  int x_begin, x_end, y ;

  if ( start_pos != -1 )
    {
      zmw_text_cursor_position(start_pos, &first) ;
      zmw_text_cursor_position(cursor_pos, &last) ;

      if ( first.y > last.y || ( first.y == last.y && first.x > last.x ) )
	{
	  tmp = first ;
	  first = last ;
	  last = tmp ;
	}

      for(y=first.y; y<=last.y; y += first.height)
	{
	  if ( y == first.y )
	    x_begin = first.x ;
	  else
	    x_begin = 0 ;

	  if ( y == last.y )
	    x_end = last.x ;
	  else
	    x_end = ZMW_SIZE_ALLOCATED.width ;

	  zmw_draw_rectangle(Zmw_Color_Background_Poped, Zmw_True
			     , ZMW_SIZE_ALLOCATED.x + x_begin
			     , ZMW_SIZE_ALLOCATED.y + y
			     , x_end - x_begin
			     , first.height
			     ) ;
	    }
    }
}

void zmw_text_draw_text(Zmw_Boolean activable, Zmw_Boolean editable)
{
  /*
   * Draw text normal or gray if not sensitive
   */
  if ( (ZMW_PARENT_SIZE.sensible && ZMW_SENSIBLE) ||
       (!activable && !editable) )
    zmw_text_render(Zmw_Color_Foreground
		    , ZMW_SIZE_ALLOCATED.x, ZMW_SIZE_ALLOCATED.y) ;  
  else
    {
      zmw_text_render(Zmw_Color_Border_Light
		    , ZMW_SIZE_ALLOCATED.x-1, ZMW_SIZE_ALLOCATED.y-1) ;  
      zmw_text_render(Zmw_Color_Border_Dark
		    , ZMW_SIZE_ALLOCATED.x, ZMW_SIZE_ALLOCATED.y) ;
    }
}

void zmw_text_draw_cursor(int cursor_pos)
{
  Zmw_Rectangle r ;

  zmw_text_cursor_position(cursor_pos, &r) ;
  zmw_draw_line(Zmw_Color_Border_Light
		, ZMW_SIZE_ALLOCATED.x + r.x, ZMW_SIZE_ALLOCATED.y + r.y
		, ZMW_SIZE_ALLOCATED.x + r.x, ZMW_SIZE_ALLOCATED.y + r.y + r.height
		) ;
}

void zmw_text_selection_take(const char *text, Zmw_Name *selectionning
			     , int *start_pos, const int *cursor_pos)
{
  char *t ;
  int len ;
      		
  if ( *start_pos == *cursor_pos )
    *start_pos = -1 ;
  else
    {
      zmw_selection_take() ;
      zmw_need_repaint() ;
      len = ZMW_ABS(*start_pos - *cursor_pos) ;
      ZMW_MALLOC(t, len + 1) ;
      strncpy(t, &text[ZMW_MIN(*start_pos, *cursor_pos)], len);
      t[len] = '\0' ;

      zmw_set_selection("PRIMARY", "UTF8_STRING", *ZMW_WINDOW, t) ;
    }
  zmw_name_unregister(selectionning) ;
}

void zmw_text_cursor_set(const char *text, int *cursor_pos
			 , int *start_pos, Zmw_Name *selectionning)
{
  int i ;

  i = zmw_text_xy_to_index(zmw.x - ZMW_SIZE_ALLOCATED.x
			   , zmw.y - ZMW_SIZE_ALLOCATED.y
			   ) ;
  *cursor_pos = i ;
  if ( start_pos )
    {
      *start_pos = i ;
      zmw_name_register(selectionning) ;
    }
}


void zmw_text_simple(char **text, Zmw_Boolean editable, Zmw_Boolean activable
		     , int *cursor_pos, int *start_pos)
{
  int x ;
  static Zmw_Name selectionning = ZMW_NAME_UNREGISTERED("Selectionning") ;
  int len = strlen(*text) ;
  char *sel ;

  ZMW_ASSERT( *text != NULL ) ;

  if ( zmw_debug_message() )
    {
      http_printf("(%s)<BR>\n", *text) ;
    }

  if ( ( (ZMW_SUBACTION == Zmw_Input_Event
	|| ZMW_SUBACTION
	== Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing)
	 ) && editable )
    {
      zmw_resource_int_get(&cursor_pos, "TextCursorPos", len) ;
      ZMW_CLAMP(*cursor_pos, 0, len) ;
      zmw_resource_int_get(&start_pos, "TextStartPos", -1) ;
      ZMW_CLAMP(*start_pos, -1, len) ;
    }


  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      zmw_text_set_text( *text, &ZMW_FONT ) ;
      zmw_text_get_size(&ZMW_SIZE_MIN.width, &ZMW_SIZE_MIN.height) ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:

      zmw_text_set_text( *text, &ZMW_FONT ) ;
      if ( start_pos && !zmw_selection_have() && !zmw_name_registered(&selectionning) )
      	{
	  *start_pos = -1 ;
      	}
      if ( editable )
	zmw_text_draw_selection(*start_pos, *cursor_pos);
      zmw_text_draw_text(activable, editable) ;
      if ( cursor_pos && zmw_focused_by_its_parents() )
	zmw_text_draw_cursor(*cursor_pos) ;
      break ;
    case Zmw_Input_Event:

      zmw_text_set_text( *text, &ZMW_FONT) ;
      if ( !editable )
	break ;
      // if ( editable && zmw.event->type == GDK_KEY_PRESS ) zmw_event_dump() ;

      /*
       * Set the selection on button 1 release
       */
      if ( zmw_button_released_anywhere() && zmw_name_is(&selectionning) )
	zmw_text_selection_take(*text, &selectionning
				, start_pos, cursor_pos) ;
      /*
       * Position the text cursor on button 1 click
       * Or paste on button 2 click
       */
      if ( zmw_button_pressed() && cursor_pos && *text )
      	{
	  if ( zmw.event->button.button == 1 )
	    zmw_text_cursor_set(*text,cursor_pos, start_pos,&selectionning);
	  else if ( zmw.event->button.button == 2 )
	    {
	      sel = zmw_get_selection("PRIMARY", "UTF8_STRING", *ZMW_WINDOW) ;
	      if ( sel[0] == '\0' )
		sel = zmw_get_selection("PRIMARY", "STRING", *ZMW_WINDOW) ;

	      zmw_text_insert(text, cursor_pos, start_pos, sel) ;
	      zmw_event_remove() ;
	    }
	  // zmw_event_remove() ;  decorator must take this event
	  // zmw.event_removed = 1 ;
      	}
      /*
       * Insert some key
       */
      if ( 0 )
	zmw_printf("key pressed = %d removed=%d insode=%d sensible=%d\n"
		   , zmw_key_pressed()
		   , zmw.event_removed
		   , zmw_name_is_inside(ZMW_FOCUS)
		   , ZMW_SENSIBLE
		   ) ;
      if ( zmw_key_pressed() )
	{
	  if ( zmw.event->key.string[0] == '\r' )
	    zmw.event->key.string[0] = '\n' ;

	  if ( cursor_pos && zmw.event->key.keyval == GDK_BackSpace )
	    {
	      zmw_text_backspace(text, cursor_pos, start_pos) ;
	      ZMW_SIZE_CHANGED = Zmw_True ;
	    }
	  else if ( cursor_pos && zmw.event->key.keyval == GDK_Left )
	    {
	      *cursor_pos = zmw_text_previous_char(*cursor_pos) ;
	    }
	  else if ( cursor_pos && zmw.event->key.keyval == GDK_Right )
	    {
	      *cursor_pos = zmw_text_next_char(*cursor_pos) ;
	    }
	  else if ( cursor_pos && zmw.event->key.keyval == GDK_Up )
	    {
	      *cursor_pos = zmw_text_up_char(*cursor_pos) ;
	    }
	  else if ( cursor_pos && zmw.event->key.keyval == GDK_Down )
	    {
	      *cursor_pos = zmw_text_down_char(*cursor_pos) ;
	    }
	  else if ( 0 && zmw.event->key.string[0] == '\n' )
	    {
	      ZMW_SIZE_ACTIVATED = Zmw_True ;
	    }  
	  else if ( cursor_pos && zmw.event->key.string[0] )
	    {
	      zmw_text_insert(text, cursor_pos, start_pos
			      , zmw.event->key.string) ;
	      ZMW_SIZE_CHANGED = Zmw_True ;
	    }  
	  zmw_event_remove() ;
	}

      if ( zmw_name_is(&selectionning) )
      	{
	  x = zmw_text_xy_to_index(zmw.x - ZMW_SIZE_ALLOCATED.x
				   , zmw.y - ZMW_SIZE_ALLOCATED.y
				   ) ;
	  if ( x != *start_pos )
	    {
	      zmw_need_repaint() ;
	      *start_pos = x ;
	    }
	}

      /* An editable text is not draggable because
       * the decorator does not receive the button press
       */
      if ( zmw_name_registered(&selectionning) && zmw.event->type == GDK_MOTION_NOTIFY
	   && editable )
	{
	  /* If the event is not removed, we see selection move
	   * on all the text widget.
	   */
	  // zmw_event_remove() ;
      	}
      break ;
    default:
      break ;
    }
}

void zmw_label(const char *text)
{
  ZMW(zmw_text_simple( (char**)&text, Zmw_False, Zmw_False, NULL, NULL)) { }
}

void zmw_text_editable_with_cursor_and_start(char **text, int *cursor_pos
					     , int *start_pos)
{
  int changed ;

  changed = Zmw_False ;
  ZMW(zmw_decorator(  Zmw_Decorator_Border_Relief
		    | Zmw_Decorator_Border_In
		    | Zmw_Decorator_Interior
		    | Zmw_Decorator_Focusable
		    )
      )
    {
      ZMW(zmw_text_simple( text, Zmw_True, Zmw_False, cursor_pos, start_pos))
	{ }
      changed = zmw_changed() ;
    }

  if ( zmw_activated() )
    {
    }

  ZMW_SIZE_CHANGED = changed ;
  if ( ZMW_SIZE_CHILD_ACTIVATED )
    {
      ZMW_SIZE_ACTIVATED = Zmw_True ;
    }
}

void zmw_text_editable_with_cursor(char **text, int *cursor_pos)
{
  zmw_text_editable_with_cursor_and_start(text, cursor_pos, NULL) ;
}

void zmw_entry(char **text)
{
  zmw_text_editable_with_cursor(text, NULL) ;
}

void zmw_int_editable(int *i)
{
  char buf[20], *text ;
  
  sprintf(buf, "%d", *i) ;
  text = strdup(buf) ;
  zmw_entry(&text) ;
  *i = atoi(text) ;
  free(text) ;
}

void zmw_int(int i)
{
  char buf[20] ;
  
  sprintf(buf, "%d", i) ;
  zmw_label(buf) ;
}
