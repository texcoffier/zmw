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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "circuit_application.h"

int number_of_inputs[] = { 0,2,2,1,2,0,0,0 } ;
char *names[] = {"Value", "And", "Or", "Not", "Xor", "Change_Input", "Delete", NULL } ;
char *icones[] = {"PNG/val.png", "PNG/and.png", "PNG/or.png", "PNG/not.png", "PNG/xor.png", "", "",  NULL } ;

void circuit_gate_eval(Circuit *c, int n, int start)
{
  int i ;
  Gate *g ;

  g = &c->gates[n] ;

  if ( g->value >= 0 )
    return ;

  for(i=0; i<number_of_inputs[g->type]; i++)
    {
      if ( g->input[i] >= 0 && g->input[i] != start )
	{
	  circuit_gate_eval(c, g->input[i], start) ;
	  if ( c->gates[g->input[i]].value != Undefined )
	    continue ;
	}
      g->value = Undefined ; /* Undefined */
      return ;
    }

  switch(g->type)
    {
    case Gate_And :
      g->value = c->gates[g->input[0]].value & c->gates[g->input[1]].value ;
      break;
    case Gate_Or :
      g->value = c->gates[g->input[0]].value | c->gates[g->input[1]].value ;
      break;
    case Gate_Xor :
      g->value = c->gates[g->input[0]].value ^ c->gates[g->input[1]].value ;
      break;
    case Gate_Not :
      g->value = !c->gates[g->input[0]].value ;
      break;
    case Gate_Deleted :
      break;
    default:
      abort() ;
    }  
}

void circuit_eval(Circuit *c)
{
  int i ;

  c->w = 0 ;
  c->h = 0 ;
  c->nb = 0 ;
  for(i=0; c->gates[i].type != Gate_EOT; i++)
    {
      if ( c->gates[i].type == Gate_Deleted )
	continue ;
      if ( c->gates[i].type != Gate_Value )
	c->gates[i].value = -1 ;
      if ( c->gates[i].x > c->w )
        c->w = c->gates[i].x + 100 ;
      if ( c->gates[i].y > c->h )
        c->h = c->gates[i].y ;
      c->nb++ ;
    }

  c->w += 500 ;
  c->h += 500 ;

  for(i=0; c->gates[i].type != Gate_EOT; i++)
    if ( c->gates[i].type != Gate_Value )
      circuit_gate_eval(c, i, i) ;
}

void circuit_do(Circuit *c, int direction)
{
  int cont ;

  do
    {
      if ( direction > 0 && c->history_position >= c->history_size )
	break ;
      if ( direction < 0 && c->history_position == 0 )
	break ;
      if ( direction > 0 )
	{
	  c->gates[c->history[c->history_position].gate_number] =
	    c->history[c->history_position].new ;
	  cont = !c->history[c->history_position].real ;
	}
      else
	{
	  c->gates[c->history[c->history_position-1].gate_number] =
	    c->history[c->history_position-1].old ;
	  cont = !c->history[c->history_position-2].real ;
	}
      
      c->history_position += direction ;
    }
  while( cont ) ;
  circuit_eval(c) ;
}

void circuit_clear_history(Circuit *c)
{
  int i ;

  for(i = c->history_position; i < c->history_size; i++)
    free( c->history[i].comment ) ;
  c->history_size = c->history_position ;
}

void circuit_add_to_history(Circuit *c, int gate_number, Gate *new_gate
			    , const char *format, ...)
{
  va_list ap;
  char buf[999] ;

  va_start(ap, format);
  vsprintf(buf, format, ap );
  va_end(ap);

  circuit_clear_history(c) ;

  c->history_size++ ;
  c->history = realloc(c->history, sizeof(*c->history)*c->history_size) ;
  c->history[c->history_position].comment = strdup(buf) ;
  c->history[c->history_position].old = c->gates[gate_number] ;
  c->history[c->history_position].new = *new_gate ;
  c->history[c->history_position].gate_number = gate_number ;
  c->history[c->history_position].real = True ;
  circuit_do(c,1) ;
}




