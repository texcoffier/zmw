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
 * Cr?? le Thu Jun 13 21:33:28 2002 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_HOME_WIDGET_WIDGETS_ZMW_WIDGETS_H
#define _HOME_EXCO_HOME_WIDGET_WIDGETS_ZMW_WIDGETS_H

/*
 * zmw_window.c
 */
/*
typedef enum zmw_detached
	 {
	 		 Zmw_Detached_Here
	 		 , Zmw_Detached_Next
	 		 , Zmw_Detached_Up
	 } Zmw_Detached ;
*/

Zmw_Boolean zmw_window_is_detached() ;
void zmw_window_detached_toggle() ;

void zmw_gc_pop() ;
void zmw_gc_push() ;

void zmw_window_with_id(GdkWindow **w, const char *title);
void zmw_window(const char *title);

void zmw_window_popup_right_with_id_and_title(GdkWindow **, const char *title);
void zmw_window_popup_right_with_title(const char *title);
void zmw_window_popup_right_with_id(GdkWindow **w) ;
void zmw_window_popup_right(void);

void zmw_window_popup_bottom_with_id_and_title(GdkWindow **,const char *title);
void zmw_window_popup_bottom_with_title(const char *title);
void zmw_window_popup_bottom_with_id(GdkWindow **w) ;
void zmw_window_popup_bottom(void);

void zmw_window_drag_with_id(GdkWindow **w);
void zmw_window_drag(void);

// char* zmw_window_name(Zmw_Boolean up) ;
void zmw_window_restore() ;

/*
 * zmw_label.c
 */
void zmw_text_simple(char **text, Zmw_Boolean editable, Zmw_Boolean activable
		     , int *cursor_pos, int *start_pos) ;
void zmw_label(const char *) ;
void zmw_entry(char **text) ;
void zmw_entry_with_cursor(char **text, int *cursor_pos) ;
void zmw_entry_with_cursor_and_start(char **text, int *cursor_pos
			, int *start_pos) ;
void zmw_button(const char *text) ;
void zmw_button_with_accelerator(const char *text
				 , GdkModifierType state, int character
				 ) ;
void zmw_button_with_hidden_accelerator(const char *text
				 , GdkModifierType state, int character
				 ) ;
void zmw_accelerator_init() ;
void zmw_accelerators_window(GdkModifierType filter) ;
void zmw_tearoff(void) ;
void zmw_int_editable(int *i) ;
void zmw_int(int i) ;
/*
 * zmw_check_button.c
 */
int zmw_check_button(int value) ;
void zmw_check_button_int(int *value) ;
void zmw_check_button_char(char *value) ;

int zmw_check_button_with_label(int value, const char *label) ;
void zmw_check_button_int_with_label(int *value, const char *label) ;
void zmw_check_button_char_with_label(char *value, const char *label) ;

int zmw_check_button_bits(int value, int bits, int options) ;
void zmw_check_button_bits_int(int *value, int bits) ;
void zmw_check_button_bits_char(char *value, int bits) ;

int zmw_check_button_bits_with_label(int value, int bits, const char *label) ;
void zmw_check_button_bits_int_with_label(int *value, int bits, const char *label) ;
void zmw_check_button_bits_char_with_label(char *value, int bits, const char *label);


void zmw_radio_button(int *value, int number) ;
void zmw_radio_button_with_label(int *value, int number, const char *label) ;
/*
 * zmw_anchor.c
 */
void zmw_anchor_vertical(int *x) ;
void zmw_anchor_vertical_float(float *x) ;
void zmw_anchor_box(int *x, int *y, int *width, int *height) ;
void zmw_anchor_box_with_init_values(int *x, int *y, int *width, int *height
				     , int *ix, int *iy, int *iw, int *ih) ;
void zmw_anchor_move(int *x, int *y) ;
void zmw_anchor_move_not(int x, int y) ;
void zmw_anchor_move_with_init_values(int *x, int *y, int *ix, int *iy) ;
/*
 * zmw_notebook.c
 */
void zmw_notebook(int *page) ;
void zmw_compute_notebook_size(int allocated) ;
/*
 * zmw_fixed.c
 */
void zmw_hbox(void) ;
void zmw_vbox(void) ;
void zmw_hbox_required_size(int focusable) ;
void zmw_hbox_children_allocated_size(int focusable) ;
void zmw_fixed(void) ;
void zmw_hbox_activable() ;
void zmw_vbox_activable() ;
/*
 * zmw_scrollbar.c
 */
void zmw_scrollbar2(Zmw_Float_0_1 *x, Zmw_Float_0_1 x_size
		    , Zmw_Float_0_1 *y, Zmw_Float_0_1 y_size) ; 
