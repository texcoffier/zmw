
default:html

include ../Makefile.config

.PHONY:html ps

html:
	cd examples ; $(MAKE) sgml
	docbook2html -u -d zmw.dsssl#html zmw.xml | \
	awk '/^<HTML/{P=1;} /^>Done.$$/ {next;} P==1{print;} P!=1{print >"/dev/stderr";}' >zmw.html
	if [ `id -nu` = 'exco' ] ; \
	then \
		D="$(HOME)/public_html/ZMW/zmw-$(ZMW_VERSION)" ; \
		rm -rf $$D ; \
		mkdir $$D ; \
		cp zmw.css zmw.html $$D ; \
		fgrep -i "<imagedata" <zmw.xml | \
		fgrep ".png" | \
		sed -e 's/.* fileref="//' -e 's/".*//' | \
		tar --files-from=- -cf - | \
		(cd $$D; tar -xf -) ; \
	fi

ps:
	cd examples ; $(MAKE) sgml eps
	docbook2ps -d zmw.dsssl#print zmw.xml

doc:: html ps

clean::
	rm -f zmw.html zmw.ps 2>/dev/null

# DO NOT DELETE
