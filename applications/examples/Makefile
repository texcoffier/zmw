.SUFFIXES:.sgml .png .exe .regtest

ZMW_LOOP=zmw_make() { [ -f Makefile ] && for I in */. ; do ( I=`dirname $$I` ; if [ "$$1" = "clean" ] ; then TARGET="$$1" ; else TARGET="$$I.$$1" ; fi ; cd $$I ; $(MAKE) -f ../Makefile $$TARGET ) ; done || true ; } ; zmw_make


ZMW_EXECUTION_ARGS=--debug=254 --sync
ZMW_EXECUTION_ARGS=--mstimout=100
ZMW_EXECUTION_ARGS=--mstimout=10


# Small goal to test only one example
ZMW_CURRENT_TEST=draganddrop
ZMW_CURRENT_TEST=window
# ZMW_CURRENT_TEST=image

compile_current:
	cd $(ZMW_CURRENT_TEST) && $(MAKE) -f ../Makefile $(ZMW_CURRENT_TEST).exe
run_current:
	cd $(ZMW_CURRENT_TEST) && $(ZMW_CURRENT_TEST).exe
png_current:
	cd $(ZMW_CURRENT_TEST) && $(MAKE) -f ../Makefile $(ZMW_CURRENT_TEST).png

regtest_current:
	cd $(ZMW_CURRENT_TEST) && $(MAKE) -f ../Makefile $(ZMW_CURRENT_TEST).regtest

current:compile_current start_xvfb png_current stop_xvfb regtest_current



sgml:
	$(ZMW_LOOP) $@
eps:
	for I in */*.png ; do if [ -s $$I ] ; then I=`dirname $$I`/`basename $$I .png` ; convert $$I.png $$I.eps ; fi ; done

png_all:exe start_xvfb png stop_xvfb
png:
	$(ZMW_LOOP) $@

.c.sgml:
	awk <$*.c 'BEGIN{I=0;}/REGRESSION TEST/||/DO NOT DISPLAY/{I=1-I;next;}I==0{print;}' | \
	sed -e 's/&/\&#38;/g' -e 's/</\&#60;/g' \
	 -e 's/\/\*STARTEMPHASIS\*\//<emphasis>/g' \
         -e 's/\/\*STOPEMPHASIS\*\//<\/emphasis>/g'| \
	 pr -T -e >$*.sgml

.exe.png:
	echo -n "$*: "
	( \
	echo "export DISPLAY=$(ZMW_XVFB)" ; \
	echo ". ../utilities" ; \
	\
	echo "zmw_move_cursor_to 1000 1000" ; \
	echo "export PROG_NAME=xxx.$*" ; \
	echo "$*.exe --cache_size=10000 $(ZMW_EXECUTION_ARGS) 2>xxx.$*.log &" ; \
	echo 'ZMWPID=$$!' ; \
	echo "zmw_sleep 1" ; \
	awk <$*.c 'BEGIN{I=0;}/REGRESSION TEST/{I=1-I;next;}I==1{print;}' ; \
	echo 'kill $$ZMWPID 2>/dev/null || true' ; \
	echo "echo -n ' '" ; \
	\
	echo "zmw_move_cursor_to 1000 1000" ; \
	echo "export PROG_NAME=xxx_.$*" ; \
	echo "$*.exe $(ZMW_EXECUTION_ARGS) 2>xxx.cache.$*.log &" ; \
	echo 'ZMWPID=$$!' ; \
	echo "zmw_sleep 1" ; \
	awk <$*.c 'BEGIN{I=0;}/REGRESSION TEST/{I=1-I;next;}I==1{print;}' ; \
	echo 'kill $$ZMWPID 2>/dev/null || true' ; \
        touch $*.png ; \
	) | sh - 2>/dev/null
	echo

.o.exe:
	echo "Link example $*"
	$(CC) $*.o $(ZMW_OBJS) -o $*.exe

export KEY_A         = 37
export KEY_B         = 38
export KEY_LEFT      = 39
export KEY_CONTROL   = 40
export KEY_SLASH     = 41
export KEY_BACKSPACE = 42
export KEY_F1        = 43

# depth 32 does NOT work, the "xwd" command does not work correctly
start_xvfb:
	Xvfb :$(ZMW_XVFBN) -ac -screen 0 700x700x24 -noreset & echo $$! >xvfb.pid
	# Xnest :$(ZMW_XVFBN) -ac -noreset & echo $$! >xvfb.pid
	sleep 1
	xsetroot -display $(ZMW_XVFB) -solid white
	xmodmap -display $(ZMW_XVFB) -e 'keycode $(KEY_A) = a' \
			 	     -e 'keycode $(KEY_B) = b' \
				     -e 'keycode $(KEY_SLASH) = slash' \
				     -e 'keycode $(KEY_LEFT) = Left' \
				    -e 'keycode $(KEY_BACKSPACE) = BackSpace' \
				     -e 'keycode $(KEY_CONTROL) = Control_L' \
				     -e 'keycode $(KEY_F1) = F1' \
				     -e 'add control = Control_L'
	fvwm2 -display $(ZMW_XVFB) -f `pwd`/.fvwm2rc &

stop_xvfb:
	kill `cat xvfb.pid`
	rm xvfb.pid



include ../../Makefile.config

.png.regtest:
	../regression_test $*

regtest:
	echo "" >xxx.replace
	$(ZMW_LOOP) $@

test::png_all regtest

exe::
	$(ZMW_LOOP) $@

clean::clean_regtest
	rm -f *.sgml *.exe *.ps *.eps 2>/dev/null
	if [ `basename $$(pwd)` = "examples" ] ; \
		then \
		$(ZMW_LOOP) clean ; \
		fi

clean_regtest:
	for I in */. ; \
		 do \
		 J=`echo $$I | sed 's,/\.,,'` ; \
		 rm -f $$J/$$J.png $$J/$$J.regtest ; \
		 done

# DO NOT DELETE
