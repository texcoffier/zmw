/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2004 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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
#include "zmw/socket.h"

static FILE *global_http ;
static char *global_url[ZMW_MAX_DEPTH] ;
static char *global_name = NULL ;
static int global_x, global_y, global_window, global_event ;
static Zmw_Size global_size ;


#define PTR(I) ((global_options[I].from_children \
                ? (char*)&ZMW_SIZE \
                : (char*)zMw \
               ) + global_options[I].offset)
#define GET(I, TYPE) *(TYPE*)(PTR(I))

#define OPTION_BIT(X) (global_option & ( (long long)1 << (X) ))
#define OPTION_BIT_SWITCH(X) (global_option ^ ( (long long)1 << (X) ))
#define OPTION_SET(X) (global_option = (X))

static Zmw_State global_zmw ;

static char global_buf[999] ;

struct options
{
  int new_column ;
  char *name ;
  char * (*get)(int i) ;
  int offset ;
  char from_children ;
  char set ; // If true option is asked
} ;

static struct options global_options[] ;

static char *get_full_name(int i)
{
  return zmw_name_full ;
}

static char *get_char(int i)
{
  char *t ;

  t = GET(i, char*) ;

  if ( strlen(t)+1 < sizeof(global_buf) )
    strcpy(global_buf, t) ;
  else
    sprintf(global_buf, "%.*s...", (int)(sizeof(global_buf)-1), t) ;
  return global_buf ;
}

static char *get_char_function_name(int i)
{
  char *t ;
  int j ;

  t = get_char(i) ;

  for(j=0; t[j]; j++)
    if ( t[j] == '(' )
      {
	t[j] = '\0' ;
	break ;
      }
  
  return t ;
}

static char *get_int(int i)
{
  int n ;

  n = GET(i, int) ;
  sprintf(global_buf, "%d", n) ;
  return global_buf ;
}


/*
 * This function reuses the others.
 * It is not nice because global_options is modified.
 */
static char *get_children(int i)
{
  int j, k ;
  static char *buf = NULL ;

  if ( zMw->u.nb_of_children == 0 )
    return "" ;

  buf = realloc(buf, 1) ;
  buf = strdup("<TABLE border><TR>") ;
  zMw++ ;

  buf = realloc(buf, strlen(buf) + 1000) ;
  
  for(k=0; global_options[k].name; k++)
    {
      if ( ! global_options[k].from_children )
	continue ; // not a size option
      if ( ! global_options[k].set )
	continue ; // User not interested
      sprintf(buf+strlen(buf), "<TH><SMALL><SMALL>%s</SMALL></SMALL></TH>", global_options[k].name) ;
    }
  sprintf(buf+strlen(buf), "</TR>") ;


  for(j=0; j<zMw[-1].u.nb_of_children; j++)
    {
      sprintf(buf+strlen(buf), "<TR>") ;
      ZMW_CHILD_NUMBER = j ;
      for(k=0; global_options[k].name; k++)
	{
	  if ( ! global_options[k].from_children )
	    continue ; // not a size option
	  if ( ! global_options[k].set )
	    continue ; // User not interested

          buf = realloc(buf, strlen(buf) + 1000) ;

	  sprintf(buf+strlen(buf), "<TD>%s</TD>", global_options[k].get(k)) ;
	}
      sprintf(buf + strlen(buf), "</TR>") ;
    }
  sprintf(buf + strlen(buf), "</TABLE>") ;
  zMw-- ;
    
  return buf ;
}

static char *get_pointer(int i)
{
  void *n ;

  n = GET(i, void*) ;
  sprintf(global_buf, "%p", n) ;
  return global_buf ;
}

static char *get_menu_state(int i)
{
  int *n ;

  n = GET(i, int*) ;
  if ( n )
    sprintf(global_buf, "%s%s%s"
	    , (*n & Zmw_Menu_Is_Detached) ? " Detached" : ""
	    , (*n & Zmw_Menu_Is_Poped) ? " Poped" : ""
	    , (*n & Zmw_Menu_Contains_A_Detached) ? " ContainsDetached" : ""
	    ) ;
  else
    return "NULL" ;
  return global_buf ;
}

