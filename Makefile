V=0.0.0
INCLUDES=`gtk-config --cflags` `gdk-pixbuf-config --cflags` -I../kernel -I../utilities
CFLAGS=-pg -Wall -g -O6 $(INCLUDES)
CFLAGS=-Wall -g -O6 $(INCLUDES)
DIRS=. utilities kernel widgets tests demos
OBJS=../widgets/widgets.a ../kernel/kernel.a ../widgets/widgets.a ../kernel/kernel.a ../utilities/utilities.a `gtk-config --libs` `gdk-pixbuf-config --libs`


default:
	@case "`gtk-config --version`" in \
	"") echo "GTK is needed to use ZMW" ; exit 1 ;; \
	"1.2.10") echo "GTK version is fine" ;; \
	*) echo "ZMW has not been tested with this GTK version" ;; \
	esac 2>/dev/null
	@case "`gdk-pixbuf-config --version`" in \
	"") echo "GDK-Pixbuf is needed to use ZMW" ; exit 1 ;; \
	"0.17.0") echo "GDK-Pixbuf version is fine" ;; \
	*) echo "ZMW has not been tested with this GDK-Pixbuf version" ;; \
	esac 2>/dev/null
	@echo "Computing dependencies"
	$(MAKE) dep >/dev/null
	@echo "Compiling sources"
	$(MAKE) all

run_tests:default
	tests/main

run_circuit:default
	cd demos ; ./circuit

dep clean all:
	for I in $(DIRS) ; do ( cd $$I ; $(MAKE) local_$@ ) ; done

local_dep::
	makedepend $(INCLUDES) *.c

local_clean::
	@-rm *.o *~ *.bak *.a gmon.out TAGS tags xxx 2>/dev/null
	@-rm -r .xvpics 2>/dev/null

TAGS:
	etags */*.[ch]

tar:clean
	P=`pwd` ; \
	cd /tmp ; \
	ln -s $$P zmw-$(V) ; \
	tar -cvf - zmw-$(V)/* | \
	gzip -9 >~/public_html/ZMW/zmw-$(V).tgz ; \
	rm /tmp/zmw-$(V)


# DO NOT DELETE
