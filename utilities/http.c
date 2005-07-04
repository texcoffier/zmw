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
 * This file is very ugly, please don't look at it */

#include <ctype.h>
#include "zmw/zmw.h"
#include "zmw/zmw_private.h"
#include "zmw/socket.h"

static FILE *global_http ;
static char *global_url[100] ;
static char *global_name = NULL ;
static int global_x, global_y, global_window, global_event ;



static void append_string(char **s, const char *t)
{
  if ( *s )
    {
      ZMW_REALLOC(*s, strlen(*s)+strlen(t)+1) ;
      strcat(*s, t) ;
    }
  else
    *s = strdup(t) ; 
}

static char* get_string(const char *t)
{
  return strdup(t) ;
}

static char* get_int(int i)
{
  char tmp[99] ;

  sprintf(tmp, "%d", i) ;
  return strdup(tmp) ;
}

static char* get_function_name(const char *v)
{
  char *t ;

  t = get_string(v) ;
  if ( strchr(t, '(') )
    *strchr(t, '(') = '\0' ;
  return t ;
}

static char* get_menu_state(int *n)
{
  char tmp[99] ;

  if ( n )
    sprintf(tmp, "%s%s%s"
	    , (*n & Zmw_Menu_Is_Detached) ? " Detached" : ""
	    , (*n & Zmw_Menu_Is_Poped) ? " Poped" : ""
	    , (*n & Zmw_Menu_Contains_A_Detached) ? " ContainsDetached" : ""
	    ) ;
  else
    strcpy(tmp, "NULL") ;

  return strdup(tmp) ;
}

int evaluate(int item, int *new_column, char **name, int **set, int *in_size,
	     char **value) ;

static char* get_children()
{
  int i, j ;
  int new_col, in_size, *visible ;
  char *name, *value, *v ;
  char tmp[999] ;

  if ( zmw_nb_of_children_get() == 0 )
    return strdup("") ;

  v = strdup("<TABLE border><TR>") ;

  (*zmw_state_get_ptr())++ ;

  for(i=0; evaluate(i, &new_col, &name, &visible, &in_size, NULL); i++)
    if ( in_size && *visible )
      {
	sprintf(tmp, "<TH><SMALL><SMALL>%s</SMALL></SMALL></TH>", name) ;
	append_string(&v, tmp) ;
      }

  append_string(&v, "</TR>") ;

  for(j=0; j<zmw_parent__nb_of_children_get(); j++)
    {
      append_string(&v, "<TR>") ;

      zmw_size_set(&zmw_parent__children_get()[j]) ;
      for(i=0; evaluate(i, &new_col, &name, &visible, &in_size, &value); i++)
	if ( in_size && *visible )
	  {
	    append_string(&v, "<TD>") ;
	    evaluate(i, &new_col, &name, &visible, &in_size, &value) ;
	    append_string(&v, value) ;
	    append_string(&v, "</TD>") ;
	  }
      append_string(&v, "</TR>") ;
    }
  append_string(&v, "</TABLE>") ;
  (*zmw_state_get_ptr())-- ;
    
  return v ;
}

static char* get_rectangle(const Zmw_Rectangle *ws)
{
  char tmp[99] ;
  char *v ;

  v = NULL ;

  if ( ws->x != ZMW_VALUE_UNDEFINED )
    sprintf(tmp,"%d,", ws->x) ;
  else
    sprintf(tmp,"?,") ;
  append_string(&v, tmp) ;

  if ( ws->y != ZMW_VALUE_UNDEFINED )
    sprintf(tmp,"%d", ws->y) ;
  else
    sprintf(tmp,"?") ;
  append_string(&v, tmp) ;

  if ( ws->width != ZMW_VALUE_UNDEFINED )
    sprintf(tmp,"&nbsp;%d", ws->width) ;
  else
    sprintf(tmp,"&nbsp;?") ;
  append_string(&v, tmp) ;

  if ( ws->height != ZMW_VALUE_UNDEFINED )
    sprintf(tmp,"x%d", ws->height) ;
  else
    sprintf(tmp,"x?") ;

  append_string(&v, tmp) ;

  return v ;
}

static char *get_boolean(int i)
{
  if ( i )
    return strdup("True") ;
  else
    return strdup("False") ;
}

static char *get_horizontal_alignment(int i)
{
  static char *a[] = {"Left", "Center", "Right"} ;

  if ( i < -1 ||  i > 1 )
    return strdup("BUG") ;

  return strdup(a[i+1]) ;
}

