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

#include <ctype.h>
#include "zmw/zmw.h"
#include "zmw/socket.h"

static FILE *global_http ;
static char *global_url[ZMW_MAX_DEPTH] ;
static char *global_name = NULL ;

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
  http_rectangle_display(&ws->asked) ;
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
	  , ZMW_HORIZONTAL_ALIGNMENT < 0
	  ? "Left" : (ZMW_HORIZONTAL_ALIGNMENT > 0 ? "Right" : "Center")
	  , ZMW_VERTICAL_ALIGNMENT < 0
	  ? "Up" : (ZMW_VERTICAL_ALIGNMENT > 0 ? "Down" : "Center")
	  , ZMW_SIZE_HORIZONTAL_EXPAND ? "Horizontal" : ""
	  , ZMW_SIZE_VERTICAL_EXPAND ? "Vertical" : ""
	  , ZMW_USED_TO_COMPUTE_PARENT_SIZE ? "" :"NotUsedToComputeParentSize"
	  ) ;
  http_printf("<TD>%d</TD>\n", ws->index) ;
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
	  http_rectangle_display(&ZMW_SIZE_ASKED) ;
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
		  , ZMW_SIZE_HORIZONTAL_EXPAND ? "Horizontal" : "&nbsp;"
		  , ZMW_SIZE_VERTICAL_EXPAND ? "Vertical" : "&nbsp;"
	  ) ;

	  http_printf("<TR><TH>Debug</TH><TD>%d</TD></TR>\n"
		  , ZMW_DEBUG) ;
	  http_printf("<TR><TH>Padding Width</TH><TD>%d</TD></TR>\n"
		  , ZMW_PADDING_WIDTH) ;
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
		  , ZMW_WINDOW) ;
	  http_printf("<TR><TH>GC</TH><TD>") ;
	  http_printf("</TD></TR>") ;

	  http_printf("<TR><TH>Auto resize</TH><TD>%d</TD></TR>\n"
		  , ZMW_AUTO_RESIZE) ;
	  http_printf("<TR><TH>Sensible</TH><TD>%d</TD></TR>\n"
		  , ZMW_SENSIBLE) ;
	  
	  http_printf("<TR><TH>Alignment</TH><TD>%s%s</TD></TR>\n"
		  , ZMW_SIZE_HORIZONTAL_ALIGNMENT<0 ? "Left" :
		  ( ZMW_SIZE_HORIZONTAL_ALIGNMENT>0 ? "Right" : "Centered" )
		  , ZMW_SIZE_VERTICAL_ALIGNMENT<0 ? "Top" :
		  ( ZMW_SIZE_VERTICAL_ALIGNMENT>0 ? "Down" : "Centered" )
		  ) ;
	  http_printf("<TR><TH>Expension</TH><TD>%s %s</TD></TR>\n"
		  , ZMW_SIZE_HORIZONTAL_EXPAND ? "Horizontal" : ""
		  , ZMW_SIZE_VERTICAL_EXPAND ? "Vertical" : ""
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
  zmw_action_do_not_enter() ;
  return(0) ;
}


int http_tree()
{
  static int index_last = 0 ;
	
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
      http_printf("<LI> [%d-%d-%d] <A HREF=\"%s\">%s</A> %s:%d %s"
		  , ZMW_INDEX
		  , ZMW_SIZE_INDEX
		  , zmw.index_last
		  , zmw_name_full
		  , ZMW_NAME
		  , ZMW_FILE
		  , ZMW_LINE
		  , ZMW_TYPE
	      ) ;
      if ( ZMW_SIZE_INDEX )
      	{
      if ( ZMW_SIZE_INDEX != zmw.index_last + 1 )
      	  http_printf("<BR>\n<EM>ZMW_SIZE_INDEX != zmw.index_last + 1</EM>") ;
      if ( ZMW_INDEX != index_last + 1 )
      	  http_printf("<BR>\n<EM>ZMW_INDEX != index_last + 1</EM>") ;
      	}
      index_last = ZMW_INDEX ;
      http_printf("\n<UL>") ;
      zmw_state_push() ;
      return(1) ;
    case 2:
      http_printf("</UL></LI>\n") ;
      
    }
  zmw_action_do_not_enter() ;
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
  zmw_action_do_not_enter() ;
  return(0) ;
}


void http_connection(gpointer o, int socket, GdkInputCondition condition)
{
  void (*fct)() = o ;
  int fildes ;
  char buf[9999] ;
  char buf2[9999] ;
  int nb_name_in_path ;
  int i ;
  char *pc ;

  /*  fprintf(stderr, "socket=%d\n", socket) ; */
  fildes = socket_accept(socket) ;
  if ( fildes < 0 )
    perror("socket_accept") ;

  i = read(fildes, buf, sizeof(buf)-1) ;
  if ( strncmp(buf, "GET /", 5) == 0 )
    {
      buf[i] = '\0' ;

      pc = strchr(buf+5, ' ') ;
      if ( pc )
	*pc = '\0' ;

      pc = strchr(buf+5, '\r') ;
      if ( pc )
	*pc = '\0' ;

      pc = strchr(buf+5, '\n') ;
      if ( pc )
	*pc = '\0' ;

      pc = buf+5 ;
    }
  else
    pc = NULL ;

  if ( pc )
    {
      pc = http_decode_url(pc) ;
      strcpy(buf, pc) ;
      pc = buf ;

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
	  "</STYLE>\n"
	  "</HEAD>\n"
	  "<HTML>\n"
	  "<BODY>\n"
	  ) ;


   if ( strcmp(buf, "?tree") == 0 )
    {
      http_printf("<H1>Zmw Tree</H1>\n") ;

      zmw_call_widget(fct, http_tree) ;
    }
   else if ( strcmp(buf, "?names") == 0 )
    {
      http_printf("<H1>Zmw Names registration</H1>\n") ;

      zmw_name_dump(global_http) ;
    }
  else if ( strcmp(buf, "?drag") == 0 )
    {
      http_printf("<H1>Drag State</H1><PRE>") ;
      zmw_drag_debug(global_http) ;
      http_printf("</PRE>\n") ;      
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
	  "<A HREF=\"/?names\">Zmw Names</A> * "
	  "<A HREF=\"/?drag\">Drag State</A>"
	  "</BODY>\n"
	  "</HTML>\n"
	  ) ;
  fclose(global_http) ;
}
