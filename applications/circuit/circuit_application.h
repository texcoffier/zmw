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

/*
 * Créé le Sat Jul 20 22:16:10 2002 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_HOME_WIDGET_DEMOS_CIRCUIT_APPLICATION_H
#define _HOME_EXCO_HOME_WIDGET_DEMOS_CIRCUIT_APPLICATION_H

typedef enum { Gate_Value, Gate_And, Gate_Or, Gate_Not ,Gate_Xor
	       , Change_Input, Gate_Delete, Gate_Deleted, Gate_EOT} Type ;

extern int number_of_inputs[] ;
extern char *names[] ;
extern char *icones[] ;

enum { False, True, Undefined } ;

typedef struct
{
  Type type ;
  int input[2] ; /* -1 if not connected */
  int x, y ;
  int w, h ;
  int value ;
} Gate ;

typedef struct
{
  Gate *gates ;
  int nb ;

  int w, h ;

  int history_size ;
  int history_position ;

  char *filename ;

  struct
  {
    char *comment ;
    int gate_number ;
    int real ;
    Gate old ;
    Gate new ;
  } *history ;

} Circuit ;


void circuit_init             (Circuit *c) ;
void circuit_eval             (Circuit *c) ;
int circuit_save             (Circuit *c) ;
int circuit_load             (Circuit *c) ;
void circuit_snap_to_grid     (Circuit *c) ;

void circuit_gate_delete      (Circuit *c, int n) ;
void circuit_gate_switch_state(Circuit *c, int n) ;

void circuit_arc_add(Circuit *c, int input, int input_port, int output) ;
void circuit_gate_add(Circuit *c, Type t, int x, int y) ;
void circuit_do(Circuit *c, int direction) ;

void circuit_add_to_history(Circuit *c, int gate_number, Gate *new_gate
			    , const char *format, ...) ;

#endif