static char *get_vertical_alignment(int i)
{
  static char *a[] = {"Top", "Center", "Bottom"} ;

  if ( i < -1 ||  i > 1 )
    return strdup("BUG") ;

  return strdup(a[i+1]) ;
}

static char *get_name(const Zmw_Name *n)
{
  char tmp[999] ;

  sprintf(tmp, "%s: %s(%d)", n->why, n->name, (int)n->value) ;
  return strdup(tmp) ;
}

static char *get_color(int n)
{
  Zmw_Float_0_1 r, g, b ;
  char tmp[999] ;

  zmw_int_to_rgb(n, &r, &g, &b) ;
  sprintf(tmp,
	  "%4.2f %4.2f %4.2f"
	  "<span style='background: #%02x%02x%02x;'>X</span>"
	  , r, g, b, (int)(r*255), (int)(g*255), (int)(b*255)) ;
  return strdup(tmp) ;
}

static char *get_pointer(void *n)
{
  char tmp[999] ;

  sprintf(tmp, "%p", n) ;
  return strdup(tmp) ;
}

static char *get_family(int n)
{
  return strdup(zmw.font_families[n]) ;
}

static char *get_pointer_pointer(void **n)
{
  char tmp[999] ;

  sprintf(tmp, "%p", *n) ;
  return strdup(tmp) ;
}

static char *get_event_to_process()
{
  return zmw_event_to_process() ? strdup("True") : strdup("False") ;
}

static char *get_event_in()
{
  return zmw_event_in() ? strdup("True") : strdup("False") ;
}


static char* get_http_debug()
{
  char tmp[999] ;

  sprintf(tmp, "Your debug message") ;

  return strdup(tmp) ;
}

static char *get_zmw()
{
  char tmp[999] ;
  char *v ;

  v = NULL ;


  if ( zmw_zmw_external_do_not_make_init_get() )
    append_string(&v, " external_do_not_make_init") ;
  if ( zmw_zmw_event_get() )
    {
      sprintf(tmp, " event:%p", zmw_zmw_event_get()) ;
      append_string(&v, tmp) ;
      if ( zmw_zmw_event_get()->any.window )
	{
	  sprintf(tmp, " event.window:%p", zmw_zmw_event_get()->any.window) ;
	  append_string(&v, tmp) ;
	}
    }
  if ( zmw_zmw_event_removed_get() )
    append_string(&v, " event_removed") ;

  sprintf(tmp, " %d[%d]%d[%d]", zmw_zmw_x_get(), zmw_zmw_x_root_get(), zmw_zmw_y_get(), zmw_zmw_y_root_get()) ;
  append_string(&v, tmp) ;
  if ( zmw_zmw_window_get() )
    {
      sprintf(tmp, " window:%p", zmw_zmw_window_get()) ;
      append_string(&v, tmp) ;
    }

  if ( zmw_zmw_key_pressed_get() )
    append_string(&v, " key_pressed") ;
  if ( zmw_zmw_button_pressed_get() )
    append_string(&v, " button_pressed") ;
  if ( zmw_zmw_still_yet_displayed_get() )
    append_string(&v, " still_yet_displayed") ;
  if ( zmw_zmw_tips_yet_displayed_get() )
    append_string(&v, " tips_yet_displayed") ;
  if ( zmw_name_registered(zmw_zmw_found_get()) )
    {
      append_string(&v, " found:") ;
      append_string(&v, zmw_zmw_found_get()->name) ;
    }
  if ( zmw_name_registered(zmw_zmw_tip_displayed_get()) )
    {
      append_string(&v, " tip_displayed:") ;
      append_string(&v, zmw_zmw_found_get()->name) ;
    }

  return v ;
}



