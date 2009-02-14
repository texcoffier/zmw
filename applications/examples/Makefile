# Executables are relinked only if you make a "make clean"
# Most of the time it is not necessary to relink them.

d:exe

default: test

include ../../Makefile.config

.SUFFIXES:.sgml .exe .regtest

ZMW_LOOP=zmw_make() { [ -f Makefile ] && for I in */. ; do ( I=`dirname $$I` ; if [ "$$1" = "clean" ] ; then TARGET="$$1" ; else TARGET="$$I.$$1" ; fi ; cd $$I ; $(MAKE) -f ../Makefile $$TARGET ) ; done || true ; } ; zmw_make



.o.exe:
	echo "Link example $*"
	$(CC) $*.o $(ZMW_OBJS) -o $*.exe

.c.regtest:
	../regression_test

test::
	. utilities ; \
	start_xvfb ; \
	$(MAKE) ask regtest

testwithoutquestion:
	. utilities ; \
	start_xvfb ; \
	$(MAKE) noforall regtest


regtest:
	$(ZMW_LOOP) $@
	A=`fgrep fail */regteststatus` ; \
	if [ "" = "$$A" ] ; \
		then echo "All tests are fine" ; \
	else \
		echo "================ List of failed tests ==============" ; \
		echo "$$A" | sed 's/\/.*//' ; \
	fi

ask:
	echo "" >xxx.replace

noforall:
	echo "noforall" >xxx.replace

exe::
	$(ZMW_LOOP) $@

clean::
	rm -f xvfb.pid */*.sgml */*.exe */*.eps */xxx* 2>/dev/null
	if [ -f "Makefile" ] ; then $(ZMW_LOOP) $@ ; fi

