
run_main:main
	main --cache_size=10000

exe::main main_sb main_trace

main:main.o
	$(ZMW_LINK) $@.o
main_sb:main_sb.o
	$(ZMW_LINK) $@.o
main_trace:main_trace.o
	$(ZMW_LINK) $@.o

include ../Makefile.config

clean::
	rm -f main main_sb main_trace 2>/dev/null


# DO NOT DELETE
