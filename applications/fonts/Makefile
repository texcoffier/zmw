run:exe
	./fonts --cache-size=10000 --pango-cache=10000

fonts: fonts.o
	$(ZMW_LINK) fonts.o

exe:: fonts

include ../../Makefile.config