#define ITEM(NEW_COLUMN, NAME, TO_STRING)			\
if ( item == i++ )						\
  {								\
    static int _visible = 0 ;					\
    *visible = &_visible ;					\
    *new_column = NEW_COLUMN ;					\
    *name = #NAME ;						\
    *in_size = strncmp(#NAME, "u.size->",8) == 0 ;		\
    if ( value && _visible ) old_value = *value = TO_STRING ;	\
    return 1 ;							\
  }								\
else


#define       ITEM_INT(NC, N) ITEM(NC, N, get_int      		  (zmw_state_get()->N))
#define    ITEM_STRING(NC, N) ITEM(NC, N, get_string		  (zmw_state_get()->N))
#define      ITEM_RECT(NC, N) ITEM(NC, N, get_rectangle		  (&zmw_state_get()->N))
#define      ITEM_BOOL(NC, N) ITEM(NC, N, get_boolean  		  (zmw_state_get()->N))
#define    ITEM_VALIGN(NC, N) ITEM(NC, N, get_vertical_alignment  (zmw_state_get()->N))
#define    ITEM_HALIGN(NC, N) ITEM(NC, N, get_horizontal_alignment(zmw_state_get()->N))
#define      ITEM_NAME(NC, N) ITEM(NC, N, get_name                (zmw_state_get()->N))
#define     ITEM_COLOR(NC, N) ITEM(NC, N, get_color               (zmw_state_get()->N))
#define    ITEM_PTRPTR(NC, N) ITEM(NC, N, get_pointer_pointer((void**)zmw_state_get()->N))
#define       ITEM_PTR(NC, N) ITEM(NC, N, get_pointer             (zmw_state_get()->N))
#define      ITEM_MENU(NC, N) ITEM(NC, N, get_menu_state          (zmw_state_get()->N))
#define    ITEM_FAMILY(NC, N) ITEM(NC, N, get_family              (zmw_state_get()->N))


 int evaluate(int item, int *new_column, char **name, int **visible, int *in_size,
	     char **value)
{
  int i ;
  static char *old_value = NULL ;

  ZMW_FREE(old_value) ;

  i = 0 ;
  ITEM         (1,namefull     	           , get_string(zmw_name_full))
    ITEM_STRING(0,u.type)
    ITEM       (0,short_type   	           , get_function_name(zmw_type_get()))
    ITEM_STRING(0,u.file)
    ITEM_INT   (0,u.line)
    ITEM_STRING(0,u.name)
    ITEM_STRING(0,u.name_index )
    ITEM_INT   (0,u.call_number)
    ITEM       (0,action       	           , get_string(zmw_action_name_fct()))
    ITEM_INT   (0,u.nb_of_children)
    ITEM_INT   (0,u.nb_of_children_max)
    ITEM_INT   (0,u.name_separator)
    ITEM_MENU  (0,u.menu_state)
    ITEM       (0,u.children  	           , get_children())
    ITEM_RECT  (1,u.asked)
    ITEM_RECT  (0,u.size->min )
    ITEM_RECT  (0,u.size->required)
    ITEM_RECT  (0,u.size->allocated)
    ITEM_BOOL  (0,u.size->used_by_parent)
    ITEM_INT   (0,u.size->hash_key)
    ITEM_BOOL  (0,u.size->current_state.horizontal_expand)
    ITEM_BOOL  (0,u.size->current_state.vertical_expand)
    ITEM_HALIGN(0,u.size->current_state.horizontal_alignment)
    ITEM_VALIGN(0,u.size->current_state.vertical_alignment)
    ITEM_BOOL  (0,u.size->horizontaly_expanded)
    ITEM_BOOL  (0,u.size->verticaly_expanded)
    ITEM_BOOL  (0,u.size->event_in_rectangle)
    ITEM_BOOL  (0,u.size->event_in_children)
    ITEM_BOOL  (0,u.size->invisible)
    ITEM_BOOL  (0,u.size->sensitived)
    ITEM_BOOL  (0,u.size->focused)
    ITEM_BOOL  (0,u.size->activated)
    ITEM_BOOL  (0,u.size->children_activated)
    ITEM_BOOL  (0,u.size->changed)
    ITEM_BOOL  (0,u.size->tip_visible)
    ITEM_BOOL  (0,u.size->do_not_map_window)
    ITEM_BOOL  (0,u.size->pass_through)
    ITEM_BOOL  (0,u.size->current_state.padding_width)
    ITEM_INT   (1,i.debug)
    ITEM_INT   (0,i.border_width)
    ITEM_INT   (0,i.focus_width)
    ITEM_NAME  (0,i.focus)
    ITEM_COLOR (0,i.colors[Zmw_Color_Background_Normal])
    ITEM_COLOR (0,i.colors[Zmw_Color_Background_Pushed])
    ITEM_COLOR (0,i.colors[Zmw_Color_Background_Poped])
    ITEM_COLOR (0,i.colors[Zmw_Color_Border_Light])
    ITEM_COLOR (0,i.colors[Zmw_Color_Border_Dark])
    ITEM_COLOR (0,i.colors[Zmw_Color_Foreground])
    ITEM_BOOL  (0,i.auto_resize)
    ITEM_BOOL  (0,i.sensitive)
    ITEM_FAMILY(1,i.font.family)
    ITEM_INT   (0,i.font.size)
    ITEM_INT   (0,i.font.style)
    ITEM_INT   (0,i.font.weight)
    ITEM_PTRPTR(0,u.parent_to_child.window)
    ITEM_PTR   (0,u.parent_to_child.gc)
    ITEM_RECT  (0,u.parent_to_child.clipping)
    ITEM       (0,subaction                       , strdup(zmw_action_name()))
    ITEM       (0,zmw_http_debug                  , get_http_debug())
    ITEM       (0,event_to_process                , get_event_to_process())
    ITEM       (0,event_in                        , get_event_in())
    ITEM       (0,zmw                             , get_zmw())
    return 0 ;
}



static char *http_decode_url(const char *url)
{
  static char *decoded = NULL ;
  static int taille_decoded = 0 ;
  int code ;
  int i ;

  for( i=0 ; *url!=' ' && *url!='\0' ; url++)
    {
      if ( i>taille_decoded-10 )
	{
	  taille_decoded = 1.5*taille_decoded + 1000 ;
	  decoded = realloc(decoded, taille_decoded) ;
	}

      if ( *url!='%' ) 
	{
	  if ( *url=='+' )
	    decoded[i++] = ' ' ;
	  else
	    decoded[i++] = *url ;
	}
      else
	{
	  if ( url[1] && url[2] )
	    {
	      sscanf(url+1, "%2x", &code) ;
	      decoded[i++] = code ;
	      url += 2 ;
	    }
	}
    }
  if ( i==0 )
    return("") ;
  else
    decoded[i++] = '\0' ;
  return(decoded) ;
}

char *http_encode_url(const char *url)
{
  static char encoded[9999] ;
  char *e ;

  e = encoded ;
  for( ; *url && (e-encoded)<sizeof(encoded)-4 ; url++)
    {
      if ( isalnum(*url) || *url=='/' || *url=='.' || *url==':'  || *url=='_' || *url=='-')
	*e++ = *url ;
      else
	{
	  sprintf(e, "%%%02X", *(unsigned char*)url) ;
	  e += 3 ;
	}
    }
  *e = '\0' ;
  return(encoded) ;
}

int http_printf(const char *format, ...)
{
    va_list ap;
    int i ;

    va_start(ap, format);
    i = vfprintf(global_http, format, ap) ;
    va_end(ap);
    return(i) ;
}

static void http_rectangle_display(Zmw_Rectangle *ws)
{
  if ( ws->x != ZMW_VALUE_UNDEFINED )
    http_printf("%d,", ws->x) ;
  else
    http_printf("?,") ;

  if ( ws->y != ZMW_VALUE_UNDEFINED )
    http_printf("%d", ws->y) ;
  else
    http_printf("?") ;

  if ( ws->width != ZMW_VALUE_UNDEFINED )
    http_printf("&nbsp;%d", ws->width) ;
  else
    http_printf("&nbsp;?") ;

  if ( ws->height != ZMW_VALUE_UNDEFINED )
    http_printf("x%d", ws->height) ;
  else
    http_printf("x?") ;
}

static void http_size_display(Zmw_Child *ws)
{
  http_printf("<TD>\n") ;
  http_printf("</TD>\n") ;
  http_printf("<TD>\n") ;
  http_rectangle_display(&ws->required) ;
  http_printf("</TD>\n") ;
  http_printf("<TD>\n") ;
  http_rectangle_display(&ws->min) ;
  http_printf("</TD>\n") ;
  http_printf("<TD>\n") ;
  http_rectangle_display(&ws->allocated) ;
  http_printf("</TD>\n") ;

  http_printf(
	  "<TD>%s</TD><TD>%s</TD><TD>%s %s %s</TD>"
	  , zmw_horizontal_alignment_get() < 0
	  ? "Left" : (zmw_horizontal_alignment_get() > 0 ? "Right" : "Center")
	  , zmw_vertical_alignment_get() < 0
	  ? "Up" : (zmw_vertical_alignment_get() > 0 ? "Down" : "Center")
	  , zmw_horizontal_expand_get() ? "Horizontal" : ""
	  , zmw_vertical_expand_get() ? "Vertical" : ""
	  , zmw_used_by_parent_get() ? "" :"NotUsedToComputeParentSize"
	  ) ;
  http_printf("<TD>%s</TD>\n", zmw_sensitived_get() ? "Sensible" : "") ;
}

int http_node()
{
  ZMW_EXTERNAL_HANDLING ;
  
  switch ( (*zmw_call_number_get_ptr())++ )
    {
    case 0:
      zmw_subaction_set(Zmw_Compute_Children_Allocated_Size) ;
      return( zmw_action_first_pass() ) ;

    case 1:
      zmw_subaction_set(Zmw_Nothing) ;

      if ( strcmp(global_name, zmw_name_full) == 0 )
	{
	  http_printf("<TABLE BORDER>\n") ;
	  http_printf("<TR><TH>Type</TH><TD>%s</TD></TR>", zmw_type_get()) ;
	  http_printf("<TR><TH>Declaration</TH><TD>%s:%d</TD></TR>"
		      , zmw_file_get(), zmw_line_get()) ;
	  http_printf("<TR><TH>Asked Size</TH><TD>") ;
	  http_rectangle_display(zmw_asked_get()) ;
	  http_printf("</TD></TR>") ;
	  http_printf("<TR><TH>Required Size</TH><TD>") ;
	  http_rectangle_display(zmw_required_get()) ;
	  http_printf("</TD></TR>") ;
	  http_printf("<TR><TH>Min Size</TH><TD>") ;
	  http_rectangle_display(zmw_min_get()) ;
	  http_printf("</TD></TR>") ;
	  http_printf("<TR><TH>Allocated Size</TH><TD>") ;
	  http_rectangle_display(zmw_allocated_get()) ;
	  http_printf("</TD></TR>") ;

	  http_printf(
		  "<TR>"
		  "<TH>Expand</TH>"
		  "<TD>%s%s</TD>"
		  "</TR>\n"
		  , zmw_horizontal_expand_get() ? "Horizontal" : "&nbsp;"
		  , zmw_vertical_expand_get() ? "Vertical" : "&nbsp;"
	  ) ;

	  http_printf("<TR><TH>Debug</TH><TD>%d</TD></TR>\n"
		  , zmw_debug_get()) ;
	  http_printf("<TR><TH>Padding Width</TH><TD>%d</TD></TR>\n"
		  , zmw_padding_width_get()) ;
	  http_printf("<TR><TH>Border Width</TH><TD>%d</TD></TR>\n"
		  , zmw_border_width_get()) ;
	  http_printf("<TR><TH>Focus Width</TH><TD>%d</TD></TR>\n"
		  , zmw_focus_width_get()) ;
	  http_printf("<TR><TH>Focus</TH><TD>%s</TD></TR>\n"
		  , zmw_focus_get()
		  ? ( zmw_name_registered(zmw_focus_get())
		      ? zmw_name_registered(zmw_focus_get()) : "*focus=NULL" ) :
		  "focus = NULL"
		  ) ;
	  http_printf("<TR><TH>Font family</TH><TD>%s</TD></TR>\n"
		      , zmw_font_family_get()) ;
	  http_printf("<TR><TH>Font size</TH><TD>%d</TD></TR>\n"
		      , zmw_font_size_get()) ;
	  http_printf("<TR><TH>Font weight</TH><TD>%d</TD></TR>\n"
		      , zmw_font_weight_get()) ;
	  http_printf("<TR><TH>Font style</TH><TD>%d</TD></TR>\n"
		      , zmw_font_style_get()) ;
	  http_printf("<TR><TH>Window</TH><TD>%p</TD></TR>\n"
		  , *zmw_window_get()) ;
	  http_printf("<TR><TH>GC</TH><TD>") ;
	  http_printf("</TD></TR>") ;

	  http_printf("<TR><TH>Auto resize</TH><TD>%d</TD></TR>\n"
		      , zmw_auto_resize_get()) ;
	  http_printf("<TR><TH>Sensible</TH><TD>%d</TD></TR>\n"
		      , zmw_sensitive_get()) ;
	  http_printf("<TR><TH>Focused</TH><TD>%d</TD></TR>\n"
		      , zmw_focused_get()) ;
	  
	  http_printf("<TR><TH>Alignment</TH><TD>%s%s</TD></TR>\n"
		  , zmw_horizontal_alignment_get()<0 ? "Left" :
		  ( zmw_horizontal_alignment_get()>0 ? "Right" : "Centered" )
		  , zmw_vertical_alignment_get()<0 ? "Top" :
		  ( zmw_vertical_alignment_get()>0 ? "Down" : "Centered" )
		  ) ;
	  http_printf("<TR><TH>Expension</TH><TD>%s %s</TD></TR>\n"
		  , zmw_horizontal_expand_get() ? "Horizontal" : ""
		  , zmw_vertical_expand_get() ? "Vertical" : ""
		  ) ;
	  
	  http_printf("<TR><TH># of children</TH><TD>%d</TD></TR>\n"
		  , zmw_nb_of_children_get()) ;

	  http_printf("</TABLE>\n") ;


	  http_printf(
		  "<TABLE BORDER><TR>"
		  "<TH>Name</TH><TH>Asked size</TH>"
		  "<TH>Required Size</TH>"
		  "<TH>Min Size</TH>"
		  "<TH>Allocated size</TH>"
		  "<TH>Horizontal<BR>Alignment</TH>\n"
		  "<TH>Vertical<BR>Alignment</TH>\n"
		  "<TH>Expand</TH>\n"
		  ) ;
	  http_printf("<TH>Index</TH>\n") ;
	  http_printf("</TR>\n") ;
	}
      else
	if ( strncmp(global_name, zmw_name_full, strlen(global_name)) == 0
	     && strchr(zmw_name_full+strlen(global_name)+2, '/') == NULL)
	  {
	    http_printf("<TR><TH><A HREF=\"%s/\">%s</A></TH>\n"
		    , http_encode_url(zmw_name_full)
		    , zmw_name_get()
		    ) ;

	    http_size_display(zmw_size_get()) ;
	  }
	  
	  
      zmw_state_push() ;
      return(1) ;
    case 2:
      if ( strcmp(global_name, zmw_name_full) == 0 )
	http_printf("</TABLE>") ;
    }
  return(0) ;
}


int http_tree()
{
  ZMW_EXTERNAL_HANDLING ;
  
  switch ( (*zmw_call_number_get_ptr())++ )
    {
    case 0:
      zmw_subaction_set(Zmw_Compute_Children_Allocated_Size) ;
      return(zmw_action_first_pass()) ;

    case 1:
      zmw_subaction_set(Zmw_Debug_Message) ;
      http_printf("<LI> <A HREF=\"%s\">%s</A> %s:%d %s"
		  , http_encode_url(zmw_name_full)
		  , zmw_name_get()
		  , zmw_file_get()
		  , zmw_line_get()
		  , zmw_type_get()
	      ) ;
      http_printf("\n<UL>") ;
      zmw_state_push() ;
      return(1) ;
    case 2:
      http_printf("</UL></LI>\n") ;
      
    }
  return(0) ;
}

int http_table()
{
  int i ;
  int new_col, *visible, in_size ;
  char *name, *value ;

  ZMW_EXTERNAL_HANDLING ;
  
  switch ( (*zmw_call_number_get_ptr())++ )
    {
    case 0:
      zmw_subaction_set(Zmw_Compute_Children_Allocated_Size) ;
      return(zmw_action_first_pass()) ;

    case 1:
      zmw_subaction_set(Zmw_Input_Event) ;
      zmw_state_push() ;
      return(1) ;

    case 2:
      http_printf("<TR>\n") ;
      for(i=0; evaluate(i, &new_col, &name, &visible, &in_size, &value); i++)
	{
	  if ( *visible )
	    http_printf("    <TD>%s</TD>\n", value) ;
	}
      http_printf("</TR>\n") ;
      break ;
    }
  return(0) ;
}

int http_debug()
{
  ZMW_EXTERNAL_HANDLING ;
  

  switch ( (*zmw_call_number_get_ptr())++ )
    {
    case 0:
      zmw_subaction_set(Zmw_Compute_Children_Allocated_Size) ;
      return(zmw_action_first_pass()) ;

    case 1:
      zmw_state_push() ;
      return(1) ;
    }
  return(0) ;
}

static char *http_table_options(int sw)
{
  static char buf[999] ;
  int new_col, *visible, in_size ;
  char *name, *value ;
  int i ;

  buf[0] = '\0' ;
  for(i=0; evaluate(i, &new_col, &name, &visible, &in_size, &value); i++)
    if ( i == sw ? !*visible : *visible )
      sprintf(buf+strlen(buf), "/%d", i) ;

  return buf ;
}



void http_connection(gpointer o, int socket, GdkInputCondition condition)
{
  void (*fct)() = o ;
  int fildes ;
  char buf2[9999] ;
  int nb_name_in_path ;
  int i, j ;
  char *pc ;
  int new_col, *visible, in_size ;
  char *name ;
  char global_buf[9999] ;

  /*  fprintf(stderr, "socket=%d\n", socket) ; */
  fildes = socket_accept(socket) ;
  if ( fildes < 0 )
    perror("socket_accept") ;

  i = read(fildes, global_buf, sizeof(global_buf)-1) ;
  if ( strncmp(global_buf, "GET /", 5) == 0 )
    {
      global_buf[i] = '\0' ;

      pc = strchr(global_buf+5, ' ') ;
      if ( pc )
	*pc = '\0' ;

      pc = strchr(global_buf+5, '\r') ;
      if ( pc )
	*pc = '\0' ;

      pc = strchr(global_buf+5, '\n') ;
      if ( pc )
	*pc = '\0' ;

      pc = global_buf+5 ;
    }
  else
    pc = NULL ;

  if ( pc )
    {
      pc = http_decode_url(pc) ;
      strcpy(global_buf, pc) ;
      pc = global_buf ;

    }

  nb_name_in_path = 0 ;
  for(;;)
    {
      global_url[nb_name_in_path++] = pc ;
      if ( pc )
	{
	  pc = strchr(pc, '/') ;
	  if ( pc )
	    {
	      *pc++ = '\0' ;
	    }
	}
      else
	break ;
    }
  global_url[nb_name_in_path] = NULL ;


  if ( nb_name_in_path >= 2 )
    if ( global_url[nb_name_in_path-2][0] == '\0' )
      {
	nb_name_in_path-- ;
	global_url[nb_name_in_path-1] = NULL ;
      }

  if ( nb_name_in_path == 1 )
    {
      global_name = "?" ;
    }



  global_http = fdopen(fildes, "w") ;
  if ( global_http == NULL )
    perror("fdopen") ;

  http_printf(
          "HTTP/1.0 200 Document follows\n"
          "MIME-Version: 1.0\n"
          "Server: ZMW\n"
          "Pragma-Type: no-cache\n"
	  "Content-Type: text/html\n"
	  "\n"
	  "<HEAD>\n"
	  "<STYLE>\n"
	  "em { color: red ; }\n"
	  "span.name { font-size: smaller ; font-weight: light; }\n"
	  "</STYLE>\n"
	  "</HEAD>\n"
	  "<HTML>\n"
	  "<BODY>\n"
	  ) ;



   if ( strcmp(global_buf, "?tree") == 0 )
    {
      http_printf("<H1>Zmw Tree</H1>\n") ;

      zmw_call_widget(fct, http_tree) ;
    }
   else if ( strcmp(global_buf, "?names") == 0 )
    {
      http_printf("<H1>Zmw Names registration</H1>\n") ;

      zmw_name_dump(global_http) ;
    }
  else if ( strcmp(global_buf, "?drag") == 0 )
    {
      http_printf("<H1>Drag State</H1><PRE>") ;
      zmw_drag_debug(global_http) ;
      http_printf("</PRE>\n") ;      
    }
  else if ( strcmp(global_buf, "?table") == 0 )
    {
      GdkEvent e ;

      /*
       * URL parsing
       */
      for(i=0; evaluate(i, &new_col, &name, &visible, &in_size, NULL); i++)
	*visible = 0 ;
      for(i=1; global_url[i]; i++)
	{
	  if ( strcmp(global_url[i], "") == 0 )
	    {
	      i++ ;
	      break ;
	    }
	  j = atoi(global_url[i]) ;
	  if ( evaluate(j, &new_col, &name, &visible, &in_size, NULL) )
	    *visible = 1 ;
	}
      global_x      = global_url[i] ?   atoi(global_url[i++]) : 10 ;
      global_y      = global_url[i] ?   atoi(global_url[i++]) : 11 ;
      global_window = global_url[i] ? strtol(global_url[i++], NULL, 16) : 0 ;
      global_event  = global_url[i] ?   atoi(global_url[i++]) : 0 ;
      /*
       * False Event construction
       */
      zmw_zmw_event_set(&e) ;
      switch(global_event)
	{
	case 0 : zmw_zmw_event_get()->any.type = GDK_MOTION_NOTIFY ; break ;
	case 1 : zmw_zmw_event_get()->any.type = GDK_BUTTON_PRESS ;
	  zmw_zmw_button_pressed_set(Zmw_True) ;
	  break ;
	case 2 : zmw_zmw_event_get()->any.type = GDK_BUTTON_RELEASE ;
	  zmw_zmw_button_pressed_set(Zmw_False) ;
	  break ;
	}
      zmw_zmw_event_get()->motion.x = global_x ;
      zmw_zmw_event_get()->motion.y = global_y ;
      zmw_zmw_event_get()->any.window = (GdkWindow*)global_window ;
      zmw_zmw_window_set(zmw_zmw_event_get()->any.window) ;
      zmw_zmw_x_set(zmw_zmw_event_get()->motion.x) ;
      zmw_zmw_y_set(zmw_zmw_event_get()->motion.y) ;
      zmw_sensitived_set(Zmw_True) ;
      zmw_event_in_rectangle_set(Zmw_True) ;
      zmw_event_in_children_set(Zmw_True) ;
      zmw_window_set(NULL) ;

      http_printf("<H1>Widget table</H1>") ;
      http_printf("<p>The displayed table is from the event:\n") ;
      http_printf("<form name='f' action='javascript: window.location=\"?table%s//\" + document.f.x.value + \"/\" + document.f.y.value + \"/\" +  document.f.w.value + \"/\" +  document.f.e.value;'>", http_table_options(-1)) ;
      http_printf("X: <input name='x' value='%d' size='5'>", global_x) ;
      http_printf("Y: <input name='y' value='%d' size='5'>", global_y) ;
      http_printf("Win: <input name='w' value='%x' size='8'>", global_window) ;
      http_printf("Event: <select name='e' value='%d'>", global_event) ;
      http_printf("<option value='0'%s>Motion</option>"
		  , global_event == 0 ? "selected='1'" : "") ;
      http_printf("<option value='1'%s>Button Press</option>"
		  , global_event == 1 ? "selected='1'" : "") ;
      http_printf("<option value='2'%s>Button Release</option>"
		  , global_event == 2 ? "selected='1'" : "") ;
      http_printf("</select>") ;
      http_printf("<input type='submit' value='Send Event'>\n") ;
      http_printf("</form>") ;
      http_printf("<TABLE>\n<TR>\n") ;      
      for(i=0; evaluate(i, &new_col, &name, &visible, &in_size, NULL); i++)
	{
	  if ( new_col )
	    {
	      if ( i != 0 )
		http_printf("</span></TD>") ;
	      http_printf("<TD VALIGN='TOP'><span class='name'>") ;
	    }

	  http_printf("%c<a href=\"/?table%s//%d/%d/%x\">%s</a><br>\n"
		      , *visible ? '-' : '+'
		      , http_table_options(i)
		      , global_x
		      , global_y
		      , global_window
		      , name
		      ) ;
	}
      http_printf("</span></TD></TR></TABLE>\n") ;      
      http_printf("<TABLE border>") ;
      http_printf("<TR>\n") ;
      for(i=0; evaluate(i, &new_col, &name, &visible, &in_size, NULL); i++)
	{
	  if ( *visible )
	    {
	      http_printf("    <td valign='bottom'><span class='name'><a href=\"/?table%s//%d/%d/%x\">"
			  , http_table_options(i)
			  , global_x
			  , global_y
			  , global_window
			  ) ;
	      for(j=0; name[j]; j++)
		{
		  http_printf("%c", name[j]) ;
		  if ( name[j] == '.' || name[j] == '_' )
		    http_printf("<br>") ;
		}

	      http_printf("</a></span></td>\n") ;
	    }
	}
      http_printf("</TR>\n") ;
      zmw_call_widget(fct, http_table) ;
      http_printf("</TABLE>") ;      

      if ( global_event == 2 )
	zmw_event_button_release() ;
    }
  else
    {
      http_printf("<H1>") ;
      http_printf("<A HREF=\"/\">(root)</A>") ;
      strcpy(buf2, "/") ;
      for(i=0;global_url[i];i++)
	{
	  sprintf( buf2+strlen(buf2), "%s/", global_url[i]) ;
	  http_printf("/<A HREF=\"%s\">%s</A>"
		  , http_encode_url(buf2)
		  , global_url[i]
		  ) ;
	}
      http_printf("</H1>\n") ;
      
      
      zmw_name("?") ;
      if ( buf2[strlen(buf2)-1] == '/' )
	buf2[strlen(buf2)-1] = '\0' ;
      global_name = buf2 ;
      zmw_call_widget(fct, http_node) ;
      
    }

  http_printf(
	  "<HR>"
	  "<A HREF=\"/\">Root</A> * "
	  "<A HREF=\"/?tree\">Zmw Tree</A> * "
	  "<A HREF=\"/?table/1\">Zmw Table</A> * "
	  "<A HREF=\"/?names\">Zmw Names</A> * "
	  "<A HREF=\"/?drag\">Drag State</A>"
	  "</BODY>\n"
	  "</HTML>\n"
	  ) ;
  fclose(global_http) ;
}
