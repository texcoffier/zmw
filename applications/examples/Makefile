default: test

include ../../Makefile.config

.SUFFIXES:.sgml .exe .regtest

ZMW_LOOP=zmw_make() { [ -f Makefile ] && for I in */. ; do ( I=`dirname $$I` ; if [ "$$1" = "clean" ] ; then TARGET="$$1" ; else TARGET="$$I.$$1" ; fi ; cd $$I ; $(MAKE) -f ../Makefile $$TARGET ) ; done || true ; } ; zmw_make


sgml:
	$(ZMW_LOOP) $@
eps:
	for I in */*.png ; do if [ -s $$I ] ; then I=`dirname $$I`/`basename $$I .png` ; convert $$I.png $$I.eps ; fi ; done


.c.sgml:
	awk <$*.c 'BEGIN{I=0;}/REGRESSION TEST/||/DO NOT DISPLAY/{I=1-I;next;}I==0{print;}' | \
	sed -e 's/&/\&#38;/g' -e 's/</\&#60;/g' \
	 -e 's/\/\*STARTEMPHASIS\*\//<emphasis>/g' \
         -e 's/\/\*STOPEMPHASIS\*\//<\/emphasis>/g'| \
	 pr -T -e >$*.sgml

.o.exe:
	echo "Link example $*"
	$(CC) $*.o $(ZMW_OBJS) -o $*.exe

.c.regtest:
	../regression_test

# depth 32 does NOT work, the "xwd" command does not work correctly
start_xvfb:
	. utilities ; start_xvfb

stop_xvfb:
	. utilities ; stop_xvfb


test::start_xvfb ask regtest stop_xvfb
testwithoutquestion:start_xvfb noforall regtest stop_xvfb

regtest:
	$(ZMW_LOOP) $@

ask:
	echo "" >xxx.replace

noforall:
	echo "noforall" >xxx.replace

exe::
	$(ZMW_LOOP) $@

clean::
	rm -f xvfb.pid */*.sgml */*.exe */*.eps */regtest.* */xxx* 2>/dev/null

# DO NOT DELETE