void zmw_hscrollbar(Zmw_Float_0_1 *x, Zmw_Float_0_1 x_size) ;
void zmw_vscrollbar(Zmw_Float_0_1 *y, Zmw_Float_0_1 y_size) ;

void zmw_scrollbar2_with_delta
(Zmw_Float_0_1 *x, Zmw_Float_0_1 x_size, Zmw_Float_0_1 x_delta
 ,Zmw_Float_0_1 *y, Zmw_Float_0_1 y_size, Zmw_Float_0_1 y_delta) ;
void zmw_hscrollbar_with_delta
(Zmw_Float_0_1 *x, Zmw_Float_0_1 x_size, Zmw_Float_0_1 x_delta) ;
void zmw_vscrollbar_with_delta(Zmw_Float_0_1 *y, Zmw_Float_0_1 y_size, Zmw_Float_0_1 y_delta) ;

/*
 * zmw_viewport.c
 */
void zmw_viewport_with_scrollbar(float *x, float *y) ;
/*
 * zmw_scrolled_view.c
 */
void zmw_scrolled_view(int *start, int *nb, int max) ;
void zmw_scrolled_view_with_columns(int *start, int *nb, int max, int nb_cols);
/*
 * zmw_file_selection.c
 */
Zmw_Boolean zmw_is_dir(const char *name) ;
void zmw_filename_normalize(char **name) ;
void zmw_file_selection(Zmw_Boolean *visible, char **filename, const char *title
		     , const char *message) ;
/*
 * zmw_alert.c
 */
void zmw_alert(char **message) ;
/*
 * zmw_table.c
 */
void zmw_table(int nb_cols) ;
void zmw_table_with_widths(int nb_cols, int *widths) ;
void zmw_table_with_widths_and_selection(int nb_cols, int *widths
			, Zmw_Boolean *selection, Zmw_Boolean unique) ;
/*
 * zmw_image.c
 */
void zmw_image(GdkPixbuf *pb) ;
void zmw_image_from_file_with_pixbuf(const char *filename, GdkPixbuf **pb);
void zmw_image_from_file_activable_with_pixbuf(const char *filename, GdkPixbuf **pb);
void zmw_image_dynamic_from_file_with_pixbuf(const char *filename, GdkPixbuf **pb,char **old_name);
void zmw_image_from_file(const char *filename);
void zmw_image_from_file_activable(const char *filename);
void zmw_image_dynamic_from_file(const char *filename,char **old_name);
/*
 * zmw_message.c
 */
int zmw_message(int visible, const char *title, const char *button_name);
void zmw_message_char(char *visible,const char *title,const char *button_name);
/*
 * zmw_void.c
 */
void zmw_void(void) ;
void zmw_if(Zmw_Boolean value) ;
void zmw_if_with_accelerators(Zmw_Boolean visible) ;
void zmw_tip(void) ;
void zmw_menu(void) ;
void zmw_menu_without_accelerators(void) ;
void zmw_menu_with_detached(int *detached) ;
/*
 * zmw_decorator.c
 */
#define Zmw_Decorator_Focusable                               0x0001
#define Zmw_Decorator_Activable                               0x0002
#define Zmw_Decorator_Border_Relief                           0x0004
#define Zmw_Decorator_Border_Solid                            0x0008
#define Zmw_Decorator_Border_Embossed                         0x0010
#define Zmw_Decorator_Border_In                               0x0020
#define Zmw_Decorator_Border_Focus                            0x0040
#define Zmw_Decorator_Interior                                0x0080
#define Zmw_Decorator_Pushable                                0x0100
// #define Zmw_Decorator_Unpop_On_Activate                       0x0200
#define Zmw_Decorator_Activable_By_Key                        0x0400
#define Zmw_Decorator_Clip                                    0x0800
#define Zmw_Decorator_Translate                               0x1000
// #define Zmw_Decorator_Unpop_On_Activate_If_Not_In_A_Popup     0x2000
// #define Zmw_Decorator_Unpop_On_Button_Press_If_Not_In_A_Popup 0x4000
#define Zmw_Decorator_Border_Out                              0x8000
#define Zmw_Decorator_Feedback                                0x10000

#define Zmw_Decorator_Border_Any (Zmw_Decorator_Border_Relief|Zmw_Decorator_Border_Solid|Zmw_Decorator_Border_Embossed|Zmw_Decorator_Pushable)
#define Zmw_Decorator_Focus_Any (Zmw_Decorator_Focusable|Zmw_Decorator_Border_Focus)

void zmw_decorator(int options, ...) ;
void zmw_alignment_horizontal_make(int i, int border_width) ;
void zmw_alignment_vertical_make(int i, int border_width) ;



#endif
