

default:verify_config TAGS dep lib # run_tests

test::lib

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

TAGS:
	echo "Make TAGS"
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

retest:clean_regtest test

clean_regtest:
	-rm applications/examples/*/regtest.*

doc::lib

lib::zmw.so

zmw.so:kernel/xxx.changed utilities/xxx.changed widgets/xxx.changed
	echo "Make library zmw.so"
	$(CC) $(CFLAGS) kernel/*.o utilities/*.o widgets/*.o -o zmw.so

clean::
	@rm -f TAGS zmw.so include/zmw/*~ include/zmw/*.bak include/zmw/"#"*


##############################################################################
# Next goals are the ZMW developpers
##############################################################################

# Always make copy
makecopy:
	echo "Make backup copy"
	F=`date '+WIDGET_%Y_%m_%d'` ; \
	rm -f "/tmp/$$F" 2>/dev/null ; \
	ln -s `pwd` "/tmp/$$F" ; \
	( cd /tmp ; tar -cf - $$F/* ) | gzip -9 >../$$F.tar.gz

# copy only if the source are modified in the las 24 hours.
copy:
	if [ "" != "`find . -name '*.[ch]' -mtime -1`" ] ; \
	then \
	$(MAKE) makecopy ; \
	fi

# This file contains true if the test passed
xxx.regtestpassed:zmw.so test
	rm -f $@
	if [ "`echo applications/examples/*/regtest.fail`" = "applications/examples/*/regtest.fail" -a `id -nu` = exco ] ; \
	then \
		echo "yes" >$@ ; \
		for I in applications/examples/*/. ; \
		do \
			FF="$$I/regtest.pass" ; \
			if [ ! -f "$$FF" ] ; \
				then \
				echo "Where is $$F?" ; \
				echo "no" >$@ ; \
			fi ; \
		done \
	else \
		echo "no" >$@ ; \
	fi

public:xxx.regtestpassed
	F=`date '+WIDGET_%Y_%m_%d.tar.gz'` ; \
	if [ `cat xxx.regtestpassed` = "yes" -a -f ../$$F ] ; \
	then \
		echo "I Make the last version public (no regtest errors)" ; \
		rm -f /home/exco/public_html/ZMW/WIDGET*tar.gz ; \
		cp ../$$F /home/exco/public_html/ZMW ; \
		sed <nightbuild.html >/home/exco/public_html/ZMW/nightbuild.html "s/VERSION/$$F/g" ; \
	else \
		echo "REGTEST Failed: no public version" ; \
	fi

nightjob:dep clean copy default test doc public

nightfilter:
	(date ; make nightjob 2>&1 ; date) | \
	      grep -v -e 'Dump' -e 'Clean' -e 'Using ' -e '[a-z0-9]* [[]1].*[0-9]$$' -e 'Compiling' -e 'Link' -e '^Make ' -e 'Terminated' -e 'font path' -e 'FVWM' -e ': 0'

night:
	echo "make nightfilter night" | at 07:00

versionchange:
	echo "Update Changelog and zmw.xml for release history"
	OLD="0.0.5" ; NEW="0.0.6" ; \
	change "$$OLD" "$$NEW" README Makefile.config
	change "; $$OLD)<"  "; $$NEW)<" doc/zmw.xml

diff:
	diff -rubB \
		--exclude="Makefile" \
		--exclude="*.o" \
		--exclude="*.so" \
		--exclude="*~" \
		--exclude="*.exe" \
		--exclude="*.bak" \
		--exclude="*.old" \
		--exclude="*.new" \
		--exclude="TAGS" \
		--exclude="*.eps" \
		--exclude="*.sgml" \
		--exclude="*.png" \
		--exclude="regtest.fail" \
		--exclude="regtest.pass" \
		--exclude="xxx*" \
		--exclude="#*" \
		../WIDGET_2003_12_05 .

# DO NOT DELETE
