run:exe
	./fonts --cache-size=10000 --pango-cache=10000

fonts: fonts.o
	$(ZMW_LINK) fonts.o

exe:: fonts

clean::
	rm -f fonts 2>/dev/null

include ../../Makefile.config
