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

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int socket_open(int port)
{
  int s ;				     /* Le socket que l'on ouvre     */
  struct sockaddr_in name ;		     /* Nom du socket                */
  int err ;				     /* Numero d'erreur de commande  */

  s = socket(AF_INET ,			     /* Le socket est sur le reseau  */
	     SOCK_STREAM,		     /* C'est analog a un pipe       */
	     0				     /* On creer le socket           */
	     ) ;
  if ( s<0 )
    {
      perror(__FILE__ "/ouvre_socket_anonyme") ;
      fprintf(stderr, "Ne peut pas ouvrir le socket de connexion\n") ;
      exit(1) ;
    }
  /*
   * Maintenant on nomme le socket, et on dit avec qui il communique
   */
  memset(&name, 0, sizeof(name)) ;
  name.sin_family      = AF_INET;	     /* Sur le reseau                */
  name.sin_addr.s_addr = INADDR_ANY ;	     /* N'importe qu'elle machine    */
  name.sin_port        = htons(port) ;	     /* Mon numero de port           */
					     /* (Suis pas sur du htons.....) */
  err = bind(s,				     /* Le socket en question        */
	     (struct sockaddr*)&name,	     /* Son nouveau nom              */
	     sizeof(name)
	     ) ;
  if ( err != 0 )
    {
      close(s) ;
      return(-1) ;
    }

  return(s) ;
}

int socket_accept(int fildes)
{
  struct sockaddr s ;
  int ui ;

  ui = sizeof(s) ;
  return( accept(fildes,&s,&ui) ) ;
}

void socket_listen(int fildes)
{
  listen(fildes, 2) ;
}