static char *get_color(int i)
{
  int n ;
  Zmw_Float_0_1 r, g, b ;

  n = GET(i, int) ;
  zmw_int_to_rgb(n, &r, &g, &b) ;
  sprintf(global_buf, "%4.2f %4.2f %4.2f<span style='background: #%02x%02x%02x;'>X</span>", r, g, b, (int)(r*255), (int)(g*255), (int)(b*255)) ;
  return global_buf ;
}

static char *get_boolean(int i)
{
  if ( GET(i, char) )
    return "True" ;
  else
    return "False" ;
}

static char *get_event_to_process(int i)
{
  return zmw_event_to_process() ? "True" : "False" ;
}

static char *get_event_in(int i)
{
  return zmw_event_in() ? "True" : "False" ;
}

static char *get_horizontal_alignment(int i)
{
  static char *a[] = {"Left", "Center", "Right"} ;

  if ( GET(i,char) < -1 ||  GET(i,char) > 1 )
    return "BUG" ;

  return a[  GET(i,char) + 1 ] ;
}

static char *get_vertical_alignment(int i)
{
  static char *a[] = {"Top", "Center", "Bottom"} ;

  if ( GET(i,char) < -1 ||  GET(i,char) > 1 )
    return "BUG" ;

  return a[  GET(i,char) + 1 ] ;
}

static char* get_rectangle_(Zmw_Rectangle *ws)
{
  global_buf[0] = '\0' ;

  if ( ws->x != ZMW_VALUE_UNDEFINED )
    sprintf(global_buf+strlen(global_buf),"%d,", ws->x) ;
  else
    sprintf(global_buf+strlen(global_buf),"?,") ;

  if ( ws->y != ZMW_VALUE_UNDEFINED )
    sprintf(global_buf+strlen(global_buf),"%d", ws->y) ;
  else
    sprintf(global_buf+strlen(global_buf),"?") ;

  if ( ws->width != ZMW_VALUE_UNDEFINED )
    sprintf(global_buf+strlen(global_buf),"&nbsp;%d", ws->width) ;
  else
    sprintf(global_buf+strlen(global_buf),"&nbsp;?") ;

  if ( ws->height != ZMW_VALUE_UNDEFINED )
    sprintf(global_buf+strlen(global_buf),"x%d", ws->height) ;
  else
    sprintf(global_buf+strlen(global_buf),"x?") ;

  return global_buf ;
}

static char* get_rectangle(int i)
{
  return get_rectangle_( &GET(i,Zmw_Rectangle) ) ;
}

static char* zmw_http_debug(int i)
{
  sprintf(global_buf, "Your debug message") ;

  return global_buf ;
}

#define OFFSET(X)  ((char*)&global_zmw.X - (char*)&global_zmw),0
#define OPTION(X,T) #X, get_ ## T, OFFSET(X)
#define OFFSET_SIZE(X)  ((char*)&global_size.X - (char*)&global_size),1
#define OPTION_SIZE(X,T) #X, get_ ## T, OFFSET_SIZE(X)

static char *get_zmw(int i)
{
  global_buf[0] = '\0' ;
  if ( zmw.external_do_not_make_init )
    strcat(global_buf, " external_do_not_make_init") ;
  if ( zmw.event )
    {
      sprintf(global_buf+strlen(global_buf), " event:%p", zmw.event) ;
      if ( zmw.event->any.window )
	sprintf(global_buf+strlen(global_buf), " event.window:%p"
		, zmw.event->any.window) ;
    }
  if ( zmw.event_removed )
    strcat(global_buf, " event_removed") ;
  sprintf(global_buf+strlen(global_buf), " %d[%d]%d[%d]"
	  , zmw.x, zmw.x_root
	  , zmw.y, zmw.y_root
	  ) ;
  if ( zmw.window )
    sprintf(global_buf+strlen(global_buf), " window:%p", zmw.window) ;

  if ( zmw.key_pressed )
    strcat(global_buf, " key_pressed") ;
  if ( zmw.button_pressed )
    strcat(global_buf, " button_pressed") ;
  if ( zmw.still_yet_displayed )
    strcat(global_buf, " still_yet_displayed") ;
  if ( zmw.tips_yet_displayed )
    strcat(global_buf, " tips_yet_displayed") ;
  if ( zmw_name_registered(&zmw.found) )
    {
      strcat(global_buf, " found:") ;
      strcat(global_buf, zmw.found.name) ;
    }
  if ( zmw_name_registered(&zmw.tip_displayed) )
    {
      strcat(global_buf, " tip_displayed:") ;
      strcat(global_buf, zmw.found.name) ;
    }

  return global_buf ;
}

