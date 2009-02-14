
default:html

include ../Makefile.config

.PHONY:html ps

PICTURE_PNG=fgrep -i "<imagedata" <zmw.xml | fgrep ".png" | sed -e 's/.* fileref="//' -e 's/".*//'

EXAMPLE_SGML=grep <zmw.xml  -i '^<!ENTITY .*\.sgml">' | sed -e 's/.* SYSTEM "//' -e 's/">//'


html: example_sgml
	docbook2html -u -d "zmw.dsssl#html" zmw.xml | \
	awk '/^<HTML/{P=1;} /^>Done.$$/ {next;} P==1{print;} P!=1{print >"/dev/stderr";}' >zmw.html
	if [ `id -nu` = 'exco' ] ; \
	then \
		D="$(HOME)/public_html/ZMW/zmw-$(ZMW_VERSION)" ; \
		rm -rf $$D ; \
		mkdir $$D ; \
		cp zmw.css zmw.html $$D ; \
		$(PICTURE_PNG) | tar --files-from=- -cf - | \
		(cd $$D; tar -xf -) ; \
	fi

sgml:
	$(ZMW_LOOP) $@

.c.sgml:

# Convert only used picture in the documentation in EPS
picture_eps:
	for I in `$(PICTURE_PNG) | sed 's/.png//g'` ;\
		do \
		convert $$I.png $$I.eps ; \
		done

# Convert only used example in the documentation in SGML
example_sgml:
	for I in `$(EXAMPLE_SGML)` ;\
		do \
		J=`echo $$I | sed 's/.sgml/.c/'` ;\
		awk <$$J 'BEGIN{I=0;}/REGRESSION TEST/||/DO NOT DISPLAY/{I=1-I;next;}I==0{print;}' | \
		sed -e 's/&/\&#38;/g' -e 's/</\&#60;/g' \
		 -e 's/\/\*STARTEMPHASIS\*\//<emphasis>/g' \
	         -e 's/\/\*STOPEMPHASIS\*\//<\/emphasis>/g'| \
		 pr -T -e >$$I ; \
		done

ps:picture_eps example_sgml
	docbook2ps -d "zmw.dsssl#print" zmw.xml

doc:: html ps

webpictures:
	if [ `id -nu` = 'exco' ] ; \
	then \
		$(MAKE) $(HOME)/public_html/ZMW/stack.png ; \
		$(MAKE) $(HOME)/public_html/ZMW/execution.png ; \
	fi

FIG2PNG=fig2dev -Lpng -S4 -b10

$(HOME)/public_html/ZMW/stack.png: stack.fig
	$(FIG2PNG) -m1 <`basename $@ .png`.fig >$@
$(HOME)/public_html/ZMW/execution.png: execution.fig Makefile
	$(FIG2PNG) -m1 <`basename $@ .png`.fig >$@


clean::
	rm -f zmw.html zmw.ps 2>/dev/null

