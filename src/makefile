#
# makefile to compile all programs for the AED.P02 computer lab class
#

clean:
	rm -f image_processing
	rm -f process_grayscale
	rm -f process_binary
	rm -f process_rgb

all: image_processing process_grayscale process_binary process_rgb

run_all:	all
	@echo Run all demonstration programs
	./image_processing

	@echo Run all demonstration programs
	./process_grayscale

	@echo Run all demonstration programs
	./process_binary

	@echo Run all demonstration programs
	./process_rgb

	@echo Done	# note the @ at the beginning (make will not print the command)


#
# $@ gets replaced by the name of the target
#

image_processing:	 image_processing.c process_grayscale.h process_rgb.h process_binary.h
	gcc -c process_binary.c
	gcc -Wall -O2 image_processing.c process_grayscale.o process_rgb.o process_binary.o -o $@ -lm

process_grayscale:	process_grayscale.c process_grayscale.h
	gcc -Wall -O2 process_grayscale.c -o $@ -lm

process_binary:	process_binary.c process_binary.h
	gcc -Wall -O2 process_binary.c -o $@ -lm

process_rgb:	process_rgb.c process_rgb.h
	gcc -Wall -O2 process_rgb.c -o $@ -lm