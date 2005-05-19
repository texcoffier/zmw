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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "library.h"

Hash_Key strings_checksum(Strings *s)
{
  Hash_Key k ;
  int i ;

  k = s->number ;
  for(i=0; i<s->number; i++)
    k = hash_key(k, s->strings[i]) ;
  return k ;
}

int strings_changed(Strings *s)
{
  //  printf("%g %g %d\n", s->checksum, strings_checksum(s),s->checksum != strings_checksum(s)) ;
  return s->checksum != strings_checksum(s) ;
}

void strings_reset_checksum(Strings *s)
{
  s->checksum = strings_checksum(s) ;
}

void strings_read(FILE *file, Strings *s)
{
  int i ;

  if ( fscanf(file, "%d\n", &s->number) != 1 )
    ABORT ;
  MALLOC(s->strings, s->number) ;

  for(i=0; i < s->number; i++)
    {
      s->strings[i] = line_read(file) ;
    }
  strings_reset_checksum(s) ;
}

void strings_write(FILE *file, const Strings *s)
{
  int i ;

  fprintf(file, "%d\n", s->number) ;
  for(i=0; i < s->number; i++)
    {
      fprintf(file, "%s\n", s->strings[i]) ;
    }
}

int strings_add(Strings *s, char *newv)
{
  s->number++ ;
  REALLOC(s->strings, s->number) ;
  s->strings[s->number-1] = strdup(newv) ;
  return s->number-1 ;
}

void strings_free(Strings *s)
{
  int i ;

  for(i=0; i < s->number; i++)
      free(s->strings[i]) ;
  free(s->strings) ;
}

int strings_convol(const char *ta, const char *tb)
{
  int la, lb, l, i, j ;
  float s ;
  int c[2000] ;
  
  la = strlen(ta) ;
  if ( la==0 )
    return(0) ;
  lb = strlen(tb) ;
  if ( lb==0 ) return(0) ;

  l = la + lb -1 ;
  if ( l > sizeof(c) )
    abort() ;
  
  for(i=0;i<=l;i++) c[i] = 0 ;

  for(i=0;ta[i];i++)
    for( j=0; tb[j]; j++)
      if ( ta[i] == tb[j] )
	{
	  c[lb-1+i-j]++ ;
	}
    
  s = 0 ;
  for( i=0;i<=l;i++ )
    {
      if ( c[i] >= 1 )
	s += pow(c[i],2) ; // 1.7
    }

  return 10 * (s / la) / lb ;
}

int valued_compare(const void *a, const void *b)
{
  const Valued *aa = a ;
  const Valued *bb = b ;
  return bb->value - aa->value ;
}

int strings_search(const char *pin, Strings *straw, Valued *index, int nb)
{
  int i ;
  char *lower_case_pin ;
  Valued *valued_straw ;

  lower_case_pin = lower_case(pin) ;
  MALLOC(valued_straw, straw->number) ;
  for(i=0; i<straw->number; i++)
    {
      valued_straw[i].index = i ;
      valued_straw[i].value = strings_convol(lower_case_pin
					     , lower_case(straw->strings[i])
					     ) ;
    }
  qsort(valued_straw, straw->number, sizeof(*valued_straw), valued_compare) ;

  for(i=0; i<nb && i<straw->number; i++)
    {
      index[i] = valued_straw[i] ;
    }

  free(valued_straw) ;

  return i ;
}
