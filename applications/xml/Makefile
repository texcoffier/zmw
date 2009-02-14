run:xmlgui
	xmlgui

exe:: xmlgui

xmlgui:xmlgui.o xml_parser.o
	$(ZMW_LINK) xmlgui.o xml_parser.o


include ../../Makefile.config

clean::
	rm -f xmlgui 2>/dev/null

