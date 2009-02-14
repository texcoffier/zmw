
default:lib

include ../Makefile.config

O=\
	zmw_notebook.o \
	zmw_hbox.o \
	zmw_vbox.o \
	zmw_box.o \
	zmw_text.o \
	zmw_button.o \
	zmw_toggle.o \
	zmw_scrollbar.o \
	zmw_viewport.o \
	zmw_scrolled_view.o \
	zmw_anchor.o \
	zmw_filechooser.o \
	zmw_alert.o \
	zmw_image.o \
	zmw_message.o \
	zmw_window.o \
	zmw_decorator.o \
	zmw_void.o \
	zmw_table.o

lib:: $(O)

zmw_vbox.c:zmw_hbox.c
	../scripts/zmw-swap-coordinates.py <zmw_hbox.c >$@

