.PHONY:html ps

html:
	cd examples ; $(MAKE) sgml
	docbook2html -u -d zmw.dsssl#html zmw.xml | \
	awk '/^<HTML/{P=1;} P==1{print;} P!=1{print >"/dev/stderr";}' >zmw.html
	if [ `id -nu` = 'exco' ] ; \
	then \
		D="$(HOME)/public_html/ZMW/zmw-$(ZMW_VERSION)" ; \
		rm -rf $$D ; \
		mkdir $$D $$D/examples ; \
		cp zmw.css zmw.html $$D ; \
		cp -Rap examples/* $$D/examples ; \
		find $$D/examples ! -type d ! -name "*.png" -print | xargs rm ; \
	fi

ps:
	cd examples ; $(MAKE) sgml eps
	docbook2ps -d zmw.dsssl#print zmw.xml

include ../Makefile.config

doc:: html ps

clean::
	rm -f zmw.html zmw.ps 2>/dev/null

# DO NOT DELETE
