run:book
	book --cache_size=10000

book: book.o loadsave.o filter.o sort.o utilities.o strings.o library.o
	$(ZMW_LINK) *.o

exe:: book

include ../../Makefile.config

# DO NOT DELETE
