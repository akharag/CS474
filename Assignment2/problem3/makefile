all: Spatial
CC = g++
DEBUG = -g
CFLAGS = -Wall -c
LFLAGS = -Wall

Corrupt:	image.h image.o ReadImage.o \
	ReadImageHeader.o WriteImage.o Corrupt.cpp
	$(CC) $(LFLAGS) image.o ReadImage.o ReadImageHeader.o WriteImage.o Corrupt.cpp -o Corrupt

Spatial:	image.h image.o ReadImage.o \
	ReadImageHeader.o WriteImage.o Spatial.cpp
	$(CC) $(LFLAGS) image.o ReadImage.o ReadImageHeader.o WriteImage.o Spatial.cpp -o Spatial

ReadImage.o:	ReadImage.cpp
	$(CC) $(CFLAGS) ReadImage.cpp

ReadImageHeader.o:	ReadImageHeader.cpp
	$(CC) $(CFLAGS) ReadImageHeader.cpp

WriteImage.o:	WriteImage.cpp
	$(CC) $(CFLAGS) WriteImage.cpp

image.o:	image.cpp
	$(CC) $(CFLAGS) image.cpp

clean:
		\rm *.o Spatial
