
default:verify_config TAGS dep lib exe # run_tests

test::lib

exe::lib

include Makefile.config

verify_config:
	if [ "" = "$(ZMW_CFLAGS)" ] ;\
		then \
		echo "Please edit Makefile.config" ; \
		exit 1 ; \
		fi >&2

run_tests:lib
	cd applications/tests ; $(MAKE)
run_circuit:lib
	cd applications/circuit ; $(MAKE)
run_book:lib
	cd applications/book ; $(MAKE)
run_xml:lib
	cd applications/xml ; $(MAKE)

TAGS: # kernel/xxx.changed utilities/xxx.changed widgets/xxx.changed
	echo "Make TAGS"
	etags */*.[ch]


doc::lib

lib::zmw.so

zmw.so:kernel/xxx.changed utilities/xxx.changed widgets/xxx.changed
	echo "Make library zmw.so"
	$(CC) $(CFLAGS) kernel/*.o utilities/*.o widgets/*.o -o zmw.so

kernel/xxx.changed utilities/xxx.changed widgets/xxx.changed:
	cd `dirname $@` ; $(MAKE)

clean::
	find . \( -name "*~" \
               -o -name ".*~" \
               -o -name "#*" \
               -o -name ".#*" \
               -o -name "*.bak" \
               -o -name "*.old" \
               \) -exec rm {} \;
	@rm -f TAGS zmw.so


##############################################################################
##############################################################################
# Next goals are for the ZMW developpers
##############################################################################
##############################################################################


##############################################################################
# Remove all dependencies from Makefiles
##############################################################################
makefile_clean:
	find . -name "Makefile" -print | \
	while read M ; \
		do \
		awk <$$M 'P!=1 {print;} /^# DO NOT DELETE/ {P=1;}' >xxx ; \
		mv xxx $$M ; \
		done

##############################################################################
# This file contains "pass" if the test passed
##############################################################################
regteststatus:zmw.so test
	rm -f $@
	echo "pass" >$@ ;
	for I in applications/examples/*/. ; \
	do \
		FF="$$I/regteststatus" ; \
		if [ ! -f "$$FF" ] ; \
			then \
			echo "Where is $$F?" ; \
			echo "fail" >$@ ; \
			break ; \
			fi ; \
		if [ `cat "$$FF"` != "pass" ] ; \
			then \
			echo "fail" >$@ ; \
			break ; \
			fi ; \
	done
	echo "Regression test: `cat $@`"


##############################################################################
# Create a nightly version if regtest passed
##############################################################################

nightly_release:regteststatus makefile_clean clean
	if [ `cat regteststatus` = "pass" -a \
	     "" != "`find . -name '*.[ch]' -mtime -1`" ] ; \
	then \
	echo "Create a nightly release" ; \
	F=`date '+WIDGET-%Y-%m-%d'` ; \
	scripts/zmw-tar $$F >../$$F.tar.gz ; \
	rm -f /home/exco/public_html/ZMW/WIDGET*tar.gz ; \
	cp ../$$F.tar.gz /home/exco/public_html/ZMW ; \
	sed <nightbuild.html >/home/exco/public_html/ZMW/nightbuild.html "s/VERSION/$$F/g" ; \
	fi

##############################################################################
# Run benchmark
##############################################################################
benchmarks:
	cd applications/benchmarks ; $(MAKE)
##############################################################################
# Create a new release
##############################################################################
tar:makefile_clean clean
	if [ `cat regteststatus` = "pass" ] ; \
	then \
	echo "Creating a new release" ; \
	scripts/zmw-tar zmw-$(ZMW_VERSION) >~/public_html/ZMW/zmw-$(ZMW_VERSION).tgz ; \
	fi


new_release:tar
	[ -d "/home/exco/public_html/ZMW" ] && cp ChangeLog /home/exco/public_html/ZMW
	if [ -d ~/public_html/ZMW/zmw-$(ZMW_VERSION) ] ; \
	then rm -r ~/public_html/ZMW/zmw ; \
	     mv ~/public_html/ZMW/zmw-$(ZMW_VERSION) ~/public_html/ZMW/zmw ; \
        fi

##############################################################################
# Goals to execute each night
# Dependencies are not used because "clean" is called twice
##############################################################################

nightjob:
	date ; echo "Fresh compile"
	$(MAKE) clean default
	date ; echo "Make testing and creates a tar file if there is no error"
	$(MAKE) nightly_release
	date ; echo "Run benchmarks (recompile without debug options)"
	$(MAKE) benchmarks
	date ; echo "Fresh compile and process documentation"
	$(MAKE) clean default doc
	date ; echo "End of night job"

# Remove ``normal'' messages from log
nightfilter:
	(make nightjob 2>&1) | \
	      grep -v -e 'Dump' -e 'Clean' -e 'Using ' -e '[a-z0-9]* [[]1].*[0-9]$$' -e 'Compiling' -e 'Link' -e '^Make ' -e 'Terminated' -e 'font path' -e 'FVWM' -e ': 0' -e '^WIDGET_200'

night:
	echo "make nightjob night" | at 07:00

##############################################################################
# Search unused functions
##############################################################################
cflow:
	cflow $(ZMW_INCLUDES) $(ZMW_DEBUG) */*.c applications/*/*.c applications/examples/*/*.c | \
	awk '/^[0-9]*\t[^\t]/ { H = /applications/ ; } H==0 { print $$0 ; }'

