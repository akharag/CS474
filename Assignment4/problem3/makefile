all: main

main: main.c pgm_fft.o pgm_reader.o wienerfilter.c boxmuller.o
	gcc -o main main.c pgm_fft.o pgm_reader.o -lm -std=c11
	rm *.o

pgm_reader.o: pgm_reader.c
	gcc -c pgm_reader.c -lm -std=c11

pgm_fft.o: pgm_fft.c
	gcc -c pgm_fft.c -lm -std=c11

wienerfilter.o: wienerfilter.c
	gcc -c wienerfilter.c -lm -std=c11

buxmuller.o: boxmuller.c
	gcc -c boxmuller.c -lm -std=c11

clean:
	rm *.o main
