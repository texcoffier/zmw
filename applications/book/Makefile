run:exe
	book --cache_size=10000

book: book.o book-preferences.o book-menubar.o \
      loadsave.o filter.o sort.o utilities.o strings.o library.o
	$(ZMW_LINK) *.o `pkg-config --libs libxml`

book-preferences.o:book-preferences.c
	echo "Compile book-preferences.c"
	$(CC) -c $(CFLAGS) `pkg-config --cflags libxml` $*.c -o $*.o

exe:: translations book

include ../../Makefile.config


translations:locale/zmwbook.pot locale/fr/LC_MESSAGES/zmwbook.mo

locale/zmwbook.pot:
	xgettext -k_ book*.c -o- | sed 's/CHARSET/ISO-8859-1/' >$@

locale/fr/LC_MESSAGES/zmwbook.mo:locale/fr/LC_MESSAGES/zmwbook.po
	msgfmt $? -o$@

clean::
	rm -f locale/*/*/*~ locale/*/*/*.mo

# DO NOT DELETE
