run:try_trace_without_cache run_main

run_main:main
	main

try_trace_without_cache: main_trace
	@if [ "`which Xvfb`" = "" ] ; \
	then echo "Run time testing is not done (you haven't Xvfb)" >&2 ; \
	else $(MAKE) trace_without_cache ; fi

trace_without_cache:
	@$(MAKE) main_trace
	(Xvfb :1 -ac ; sleep 10 && killall Xvfb) &
	sleep 1
	@export DISPLAY=:1 ; \
	for I in 0 1 2 3 ; do \
	for C in "" "--cache_size=9999" ; do \
		F="trace_$${I}_$${C}" ; \
		time main_trace $$I $$C 2>xxx ; \
		if diff -u $$F xxx ; \
		then \
		echo "=========TEST OK: $$F ==========" ; \
		else \
		echo "=========TEST FAILED: $$F ==========" ; \
		echo "It can not be possible, so there is a big bug" ; \
		exit 1 ; \
		fi ; \
	done ; \
	done
	@wc trace_*
	@rm xxx
	killall Xvfb >/dev/null


local_all::main main_sb main_trace

main:main.o ../widgets/widgets.a ../kernel/kernel.a ../utilities/utilities.a
	$(CC) $(CFLAGS) $@.o $(OBJS) -o $@
main_sb:main_sb.o ../widgets/widgets.a ../kernel/kernel.a ../utilities/utilities.a
	$(CC) $(CFLAGS) $@.o $(OBJS) -o $@
main_trace:main_trace.o ../widgets/widgets.a ../kernel/kernel.a ../utilities/utilities.a
	$(CC) $(CFLAGS) $@.o $(OBJS) -o $@

local_clean::
	@-rm main main_sb main_trace 2>/dev/null

include ../Makefile

# DO NOT DELETE

main.o: /usr/include/time.h /usr/include/features.h /usr/include/sys/cdefs.h
main.o: /usr/include/gnu/stubs.h
main.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
main.o: /usr/include/bits/time.h /usr/include/bits/types.h
main.o: /usr/include/math.h /usr/include/bits/huge_val.h
main.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
main.o: /usr/include/sys/time.h /usr/include/sys/select.h
main.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
main.o: /usr/include/ctype.h /usr/include/endian.h /usr/include/bits/endian.h
main.o: ../kernel/zmw.h /usr/include/gtk-1.2/gdk/gdk.h
main.o: /usr/include/gtk-1.2/gdk/gdktypes.h /usr/include/glib-1.2/glib.h
main.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
main.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h
main.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/float.h
main.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h
main.o: /usr/include/gtk-1.2/gdk/gdkcursors.h
main.o: /usr/include/gtk-1.2/gdk/gdkrgb.h
main.o: /usr/include/gdk-pixbuf-1.0/gdk-pixbuf/gdk-pixbuf.h
main.o: /usr/include/gdk-pixbuf-1.0/gdk-pixbuf/gdk-pixbuf-features.h
main.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
main.o: /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/setjmp.h
main.o: /usr/include/bits/setjmp.h /usr/include/stdlib.h
main.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
main.o: /usr/include/bits/confname.h /usr/include/string.h
main.o: ../widgets/zmw_widgets.h hello_world.c editable_text.c
main.o: horizontal_box.c expand.c hor_hor_box.c ver_ver_box.c ver_hor_box.c
main.o: vertical_box.c many_widgets.c color.c focus.c button.c menu.c
main.o: notebook.c tips.c drag_and_drop.c toggle.c quit.c asked.c dragger.c
main.o: animation.c anchor_box.c scrollbar.c viewport.c embeded.c sensible.c
main.o: state.c accel.c filechooser.c image.c compose.c message.c
main.o: test_trace.c
main_sb.o: ../kernel/zmw.h /usr/include/gtk-1.2/gdk/gdk.h
main_sb.o: /usr/include/gtk-1.2/gdk/gdktypes.h /usr/include/glib-1.2/glib.h
main_sb.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
main_sb.o: /usr/include/features.h /usr/include/sys/cdefs.h
main_sb.o: /usr/include/gnu/stubs.h
main_sb.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h
main_sb.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/float.h
main_sb.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h
main_sb.o: /usr/include/gtk-1.2/gdk/gdkcursors.h
main_sb.o: /usr/include/gtk-1.2/gdk/gdkrgb.h
main_sb.o: /usr/include/gdk-pixbuf-1.0/gdk-pixbuf/gdk-pixbuf.h
main_sb.o: /usr/include/gdk-pixbuf-1.0/gdk-pixbuf/gdk-pixbuf-features.h
main_sb.o: /usr/include/stdio.h
main_sb.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
main_sb.o: /usr/include/bits/types.h /usr/include/libio.h
main_sb.o: /usr/include/_G_config.h /usr/include/wchar.h
main_sb.o: /usr/include/bits/wchar.h /usr/include/gconv.h
main_sb.o: /usr/include/bits/stdio_lim.h /usr/include/setjmp.h
main_sb.o: /usr/include/bits/setjmp.h /usr/include/bits/sigset.h
main_sb.o: /usr/include/stdlib.h /usr/include/unistd.h
main_sb.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
main_sb.o: /usr/include/string.h ../widgets/zmw_widgets.h
main_trace.o: ../kernel/zmw.h /usr/include/gtk-1.2/gdk/gdk.h
main_trace.o: /usr/include/gtk-1.2/gdk/gdktypes.h
main_trace.o: /usr/include/glib-1.2/glib.h /usr/lib/glib/include/glibconfig.h
main_trace.o: /usr/include/limits.h /usr/include/features.h
main_trace.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
main_trace.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h
main_trace.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/float.h
main_trace.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h
main_trace.o: /usr/include/gtk-1.2/gdk/gdkcursors.h
main_trace.o: /usr/include/gtk-1.2/gdk/gdkrgb.h
main_trace.o: /usr/include/gdk-pixbuf-1.0/gdk-pixbuf/gdk-pixbuf.h
main_trace.o: /usr/include/gdk-pixbuf-1.0/gdk-pixbuf/gdk-pixbuf-features.h
main_trace.o: /usr/include/stdio.h
main_trace.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
main_trace.o: /usr/include/bits/types.h /usr/include/libio.h
main_trace.o: /usr/include/_G_config.h /usr/include/wchar.h
main_trace.o: /usr/include/bits/wchar.h /usr/include/gconv.h
main_trace.o: /usr/include/bits/stdio_lim.h /usr/include/setjmp.h
main_trace.o: /usr/include/bits/setjmp.h /usr/include/bits/sigset.h
main_trace.o: /usr/include/stdlib.h /usr/include/unistd.h
main_trace.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
main_trace.o: /usr/include/string.h ../widgets/zmw_widgets.h test_trace.c
main_trace.o: many_widgets.c notebook.c