static struct options global_options[] =
  {
    { 1, "namefull" , get_full_name, OFFSET(u.name) },
    { 0, OPTION(u.type                               , char                ) },
    { 0, "short type", get_char_function_name, OFFSET(u.type)                },
    { 0, OPTION(u.file                               , char                ) },
    { 0, OPTION(u.line                               , int                 ) },
    { 0, OPTION(u.name                               , char                ) },
    { 0, OPTION(u.name_index                         , char                ) },
    { 0, OPTION(u.call_number                        , int                 ) },
    { 0, "action", (char*(*)(int))zmw_action_name_fct, OFFSET(i.action)      },
    { 0, OPTION(u.nb_of_children                     , int                 ) },
    { 0, OPTION(u.nb_of_children_max                 , int                 ) },
    { 0, OPTION(u.font_copy_on_write                 , boolean             ) },
    { 0, OPTION(u.name_separator                     , int                 ) },
    { 0, OPTION(u.menu_state                         , menu_state          ) },
    { 0, OPTION(u.children                           , children            ) },
    { 0, OPTION(u.asked                         , rectangle           ) },
    { 1, OPTION_SIZE(min                           , rectangle           ) },
    { 0, OPTION_SIZE(required                      , rectangle           ) },
    { 0, OPTION_SIZE(allocated                , rectangle           ) },
    { 0, OPTION_SIZE(used_to_compute_parent_size   , boolean             ) },
    { 0, OPTION_SIZE(hash                          , int                 ) },
    { 0, OPTION_SIZE(current_state.horizontal_expand   , boolean            )},
    { 0, OPTION_SIZE(current_state.vertical_expand     , boolean            )},
    { 0, OPTION_SIZE(current_state.horizontal_alignment,horizontal_alignment)},
    { 0, OPTION_SIZE(current_state.vertical_alignment  , vertical_alignment )},
    { 0, OPTION_SIZE(horizontal_expand             , boolean            )},
    { 0, OPTION_SIZE(vertical_expand               , boolean            )},
    { 0, OPTION_SIZE(event_in_rectangle            , boolean             ) },
    { 0, OPTION_SIZE(event_in_children             , boolean             ) },
    { 0, OPTION_SIZE(invisible                     , boolean             ) },
    { 0, OPTION_SIZE(sensible                      , boolean             ) },
    { 0, OPTION_SIZE(focused                       , boolean             ) },
    { 0, OPTION_SIZE(activated                     , boolean             ) },
    { 0, OPTION_SIZE(child_activated               , boolean             ) },
    { 0, OPTION_SIZE(changed                       , boolean             ) },
    { 0, OPTION_SIZE(tip_visible                   , boolean             ) },
    { 0, OPTION_SIZE(do_not_map_window             , boolean             ) },
    { 0, OPTION_SIZE(pass_through                  , boolean             ) },
    { 0, OPTION_SIZE(current_state.padding_width   , int                 ) },
    { 1, OPTION(i.debug                              , int                 ) },
    { 0, OPTION(i.border_width                       , int                 ) },
    { 0, OPTION(i.focus_width                        , int                 ) },
    { 0, OPTION(i.focus                              , pointer             ) },
    { 0, OPTION(i.colors[Zmw_Color_Background_Normal], color               ) },
    { 0, OPTION(i.colors[Zmw_Color_Background_Pushed], color               ) },
    { 0, OPTION(i.colors[Zmw_Color_Background_Poped] , color               ) },
    { 0, OPTION(i.colors[Zmw_Color_Border_Light]     , color               ) },
    { 0, OPTION(i.colors[Zmw_Color_Border_Dark]      , color               ) },
    { 0, OPTION(i.colors[Zmw_Color_Foreground]       , color               ) },
    { 0, OPTION(i.auto_resize                        , boolean             ) },
    { 0, OPTION(i.sensible                           , boolean             ) },
    { 1, OPTION(i.font                               , pointer             ) },
    { 0, OPTION(u.parent_to_child.window             , pointer             ) },
    { 0, OPTION(u.parent_to_child.gc                 , pointer             ) },
    { 0, OPTION(u.parent_to_child.clipping           , rectangle           ) },
    { 0, OPTION(i.event_in_masked                    , boolean             ) },
    { 0, OPTION(i.event_in_focus                     , boolean             ) },
    //    { 0, OPTION(i.event_in                           , boolean             ) },
    { 0, "subaction",(char*(*)(int))zmw_action_name, OFFSET(u.subaction)     },
    { 0, "zmw_http_debug",zmw_http_debug, OFFSET(u.subaction)                },
    { 0, "event_to_process", get_event_to_process, OFFSET(u.type)            },
    { 0, "event_in", get_event_in, OFFSET(u.type)            },
    { 0, "zmw", get_zmw, OFFSET(u.type)            },
    { 0, NULL }
  } ;






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
    i = vfprintf(global_http, format, ap );
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

