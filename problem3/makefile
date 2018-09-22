CC = g++
DEBUG = -g
CFLAGS = -Wall -c
LFLAGS = -Wall

histogram:	image.h image.o ReadImage.o \
	ReadImageHeader.o WriteImage.o histogram.cpp
	$(CC) $(LFLAGS) image.o ReadImage.o ReadImageHeader.o WriteImage.o histogram.cpp -o histogram

ReadImage.o:	ReadImage.cpp
	$(CC) $(CFLAGS) ReadImage.cpp

ReadImageHeader.o:	ReadImageHeader.cpp
	$(CC) $(CFLAGS) ReadImageHeader.cpp

WriteImage.o:	WriteImage.cpp
	$(CC) $(CFLAGS) WriteImage.cpp

image.o:	image.cpp
	$(CC) $(CFLAGS) image.cpp

clean:
		\rm *.o histogram
