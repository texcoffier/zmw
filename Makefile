

default:verify_config TAGS dep lib # run_tests

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

TAGS:
	echo "Making TAGS"
	etags */*.[ch]

tar:makefile_clean clean
	[ -d "/home/exco/public_html/ZMW" ] && cp ChangeLog /home/exco/public_html/ZMW
	echo "Creating tar archive"
	P=`pwd` ; \
	cd /tmp ; \
	ln -s $$P zmw-$(ZMW_VERSION) ; \
	tar -cvf - zmw-$(ZMW_VERSION)/* | \
	gzip -9 >~/public_html/ZMW/zmw-$(ZMW_VERSION).tgz ; \
	rm /tmp/zmw-$(ZMW_VERSION)

# Remove all dependencies from Makefiles
makefile_clean:
	find . -name "Makefile" -print | \
	while read M ; \
		do \
		awk <$$M 'P!=1 {print;} /^# DO NOT DELETE/ {P=1;}' >xxx ; \
		mv xxx $$M ; \
		done


exe::lib

test::lib

retest:clean_regtest test

clean_regtest:
	-cd applications/examples ; $(MAKE) clean_regtest

doc::lib

include Makefile.config

lib::zmw.so

zmw.so:kernel/xxx.changed utilities/xxx.changed widgets/xxx.changed
	echo "Creating library zmw.so"
	$(CC) $(CFLAGS) kernel/*.o utilities/*.o widgets/*.o -o zmw.so

clean::
	@rm -f TAGS zmw.so include/zmw/*~ include/zmw/*.bak include/zmw/"#"*

copy:
	if [ "" != "`find . -name '*.[ch]' -mtime -1`" ] ; \
	then \
	echo "Make backup copy" ; \
	F=`date '+WIDGET_%Y_%m_%d'` ; \
	rm "/tmp/$$F" ; \
	ln -s `pwd` "/tmp/$$F" ; \
	( cd /tmp ; tar -cf - $$F/* ) | gzip -9 >../$$F.tar.gz ; \
	fi

night:
	echo "(date ; make night dep clean copy default test doc 2>&1 ; date) | \
	      fgrep -v -e 'Dump' -e 'Clean' -e 'Using ' -e 'is OK' -e 'Compiling' -e 'Link' -e 'Make ' -e 'Terminated' -e 'font path' -e 'FVWM' -e ': 0'" | at 07:00

versionchange:
	echo "Update Changelog and zmw.xml for release history"
	change 0.0.1 0.0.2 README Makefile.config

diff:
	diff -rubB \
		--exclude="Makefile" \
		--exclude="*.o" \
		--exclude="*.so" \
		--exclude="*~" \
		--exclude="*.exe" \
		--exclude="*.regtest" \
		--exclude="xxx*" \
		--exclude="#*" \
		../WIDGET-0.0.1 .

# DO NOT DELETE
