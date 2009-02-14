run:circuit
	circuit --cache-size=10000 --pango-cache=1000

exe:: circuit

circuit:circuit_gui.o circuit_application.o
	$(ZMW_LINK) circuit_gui.o circuit_application.o

include ../../Makefile.config

clean::
	rm -f circuit 2>/dev/null
	rm -rf PNG/.xvpics 2>/dev/null

