CC = g++
DEBUG = -g
CFLAGS = -Wall -c
LFLAGS = -Wall

GreyLevels:	image.h image.o ReadImage.o \
	ReadImageHeader.o WriteImage.o GreyLevels.cpp
	$(CC) $(LFLAGS) image.o ReadImage.o ReadImageHeader.o WriteImage.o GreyLevels.cpp -o GreyLevels

ReadImage.o:	ReadImage.cpp
	$(CC) $(CFLAGS) ReadImage.cpp

ReadImageHeader.o:	ReadImageHeader.cpp
	$(CC) $(CFLAGS) ReadImageHeader.cpp

WriteImage.o:	WriteImage.cpp
	$(CC) $(CFLAGS) WriteImage.cpp

image.o:	image.cpp
	$(CC) $(CFLAGS) image.cpp

clean:
		\rm *.o GreyLevels