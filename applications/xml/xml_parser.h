/*
 * Créé le Sat Jul 31 21:52:56 2004 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_HOME_WIDGET_APPLICATIONS_XML_XML_PARSER_H
#define _HOME_EXCO_HOME_WIDGET_APPLICATIONS_XML_XML_PARSER_H

void xml_jump_tag(char **txt) ;
char *xml_get_tag_name(char *tag) ;
char * xml_get_tag_attribute(char *tag, char *name, char *default_value) ;
char* xml_get_tag(char **txt) ;


#endif
