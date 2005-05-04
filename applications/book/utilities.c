/*
  ZMW: A Zero Memory Widget Library
  Copyright (C) 2003 Thierry EXCOFFIER

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

#include <stdlib.h>
#include <ctype.h>
#include "library.h"
#include <string.h>


char *line_read(FILE *file)
{
  char lig[999] ;

  if ( fgets(lig, sizeof(lig), file) == NULL )
    ABORT ;
  lig[strlen(lig)-1] = '\0' ;
  return strdup(lig) ;
}



char *lower_case(const char *t)
{
  char *s ;
  int i ;

  MALLOC(s, strlen(t)+1) ;
  for(i=0; t[i]; i++)
    s[i] = tolower(t[i]) ;
  s[i] = '\0' ;

  return s ;
}


int strcmp_caseless(const char *a, const char *b)
{
  char *aa, *bb ;
  int result ;

  aa = lower_case(a) ;
  bb = lower_case(b) ;
  result = strcmp(aa, bb) ;
  free(aa) ;
  free(bb) ;
  return result ;
}

const char *surname(const char *a)
{
  int i ;

  i = strlen(a) - 1 ;
  while( i>0 && a[i]!=' ')
    i-- ;

  if ( a[i] == ' ' )
    return a+i+1 ;
  else
    return a ;
}

Hash_Key hash_key(Hash_Key start, const char *string)
{
  while(*string)
    {
      if ( *string != '/' )
	start = start*532483 + *string ;
      string++ ;
    }
  return start ;
}
