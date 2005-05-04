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


void many_widgets(const char *title)
{
  int i, j, k ;
  char c[2] ;
  char tmp[99], tmp2[99] ;
  static char *t[20][20][3] = {{{NULL}}} ;

  if ( t[0][0][0] == NULL )
    {
      for(j=0; j<10; j++)
	for(i=0; i<20; i++)
	  for(k=0; k<3; k++)
	    {
	      t[j][i][k] = malloc(2) ;
	      t[j][i][k][0] = ' ' + (rand()%(128-33)) ;
	      t[j][i][k][1] = '\0' ;
	    }
    }
  

  ZMW(zmw_window(title))
    {
      ZMW(zmw_vbox())
	{

	  c[1] = '\0' ;
	  for(j=0; j<10; j++)
	    {
	      sprintf(tmp, "Line=%d", j) ;
	      zmw_name(tmp) ;
	      ZMW(zmw_hbox())
		{
		  for(i=0; i<20; i++)
		    {
		      sprintf(tmp2, "Col=%d", i) ;
		      zmw_name(tmp2) ;
		      ZMW(zmw_vbox())
			{
			  for(k=0; k<3; k++)
			    {
			      zmw_entry(&t[j][i][k]) ;
			    }
			}
		    }
		}
	    }
	}
    }
}
