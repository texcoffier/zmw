
default:verify_config TAGS dep lib # run_tests

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

TAGS:kernel/xxx.changed utilities/xxx.changed widgets/xxx.changed
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
	@rm -f TAGS zmw.so include/zmw/*~ include/zmw/*.bak include/zmw/"#"*


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
# Create the tar
##############################################################################
CREATE_TGZ=create_tgz() { (P=`pwd`; cd /tmp; ln -s $$P $$1; tar --exclude "*regteststatus" -cvf - $$1/* | gzip -9; rm $$1;) } ; create_tgz

##############################################################################
# Create a nightly version if regtest passed
##############################################################################
nightly_release:regteststatus makefile_clean clean
	if [ `cat regteststatus` = "pass" -a \
	     "" != "`find . -name '*.[ch]' -mtime -1`" ] ; \
	then \
	echo "Create a nightly release" ; \
	F=`date '+WIDGET_%Y_%m_%d'` ; $(CREATE_TGZ) $$F >../$$F.tar.gz ; \
	rm -f /home/exco/public_html/ZMW/WIDGET*tar.gz ; \
	cp ../$$F.tar.gz /home/exco/public_html/ZMW ; \
	sed <nightbuild.html >/home/exco/public_html/ZMW/nightbuild.html "s/VERSION/$$F/g" ; \
	fi

##############################################################################
# Create a new release
##############################################################################
new_release:makefile_clean clean
	[ -d "/home/exco/public_html/ZMW" ] && cp ChangeLog /home/exco/public_html/ZMW
	if [ `cat regteststatus` = "pass" ] ; \
	then \
	echo "Creating a new release" ; \
	$(CREATE_TGZ) zmw-$(ZMW_VERSION) >~/public_html/ZMW/zmw-$(ZMW_VERSION).tgz ; \
	fi

##############################################################################
# Goals to execute each night
##############################################################################

nightjob:clean default test nightly_release default doc

# Remove ``normal'' messages from log
nightfilter:
	(date ; make nightjob 2>&1 ; date) | \
	      grep -v -e 'Dump' -e 'Clean' -e 'Using ' -e '[a-z0-9]* [[]1].*[0-9]$$' -e 'Compiling' -e 'Link' -e '^Make ' -e 'Terminated' -e 'font path' -e 'FVWM' -e ': 0' -e '^WIDGET_200'

night:
	echo "make nightfilter night" | at 07:00

##############################################################################
# Change the version number in some files
##############################################################################
versionchange:
	echo "Update Changelog and zmw.xml for release history"
	OLD="0.0.5" ; NEW="0.0.6" ; \
	change "$$OLD" "$$NEW" README Makefile.config ; \
	change "; $$OLD)<"  "; $$NEW)<" doc/zmw.xml

##############################################################################
# Display difference between developpement files and the last version
##############################################################################
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
		--exclude="xxx*" \
		--exclude="regteststatus" \
		--exclude="#*" \
		../zmw-0.0.5 .

# DO NOT DELETE
