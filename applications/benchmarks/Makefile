
all:benchmarks profiling

# Launch script data_analyse.py to display some plots


ZMW_H = ../../include/zmw/zmw.h

# Remove debug option. compile and launch tests.
benchmarks:
	cd ../.. ; $(MAKE) clean
	$(MAKE) remove_debug_option
	cd ../.. ; $(MAKE) lib
	$(MAKE) xxx.speed
	mv xxx.speed speed.`date '+%Y-%m-%d'`
	$(MAKE) restore_debug_option
	cd ../.. ; $(MAKE) clean

remove_debug_option:
	sed <$(ZMW_H) >xxx 's/ZMW_DEBUG_OPTIONS_DEFAULT 1/ZMW_DEBUG_OPTIONS_DEFAULT 0/'
	[ ! -f $(ZMW_H).orig ] && mv $(ZMW_H) $(ZMW_H).orig || true
	mv xxx $(ZMW_H)

restore_debug_option:
	mv $(ZMW_H).orig $(ZMW_H)

DEPTHS=1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
CACHES=0 1 10 100 1000 10000 100000 # 1000000 10000000
CHILDRENS=2 4 8 16 32 # 64 (window height = 4096)

xxx.speed: speed ../../zmw.so Makefile
	. ../examples/utilities ; \
	start_xvfb 2048x2048 ; \
	export DISPLAY="$$ZMW_XVFB" ; \
	zmw_move_cursor_to 10 25 ; \
	for DEPTH in $(DEPTHS) ; \
	do \
	   for CACHE in $(CACHES) ; \
	   do \
		for CHILDREN in $(CHILDRENS) ; \
		do \
			rm -f "xxx.drawdone" "xxx.exit" ; \
			echo "Before $$CACHE $$DEPTH $$CHILDREN" >/dev/tty ; \
			./speed --display=$$ZMW_XVFB \
		      		--debug=0 \
		      		--cache-size=$$CACHE \
		      		$$DEPTH $$CHILDREN \
				& \
			while true ; do sleep 1; if [ -f "xxx.drawdone" ] ; then break ; fi ; done ; \
			echo "drawdone" >/dev/tty ; \
			zmw_key $$KEY_A ; \
			while true ; do sleep 0.1; if [ -f "xxx.exit" ] ; then break ; fi ; done ; \
			echo "program terminated" >/dev/tty ; \
		done ; \
	   done ; \
        done >$@ 2>/dev/null

# Remove debug option, add profiling. compile and launch tests.

add_profiling:
	sed <../../Makefile.config >xxx -e 's/^ZMW_GPROF_PROFILING=.*/ZMW_GPROF_PROFILING=-pg/'
	[ ! -f ../../Makefile.config.orig ] && mv ../../Makefile.config ../../Makefile.config.orig || true
	mv xxx ../../Makefile.config

restore_profiling:
	mv ../../Makefile.config.orig ../../Makefile.config

profiling:
	cd ../.. ; $(MAKE) clean
	$(MAKE) remove_debug_option add_profiling
	cd ../.. ; $(MAKE) lib
	$(MAKE) gmon.out
	mv gmon.out gmon.`date '+%Y-%m-%d'`
	$(MAKE) restore_debug_option restore_profiling
	cd ../.. ; $(MAKE) clean


gmon.out: speed.o Makefile
	. ../examples/utilities ; \
	start_xvfb 2048x2048 ; \
	export DISPLAY="$$ZMW_XVFB" ; \
	zmw_move_cursor_to 50 50 ; \
	rm -f "xxx.drawdone" "xxx.exit" ; \
	$(CC) speed.o $(ZMW_GPROF_PROFILING) ../../*/*.o $(ZMW_LIBS) -o speed ; \
	./speed --display=$$ZMW_XVFB \
		      		--debug=0 \
		      		--cache-size=0 \
		      		18 8 & \
	while true ; do sleep 1; if [ -f "xxx.drawdone" ] ; then break ; fi ; done ; \
	zmw_key $$KEY_A ; \
	while true ; do sleep 0.1; if [ -f "xxx.exit" ] ; then break ; fi ; done
	gprof ./speed >xxx
	gprof -l ./speed >> xxx
	mv xxx $@
	rm speed


exe:: speed

speed:speed.o
	$(ZMW_LINK) speed.o

default: xxx.speed

clean::
	rm -f speed *.png 2>/dev/null

update_www:
	data_analyse.py
	if [ `id -nu` = 'exco' ] ; \
	then \
		mv *.png ~/public_html/ZMW/BENCHMARKS/ ; \
	fi



include ../../Makefile.config