void circuit_gate_delete(Circuit *c, int g)
{
  Gate new_gate ;
  int i, j, changed ;

  for(i=0;  c->gates[i].type != Gate_EOT; i++ )
    {
      new_gate = c->gates[i] ;
      changed = 0 ;
      for(j=0; j<number_of_inputs[new_gate.type]; j++)
	if ( new_gate.input[j] == g )
	  {
	    new_gate.input[j] = -1 ;
	    changed = 1 ;
	  }
      if ( changed )
	{
	  circuit_add_to_history(c, i, &new_gate, "Delete arc %d->%d", i, g) ;
	  c->history[c->history_position-1].real  = False ;
	}
    }
  
  new_gate.type = Gate_Deleted ;
  circuit_add_to_history(c, g, &new_gate, "Delete gate %d", g) ;
}

void circuit_gate_switch_state(Circuit *c, int i)
{
  Gate new_gate = c->gates[i] ;
  new_gate.value ^= 1 ;
  circuit_add_to_history(c, i,&new_gate,"Value of %d <- %d",i,new_gate.value) ;
}

void circuit_arc_add(Circuit *c, int input, int input_port, int output)
{
  Gate new_gate = c->gates[input] ;
  new_gate.input[input_port] = output ;
  circuit_add_to_history(c,input,&new_gate,"Create arc %d->%d", output,input) ;
}


void circuit_init(Circuit *c)
{
  c->gates = malloc( sizeof(*c->gates) ) ;
  c->gates[0].type = Gate_EOT ;
  c->history_size = 0 ;
  c->history_position = 0 ;
  c->history = malloc(0) ;
  c->filename = strdup("CIRCUITS/no_name.circuit") ;
}


void circuit_gate_add(Circuit *c, Type t, int x, int y)
{
  int i ;
  Gate new_gate = {.x=x,.y=y,.type=t,.value=False,.input={-1,-1}} ;

  for(i=0;  c->gates[i].type != Gate_EOT; i++ )
    if ( c->gates[i].type == Gate_Deleted )
      break ;
  if ( c->gates[i].type == Gate_EOT )
    {
      c->gates = realloc(c->gates, sizeof(*c->gates)*(i+2)) ;
      c->gates[i+1] = c->gates[i] ;
    }
  circuit_add_to_history(c,i,&new_gate,"Create %s @ (%d,%d)", names[t], x, y) ;
}

/*
 * Returns 0 if OK
 */
int circuit_save(Circuit *c)
{
  FILE *f ;
  int i ;

  f = fopen(c->filename, "wb") ;
  if ( f == NULL )
    return(1) ;
  i = 0 ;
  do
    if ( fwrite(&c->gates[i], sizeof(*c->gates), 1, f) != 1 )
      return(1) ;
  while( c->gates[i++].type != Gate_EOT ) ;
  if ( fclose(f) )
    return(1) ;

  return(0) ;
}

int circuit_load(Circuit *c)
{
  FILE *f ;
  int i ;

  f = fopen(c->filename, "rb") ;
  if ( f == NULL )
    return(1) ;
  i = 0 ;
  for(;;)
    {
      c->gates = realloc(c->gates, (i+1)*sizeof(*c->gates)) ;
      if ( fread(&(c->gates)[i++], sizeof(*c->gates), 1, f) != 1 )
	break ;
    }
  (c->gates)[i-1].type = Gate_EOT ;
  fclose(f) ;

  c->history_position = 0 ;

  circuit_clear_history(c) ;

  circuit_eval(c) ;
  return(0) ;
}

void circuit_snap_to_grid(Circuit *c)
{
  int i, nb ;
  Gate g ;

  nb = 0 ;
  for(i=0; c->gates[i].type != Gate_EOT; i++)
    {
      g = c->gates[i] ;
      g.x &= ~15 ;
      g.y &= ~15 ;
      if ( memcmp(&g, &c->gates[i], sizeof(Gate)) )
	{
	  circuit_add_to_history(c, i, &g, "Snap to grid %d", i) ;
	  c->history[c->history_position-1].real  = False ;
	  nb++ ;
	}
    }
  if ( nb )
    c->history[c->history_position-1].real = True ;
}
