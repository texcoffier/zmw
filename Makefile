

default:verify_config TAGS dep lib # run_tests

verify_config:
	@case "`gtk-config --version`" in \
	"") echo "GTK is needed to use ZMW" ; exit 1 ;; \
	"1.2.10") echo "GTK version is fine" ;; \
	*) echo "ZMW has not been tested with this GTK version" ;; \
	esac 2>/dev/null
	@case "`gdk-pixbuf-config --version`" in \
	"") echo "GDK-Pixbuf is needed to use ZMW" ; exit 1 ;; \
	"0.17.0") echo "GDK-Pixbuf version is fine" ;; \
	*) echo "ZMW has not been tested with this GDK-Pixbuf version" ;; \
	esac 2>/dev/null

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
	      fgrep -v -e 'Dump' -e 'Clean' -e 'Using ' -e 'is OK' -e 'Compiling' -e 'Link' -e 'Make ' -e 'Terminated' -e 'font path' -e 'FVWM' -e ': 0'" | at 03:00


# DO NOT DELETE
