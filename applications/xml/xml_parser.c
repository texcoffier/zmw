#include "xml_parser.h"
#include <string.h>

/*
 * These functions do not really works.
 * Even on the most simple XML
 */

/*
 * "txt" is positionned after the first "c" encountered.
 */
int jump_after_impossible(char **txt, char c)
{
  while( **txt && **txt != c )
    (*txt)++ ;
  if ( **txt )
    {
      (*txt)++ ;
      return 0 ;
    }
  else
    {
      return 1 ;
    }
}

/*
 * jump over <tag>.....</tag>
 */
void xml_jump_tag(char **txt)
{
  // Jump over start tag
  if ( jump_after_impossible(txt, '>') )
    {
      return ;
    }

  for(;;)
    {
      if ( jump_after_impossible(txt, '<') )
	return ;
      if ( **txt == '/' )
	{
	  // jump after end tag
	  jump_after_impossible(txt, '>') ;
	  return ;
	}
      (*txt)-- ; // On the < of start tag
      xml_jump_tag(txt) ;
    }
}

/*
 * Set "txt" after the next "<ELEMENT>" or "</ELEMENT>"
 * Returns NULL if it is an end element.
 * Or return "ELEMENT" if it is a begin element.
 */
char *xml_get_tag(char **txt)
{
  char *tag ;

  if ( jump_after_impossible(txt, '<') )
    return NULL ;
  tag = *txt ;
  jump_after_impossible(txt, '>') ;

  if ( tag[0] == '/' )
      return NULL ;
  return tag ;
}

char *xml_get_tag_name(char *tag)
{
  static char name[99] ;
  char *p ;

  p = name ;
  while( *tag && *tag != ' ' && *tag != '>' )
    *p++ = *tag++ ;
  *p++ = '\0' ;

  return name ;
}

// Real garbage : this function does not parse correctly
char * xml_get_tag_attribute(char *tag, char *name, char *default_value)
{
  char *p, *v ;
  static char value[99] ;

  p = strstr(tag, name) ;
  if ( p == NULL )
    return default_value ; // Attribut not defined
  if ( p > strchr(tag, '>') ) 
    return default_value ; // Attribute outside the tag


  p += strlen(name) ;
  if ( *p != '=' )
    return default_value ;
  p++ ;

  v = value ;
  while( *p && *p != ' ' && *p != '>' )
    *v++ = *p++ ;
  *v++ = '\0' ;

  return value ;
}
