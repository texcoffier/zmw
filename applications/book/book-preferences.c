/*
  ZMW: A Zero Memory Widget Library
  Copyright (C) 2003-2005 Thierry EXCOFFIER

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

#include <libxml/parser.h>
#include <errno.h>
#include "book.h"

static void color_save(FILE *f, const char *name, const float color[3])
{
  fprintf(f, "<color name='%s' r='%g' g='%g' b='%g' />\n",
	  name, color[0], color[1], color[2]) ;
}


void prefs_save(Library_GUI *gui, const char *filename)
{
  FILE *f ;
  int i ;

  f = fopen(filename, "w") ;
  if ( f == NULL )
    {
      gui->error_message[0] = "Error writing preference file:" ;
      gui->error_message[1] = (char*)filename ;
      gui->error_message[2] = strerror(errno) ;
      gui->error_message[3] = NULL ;
      return ;
    }
  fprintf(f, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n") ;
  fprintf(f, "<!DOCTYPE book-preferences SYSTEM \"book-preferences.dtd\">\n") ;
  fprintf(f, "<?xml-stylesheet type=\"text/xsl\" href=\"book-preferences.xsl\"?>\n") ;

  fprintf(f, "<book-preferences>\n") ;
  for(i=0; i<Column_Last; i++)
    {
      fprintf(f, "<column name='%s'", gui->prefs.cols[i].id) ;
      fprintf(f, " width='%d'", gui->prefs.cols[i].width) ;
      fprintf(f, " visible='%d'", gui->prefs.cols[i].visible) ;
      if ( i != Column_Borrower )
	fprintf(f, " default='%s'", gui->prefs.cols[i].default_value) ;
      fprintf(f, " />\n") ;
    }

  fprintf(f, "<column-order>\n") ;
  for(i=0; i<Column_Last; i++)
    {
      fprintf(f, "\t<column-name>%s</column-name>\n"
	      , gui->prefs.cols[gui->prefs.cols[i].order].id) ;
    }
  fprintf(f, "</column-order>\n") ;

  color_save(f, "standard", gui->prefs.standard_color ) ;
  color_save(f, "tip"     , gui->prefs.tip_color ) ;
  color_save(f, "header"  , gui->prefs.header_color ) ;
  color_save(f, "borrowed", gui->prefs.borrowed_color ) ;
  color_save(f, "menu"    , gui->prefs.menu_color ) ;


  fprintf(f, "<language name='%s' />\n", gui->prefs.language) ;

  fprintf(f, "</book-preferences>\n") ;

  fclose(f) ;
}

static void color_load(float *c, xmlNodePtr n)
{
  char *value ;

  value = xmlGetProp(n, "r") ;
  if ( value )
    c[0] = atof(value) ;
  value = xmlGetProp(n, "g") ;
  if ( value )
    c[1] = atof(value) ;
  value = xmlGetProp(n, "b") ;
  if ( value )
    c[2] = atof(value) ;
}

void prefs_load(Library_GUI *gui, const char *filename)
{
  xmlDocPtr doc ;
  xmlNodePtr n, nn ;
  int i, j ;
  char *name, *value ;

  doc = xmlParseFile(filename) ;
  for(n = xmlDocGetRootElement(doc)->xmlChildrenNode ; n; n = n->next)
    {
      if ( strcmp("column", n->name) == 0 )
	{
	  i = column_name_to_index(gui->prefs.cols, xmlGetProp(n, "name")) ;

	  value = xmlGetProp(n, "width") ;
	  if ( value )
	    gui->prefs.cols[i].width = atoi(value) ;

	  value = xmlGetProp(n, "visible") ;
	  if ( value )
	    gui->prefs.cols[i].visible = atoi(value) ;

	  value = xmlGetProp(n, "default") ;
	  if ( value )
	    {
	      free( gui->prefs.cols[i].default_value) ;
	      gui->prefs.cols[i].default_value = strdup(value) ;
	    }
	}
      else if ( strcmp("language", n->name) == 0 )
	{
	  gui->prefs.language = strdup(xmlGetProp(n, "name")) ;
	}
      else if ( strcmp("color", n->name) == 0 )
	{
	  name = xmlGetProp(n, "name") ;

	  if ( strcmp(name, "standard") == 0 )
	    color_load(gui->prefs.standard_color, n) ;
	  else if ( strcmp(name, "tip") == 0 )
	    color_load(gui->prefs.tip_color, n) ;
	  else if ( strcmp(name, "header") == 0 )
	    color_load(gui->prefs.header_color, n) ;
	  else if ( strcmp(name, "borrowed") == 0 )
	    color_load(gui->prefs.borrowed_color, n) ;
	  else if ( strcmp(name, "menu") == 0 )
	    color_load(gui->prefs.menu_color, n) ;
	}
      else if ( strcmp("column-order", n->name) == 0 )
	{
	  i = 0 ;
	  for(nn = n->xmlChildrenNode ; nn; nn = nn->next)
	    {
	      if ( strcmp(nn->name, "column-name") == 0 )
		{
		  j = column_name_to_index(gui->prefs.cols
					   ,xmlNodeGetContent(nn)) ;
		  gui->prefs.cols[i].order = j ;
		  i++ ;
		}
	    }
	}
      else if ( strcmp("text", n->name) == 0 )
	{
	}
      else
	fprintf(stderr, _("XML TAG UNKNOWN: %s\n"), n->name) ;
    }
  xmlFreeDoc(doc) ;
}
