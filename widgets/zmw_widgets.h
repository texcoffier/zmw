/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2003  Thierry EXCOFFIER, LIRIS

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
 * Créé le Thu Jun 13 21:33:28 2002 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_HOME_WIDGET_WIDGETS_ZMW_WIDGETS_H
#define _HOME_EXCO_HOME_WIDGET_WIDGETS_ZMW_WIDGETS_H

/*
 * zmw_window.c
 */
void zmw_window(GdkWindow **w, const char *title) ;
void zmw_window_popup_with_title(GdkWindow **w, int detached
				 , const char *title) ;
void zmw_window_popup(GdkWindow **w, int detached) ;
void zmw_window_drag(GdkWindow **w) ;
/*
 * zmw_text.c
 */
#define Zmw_Option_Border      1    
#define Zmw_Option_Focusable   2 
#define Zmw_Option_Editable    4  
#define Zmw_Option_Activable   8

void zmw_text_general(char **text, int option) ;
void zmw_text(const char *) ;
void zmw_text_editable(char **text) ;
void zmw_text_activable(const char *text) ;
void zmw_button(const char *text) ;
void zmw_button_with_accelerator(const char *text
				 , GdkModifierType state, int character
				 ) ;
void zmw_tearoff(int *detached, GdkWindow **w) ;
void zmw_int_editable(int *i) ;
/*
 * zmw_toggle.c
 */
void zmw_toggle(int *value) ;
void zmw_toggle_with_label(int *value, const char *label) ;
void zmw_toggle_bits(int *value, int bits) ;
void zmw_toggle_bits_with_label(int *value, int bits, const char *label) ;
void zmw_radio(int *value, int number) ;
void zmw_radio_with_label(int *value, int number, const char *label) ;
/*
 * zmw_anchor.c
 */
void zmw_anchor_vertical(int *x) ;
void zmw_anchor_vertical_float(float *x) ;
void zmw_anchor_box(int *x, int *y, int *width, int *height) ;
void zmw_anchor_box_with_init_values(int *x, int *y, int *width, int *height
				     , int *ix, int *iy, int *iw, int *ih) ;
void zmw_anchor_move(int *x, int *y) ;
void zmw_anchor_move_with_init_values(int *x, int *y, int *ix, int *iy) ;
/*
 * zmw_notebook.c
 */
void zmw_notebook(int *page) ;
void zmw_compute_notebook_size(int allocated) ;
/*
 * zmw_box.c
 */
void zmw_box_horizontal() ;
void zmw_box_vertical() ;
void zmw_box_horizontal_required_size() ;
void zmw_box_horizontal_children_allocated_size() ;
void zmw_box() ;
/*
 * zmw_scrollbar.c
 */
void zmw_scrollbar_horizontal(float *v) ;
/*
 * zmw_viewport.c
 */
void zmw_viewport(int x, int y) ;
void zmw_viewport_with_scrollbar(float *x, float *y) ;
void zmw_viewport_with_scrollbar_if_needed(float *x, float *y
					   , Zmw_Boolean *needed) ;
/*
 * zmw_filechooser.c
 */
Zmw_Boolean zmw_is_dir(const char *name) ;
void zmw_filename_normalize(char *name) ;
void zmw_filechooser(Zmw_Boolean *visible, GdkWindow **w
		    , char **filename, const char *title
		     , const char *message) ;
/*
 * zmw_alert.c
 */
void zmw_alert(GdkWindow **w, const char *message, Zmw_Boolean *visible) ;
/*
 * zmw_image.c
 */
void zmw_image_general(GdkPixbuf *pb, int option) ;
void zmw_image(GdkPixbuf *pb) ;
void zmw_image_from_file(const char *filename, GdkPixbuf **pb);
void zmw_image_from_file_activable(const char *filename, GdkPixbuf **pb);
void zmw_image_dynamic_from_file(const char *filename, GdkPixbuf **pb,char **old_name);
/*
 * zmw_message.g
 */
void zmw_message(GdkWindow **w, Zmw_Boolean *visible
		 , const char *title, const char *button_name) ;

#endif