static void http_size_display(Zmw_Size *ws)
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
	  , ZMW_SIZE_HORIZONTAL_ALIGNMENT < 0
	  ? "Left" : (ZMW_SIZE_HORIZONTAL_ALIGNMENT > 0 ? "Right" : "Center")
	  , ZMW_SIZE_VERTICAL_ALIGNMENT < 0
	  ? "Up" : (ZMW_SIZE_VERTICAL_ALIGNMENT > 0 ? "Down" : "Center")
	  , ZMW_HORIZONTAL_EXPAND ? "Horizontal" : ""
	  , ZMW_VERTICAL_EXPAND ? "Vertical" : ""
	  , ZMW_USED_TO_COMPUTE_PARENT_SIZE ? "" :"NotUsedToComputeParentSize"
	  ) ;
  http_printf("<TD>%s</TD>\n", ZMW_SIZE_SENSIBLE ? "Sensible" : "") ;
}

int http_node()
{
  ZMW_EXTERNAL_HANDLING ;
  
  switch ( ZMW_CALL_NUMBER++ )
    {
    case 0:
      ZMW_SUBACTION = Zmw_Compute_Children_Allocated_Size ;
      return( zmw_action_first_pass() ) ;

    case 1:
      zmw_action_second_pass() ;
      ZMW_SUBACTION = Zmw_Nothing ;

      if ( strcmp(global_name, zmw_name_full) == 0 )
	{
	  http_printf("<TABLE BORDER>\n") ;
	  http_printf("<TR><TH>Type</TH><TD>%s</TD></TR>", ZMW_TYPE) ;
	  http_printf("<TR><TH>Declaration</TH><TD>%s:%d</TD></TR>"
		      , ZMW_FILE, ZMW_LINE) ;
	  http_printf("<TR><TH>Asked Size</TH><TD>") ;
	  http_rectangle_display(&ZMW_ASKED) ;
	  http_printf("</TD></TR>") ;
	  http_printf("<TR><TH>Required Size</TH><TD>") ;
	  http_rectangle_display(&ZMW_SIZE_REQUIRED) ;
	  http_printf("</TD></TR>") ;
	  http_printf("<TR><TH>Min Size</TH><TD>") ;
	  http_rectangle_display(&ZMW_SIZE_MIN) ;
	  http_printf("</TD></TR>") ;
	  http_printf("<TR><TH>Allocated Size</TH><TD>") ;
	  http_rectangle_display(&ZMW_SIZE_ALLOCATED) ;
	  http_printf("</TD></TR>") ;

	  http_printf(
		  "<TR>"
		  "<TH>Expand</TH>"
		  "<TD>%s%s</TD>"
		  "</TR>\n"
		  , ZMW_HORIZONTAL_EXPAND ? "Horizontal" : "&nbsp;"
		  , ZMW_VERTICAL_EXPAND ? "Vertical" : "&nbsp;"
	  ) ;

	  http_printf("<TR><TH>Debug</TH><TD>%d</TD></TR>\n"
		  , ZMW_DEBUG) ;
	  http_printf("<TR><TH>Padding Width</TH><TD>%d</TD></TR>\n"
		  , ZMW_SIZE_PADDING_WIDTH) ;
	  http_printf("<TR><TH>Border Width</TH><TD>%d</TD></TR>\n"
		  , ZMW_BORDER_WIDTH) ;
	  http_printf("<TR><TH>Focus Width</TH><TD>%d</TD></TR>\n"
		  , ZMW_FOCUS_WIDTH) ;
	  http_printf("<TR><TH>Focus</TH><TD>%s</TD></TR>\n"
		  , ZMW_FOCUS
		  ? ( zmw_name_registered(ZMW_FOCUS)
		      ? zmw_name_registered(ZMW_FOCUS) : "*focus=NULL" ) :
		  "focus = NULL"
		  ) ;
	  http_printf("<TR><TH>Font</TH><TD>%p%s</TD></TR>\n"
		  , ZMW_FONT
		  , ZMW_FONT_COPY_ON_WRITE ? "" : "Modified") ;
	  http_printf("<TR><TH>Window</TH><TD>%p</TD></TR>\n"
		  , *ZMW_WINDOW) ;
	  http_printf("<TR><TH>GC</TH><TD>") ;
	  http_printf("</TD></TR>") ;

	  http_printf("<TR><TH>Auto resize</TH><TD>%d</TD></TR>\n"
		  , ZMW_AUTO_RESIZE) ;
	  http_printf("<TR><TH>Sensible</TH><TD>%d</TD></TR>\n"
		  , ZMW_SENSIBLE) ;
	  http_printf("<TR><TH>Focused</TH><TD>%d</TD></TR>\n"
		  , ZMW_SIZE_FOCUSED) ;
	  
	  http_printf("<TR><TH>Alignment</TH><TD>%s%s</TD></TR>\n"
		  , ZMW_SIZE_HORIZONTAL_ALIGNMENT<0 ? "Left" :
		  ( ZMW_SIZE_HORIZONTAL_ALIGNMENT>0 ? "Right" : "Centered" )
		  , ZMW_SIZE_VERTICAL_ALIGNMENT<0 ? "Top" :
		  ( ZMW_SIZE_VERTICAL_ALIGNMENT>0 ? "Down" : "Centered" )
		  ) ;
	  http_printf("<TR><TH>Expension</TH><TD>%s %s</TD></TR>\n"
		  , ZMW_HORIZONTAL_EXPAND ? "Horizontal" : ""
		  , ZMW_VERTICAL_EXPAND ? "Vertical" : ""
		  ) ;
	  
	  http_printf("<TR><TH># of children</TH><TD>%d</TD></TR>\n"
		  , ZMW_NB_OF_CHILDREN) ;

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
		    , ZMW_NAME
		    ) ;

	    http_size_display(&ZMW_SIZE) ;
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
  
  switch ( ZMW_CALL_NUMBER++ )
    {
    case 0:
      ZMW_SUBACTION = Zmw_Compute_Children_Allocated_Size ;
      return(zmw_action_first_pass()) ;

    case 1:
      ZMW_SUBACTION = Zmw_Nothing ;
      ZMW_SUBACTION = Zmw_Debug_Message ;
      zmw_action_second_pass() ;
      http_printf("<LI> <A HREF=\"%s\">%s</A> %s:%d %s"
		  , http_encode_url(zmw_name_full)
		  , ZMW_NAME
		  , ZMW_FILE
		  , ZMW_LINE
		  , ZMW_TYPE
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

  ZMW_EXTERNAL_HANDLING ;
  
  switch ( ZMW_CALL_NUMBER++ )
    {
    case 0:
      ZMW_SUBACTION = Zmw_Compute_Children_Allocated_Size ;
      return(zmw_action_first_pass()) ;

    case 1:
      zmw_action_second_pass() ;
      ZMW_SUBACTION = Zmw_Input_Event ;
      zmw_state_push() ;
      return(1) ;

    case 2:
      http_printf("<TR>\n") ;
      for(i=0; global_options[i].name; i++)
	{
	  if ( global_options[i].set )
	    {
	      http_printf("    <TD>%s</TD>\n", global_options[i].get(i)) ;
	    }
	}
      http_printf("</TR>\n") ;
      break ;
    }
  return(0) ;
}

int http_debug()
{
  ZMW_EXTERNAL_HANDLING ;
  

  switch ( ZMW_CALL_NUMBER++ )
    {
    case 0:
      ZMW_SUBACTION = Zmw_Compute_Children_Allocated_Size ;
      return(zmw_action_first_pass()) ;

    case 1:

      zmw_action_second_pass() ;
      zmw_state_push() ;
      return(1) ;
    }
  return(0) ;
}

static char *http_table_options(int sw)
{
  static char buf[999] ;
  int i ;

  buf[0] = '\0' ;
  for(i=0; global_options[i].name; i++)
    if ( i == sw ? !global_options[i].set : global_options[i].set )
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
      for(i=0; global_options[i].name; i++)
	global_options[i].set = 0 ;
      for(i=1; global_url[i]; i++)
	{
	  if ( strcmp(global_url[i], "") == 0 )
	    {
	      i++ ;
	      break ;
	    }
	  j = atoi(global_url[i]) ;
	  if ( j>=0 && j<ZMW_TABLE_SIZE(global_options) )
	    global_options[j].set = 1 ;
	}
      global_x      = global_url[i] ?   atoi(global_url[i++]) : 10 ;
      global_y      = global_url[i] ?   atoi(global_url[i++]) : 11 ;
      global_window = global_url[i] ? strtol(global_url[i++], NULL, 16) : 0 ;
      global_event  = global_url[i] ?   atoi(global_url[i++]) : 0 ;
      /*
       * False Event construction
       */
      zmw.event = &e ;
      switch(global_event)
	{
	case 0 : zmw.event->any.type = GDK_MOTION_NOTIFY ; break ;
	case 1 : zmw.event->any.type = GDK_BUTTON_PRESS ;
	  zmw.button_pressed = Zmw_True ;
	  break ;
	case 2 : zmw.event->any.type = GDK_BUTTON_RELEASE ;
	  zmw.button_pressed = Zmw_False ;
	  break ;
	}
      zmw.event->motion.x = global_x ;
      zmw.event->motion.y = global_y ;
      zmw.event->any.window = (GdkWindow*)global_window ;
      zmw.window = zmw.event->any.window ;
      zmw.x = zmw.event->motion.x ;
      zmw.y = zmw.event->motion.y ;
      ZMW_SIZE_SENSIBLE = Zmw_True ;
      ZMW_SIZE_EVENT_IN_RECTANGLE = Zmw_True ;
      ZMW_SIZE_EVENT_IN_CHILDREN = Zmw_True ;
      ZMW_WINDOW = NULL ;

      



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
      for(i=0; global_options[i].name; i++)
	{
	  if ( global_options[i].new_column )
	    {
	      if ( i != 0 )
		http_printf("</span></TD>") ;
	      http_printf("<TD VALIGN='TOP'><span class='name'>") ;
	    }

	  http_printf("%c<a href=\"/?table%s//%d/%d/%x\">%s</a><br>"
		      , global_options[i].set ? '-' : '+'
		      , http_table_options(i)
		      , global_x
		      , global_y
		      , global_window
		      , global_options[i].name
		      ) ;
	}
      http_printf("</span></TD></TR></TABLE>\n") ;      
      http_printf("<TABLE border>") ;
      http_printf("<TR>\n") ;
      for(i=0; global_options[i].name; i++)
	{
	  if ( global_options[i].set )
	    {
	      http_printf("    <td valign='bottom'><span class='name'><a href=\"/?table%s//%d/%d/%x\">"
			  , http_table_options(i)
			  , global_x
			  , global_y
			  , global_window
			  ) ;
	      if ( global_options[i].get == get_int
		   || global_options[i].get == get_boolean
		   || global_options[i].get == get_color
		   || global_options[i].get == get_vertical_alignment
		   || global_options[i].get == get_horizontal_alignment
		   || global_options[i].get == get_event_to_process
		   || global_options[i].get == get_event_in
		   )
		for(j=0; global_options[i].name[j]; j++)
		  {
		    http_printf("%c", global_options[i].name[j]) ;
		    if ( global_options[i].name[j] == '.'
			 || global_options[i].name[j] == '_'
			 )
		      http_printf("<br>") ;
		  }
	      else
		http_printf("%s", global_options[i].name) ;

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
