
# Launch script data_analyse.py to display some plots


# Remove debug option. compile and launch tests.
benchmarks:
	cd ../.. ; $(MAKE) clean
	sed <../../Makefile.config >xxx 's/ZMW_DEBUG_OPTIONS_DEFAULT=1/ZMW_DEBUG_OPTIONS_DEFAULT=0/'
	mv ../../Makefile.config ../../Makefile.config.orig
	mv xxx ../../Makefile.config
	cd ../.. ; $(MAKE) lib
	$(MAKE) xxx.speed
	mv xxx.speed speed.`date '+%Y-%m-%d'`
	mv ../../Makefile.config.orig ../../Makefile.config
	cd ../.. ; $(MAKE) clean




xxx.speed: speed ../../zmw.so Makefile
	cd ../examples ; $(MAKE) start_xvfb
	. ../examples/utilities ; \
	for DEPTH in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 ; \
	do \
	   for CACHE in 0 1 10 100 1000 10000 100000 `: 1000000 10000000` ; \
	   do \
		for CHILDREN in 2 4 8 16 32 64 ; \
		do \
			speed --display=$$ZMW_XVFB \
		      		--debug=0 \
		      		--cache_size=$$CACHE \
		      		$$DEPTH $$CHILDREN \
				; \
		done ; \
	   done ; \
        done >$@ 2>/dev/null
	cd ../examples ; $(MAKE) stop_xvfb

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


# DO NOT DELETE
