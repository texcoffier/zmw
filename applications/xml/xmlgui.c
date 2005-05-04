// A real XML parser must be used in place of this FAKE one.

#include "zmw/zmw.h"
#include "xml_parser.h"

void xml_window(char *tag)
{
  zmw_window( xml_get_tag_attribute(tag, "title", "No Title") ) ;
}

void xml_box_vertical(char *tag)
{
  zmw_vbox() ;
}

void xml_box_horizontal(char *tag)
{
  zmw_hbox() ;
}

void xml_label(char *tag)
{
  char *p ;
  p = xml_get_tag_attribute(tag, "value", "A label") ;
  zmw_label(p) ;
} 

void xml_button(char *tag)
{
  char *p, *action ;
  p = xml_get_tag_attribute(tag, "value", "A button") ;
  zmw_button(p) ;
  action = xml_get_tag_attribute(tag, "action", "") ;
  if ( action[0] )
    if ( zmw_activated() )
      {
	printf("ACTION = %s\n", action) ;
      }
} 

void xml_text(char *tag)
{
  char **v ;
	  
  v = NULL ;
  zmw_resource_pointer_get((void**)&v, "TextValue", NULL) ;
  if ( *v == NULL )
    *v = strdup(xml_get_tag_attribute(tag, "value", "edit me")) ;
  
  zmw_entry(v) ;
}


static struct
{
  char *tag_name ;
  int has_children ;
  void (*function)(char *tag) ;
} global_tags[] =
  {
    { "window"         , 1, xml_window         },
    { "label"          , 0, xml_label          },
    { "box_vertical"   , 1, xml_box_vertical   },
    { "box_horizontal" , 1, xml_box_horizontal },
    { "text"           , 0, xml_text           },
    { "button"         , 0, xml_button         },
    { NULL }
  } ;



char *xml(char *txt)
{
  char *tag, *tag_name, *after, *before ;
  int i ;

  // For each tags at the same level
  for(;;)
    {
      before = txt ;
      /* Get the pointer on the tag content (if it is a begin tag) */
      tag = xml_get_tag(&txt) ;
      if ( tag == NULL )
	return txt ;

      after = NULL ;
      tag_name = xml_get_tag_name(tag) ;
      for(i=0; global_tags[i].tag_name; i++)
	if ( strcmp(tag_name, global_tags[i].tag_name) == 0 )
	  {
	    if ( global_tags[i].has_children )
	      ZMW( global_tags[i].function(tag) )
		after = xml(txt) ;
	    else
	      global_tags[i].function(tag) ;
	    break ;
	  }

      if ( after )
	txt = after ;
      else
	{
	  txt = before ;
	  xml_jump_tag(&txt) ;
	}
    }
}

void xml_top()
{
  static char *text = NULL ;

  if ( text == NULL )
    text = strdup
      (
       "<window title=Hello>\n" \
       "   <box_vertical>\n" \
       "      <label value=Iamalabel></label>\n" \
       "      <button value=Action!></button>\n" \
       "      <label value=another></label>\n" \
       "      <box_horizontal>\n" \
       "         <label value=X></label>\n" \
       "         <text value=editable></text>\n" \
       "         <box_vertical>\n" \
       "             <button value=PressMe action=Hello></button>\n" \
       "             <label value=Y></label>\n" \
       "             <button value=DontPressMe></button>\n" \
       "         </box_vertical>\n" \
       "      </box_horizontal>\n" \
       "   </box_vertical>\n" \
       "</window>"
       ) ;
  ZMW(zmw_window("XML source"))
    zmw_entry(&text) ;

  xml(text) ;
}

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(xml_top) ;
  return 0 ;
}
