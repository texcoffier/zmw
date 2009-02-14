
ZMW_BOOK_OBJS=book.o book-preferences.o book-menubar.o loadsave.o filter.o sort.o utilities.o strings.o library.o

ZMW_XML_INCLUDE=`pkg-config --cflags libxml-2.0`
ZMW_XML_LIB=`pkg-config --libs libxml-2.0`

run:exe
	book --cache-size=10000 --pango-cache=10000

prof:$(ZMW_BOOK_OBJS)
	if [ "$(ZMW_GPROF_PROFILING)" == "" ] ; then echo "Needs ZMW_PROFILING to do profiling" >&2 ; exit 1 ; fi
	$(CC) $(ZMW_GPROF_PROFILING) ../../*/*.o *.o $(ZMW_LIBS) $(ZMW_XML_LIB)
	a.out --cache-size=10000 --pango-cache=1000 --debug=0
	gprof a.out


book: $(ZMW_BOOK_OBJS)
	$(ZMW_LINK) $(ZMW_BOOK_OBJS) $(ZMW_XML_LIB)

book-preferences.o:book-preferences.c
	echo "Compile book-preferences.c"
	$(CC) -c $(CFLAGS) $(ZMW_XML_INCLUDE) $*.c -o $*.o

exe:: translations book

include ../../Makefile.config


translations:locale/zmwbook.pot locale/fr/LC_MESSAGES/zmwbook.mo

locale/zmwbook.pot:
	xgettext -k_ book*.c -o- | sed 's/CHARSET/ISO-8859-1/' >$@

locale/fr/LC_MESSAGES/zmwbook.mo:locale/fr/LC_MESSAGES/zmwbook.po
	msgfmt $? -o$@

# Run this goal to update translated files
update-po-files:
	xgettext -j -k_ --omit-header --no-location -o locale/fr/LC_MESSAGES/zmwbook.po book*.c

clean::
	rm -f book locale/*/*/*~ locale/*/*/*.mo

