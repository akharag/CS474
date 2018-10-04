#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "image.h"

using namespace std;

int mask7[7][7] =
{
    {1, 1, 2, 2, 2, 1, 1},
    {1, 2, 2, 4, 2, 2, 1},
    {2, 2, 4, 8, 4, 2, 2},
    {2, 4, 8,16, 8, 4, 2},
    {2, 2, 4, 8, 4, 2, 2},
    {1, 2, 2, 4, 2, 2, 1},
    {1, 1, 2, 2, 2, 1, 1}
};

int mask15[15][15]=
{
    {2 ,2, 3, 4, 5, 5, 6, 6, 6, 5, 5, 4, 3, 2, 2},
    {2, 3, 4, 5, 7, 7, 8, 8, 8, 7, 7, 5, 4, 3, 2},
    {3, 4, 6, 7, 9,10,10,11,10,10, 9, 7, 6, 4, 3},
    {4, 5, 7, 9,10,12,13,13,13,12,10, 9, 7, 5, 4},
    {5, 7, 9,11,13,14,15,16,15,14,13,11, 9, 7, 5},
    {5, 7,10,12,14,16,17,18,17,16,14,12,10, 7, 5},
    {6, 8,10,13,15,17,19,19,19,17,15,13,10, 8, 6},
    {6, 8,11,13,16,18,19,20,19,18,16,13,11, 8, 6},
    {6, 8,10,13,15,17,19,19,19,17,15,13,10, 8, 6},
    {5, 7,10,12,14,16,17,18,17,16,14,12,10, 7, 5},
    {5, 7, 9,11,13,14,15,16,15,14,13,11, 9, 7, 5},
    {4, 5, 7, 9,10,12,13,13,13,12,10, 9, 7, 5, 4},
    {3, 4, 6, 7, 9,10,10,11,10,10, 9, 7, 6, 4, 3},
    {2, 3, 4, 5, 7, 7, 8, 8, 8, 7, 7, 5, 4, 3, 2},
    {2 ,2, 3, 4, 5, 5, 6, 6, 6, 5, 5, 4, 3, 2, 2}
};

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

int (*function)(ImageType&, int, int);

int average_pixel7(ImageType&, int, int);
int average_pixel15(ImageType&, int, int);
int gaussian7(ImageType&, int, int);
int gaussian15(ImageType&, int, int);

int main(int argc, char *argv[])
{
    int i, j; 
    int M, N, Q;
    bool type;

    function = gaussian15;

    // read image header
    readImageHeader(argv[1], N, M, Q, type);

    // allocate memory for the image array

    ImageType image(N, M, Q);
    ImageType image2(N, M, Q);

    // read image
    readImage(argv[1], image);

    for(i = 0; i < N; i++)
    {
        for(j = 0; j < M; j++)
        {
            image2.setPixelVal(i, j, function(image, i, j));
        }
        
    }
    writeImage(argv[2], image2);
    return 0;
}

int average_pixel7(ImageType& image, int x, int y)
{
    int width;
    int height;
    int value;
    image.getImageInfo(width, height, value);
    int sum = 0;
    int total = 49;
    for (int j = 0; j < 7; j++)
    {
        for (int i = 0; i < 7; i++)
        {
            if (x + i - 3 < 0 || y + j - 3 < 0 || x + i - 3 > width-1 || y + j - 3 > height-1)
            {
                total --;
                continue;
            }
            image.getPixelVal(x+i-3, y+j-3, value);
            sum += value;
        }
    }
    return sum / total;
}

int average_pixel15(ImageType& image, int x, int y)
{
    int width;
    int height;
    int value;
    image.getImageInfo(width, height, value);
    int sum = 0;
    int total = 225;
    for (int j = 0; j < 15; j++)
    {
        for (int i = 0; i < 15; i++)
        {
            if (x + i - 7 < 0 || y + j - 7 < 0 || x + i - 7 > width-1 || y + j - 7 > height-1)
            {
                total --;
                continue;
            }
            image.getPixelVal(x+i-7, y+j-7, value);
            sum += value;
        }
    }
    return sum / total;
}

int gaussian7(ImageType& image, int x, int y)
{
    int width;
    int height;
    int value;
    image.getImageInfo(width, height, value);
    int sum = 0;
    int total = 0;
    for (int j = 0; j < 7; j++)
    {
        for (int i = 0; i < 7; i++)
        {
            if (x + i - 3 < 0 || y + j - 3 < 0 || x + i - 3 > width-1 || y + j - 3 > height-1)
            {
                continue;
            }
            total += mask7[j][i];
            image.getPixelVal(x+i-3, y+j-3, value);
            sum += value * mask7[j][i];
        }
    }
    return sum / total;
}

int gaussian15(ImageType& image, int x, int y)
{
    int width;
    int height;
    int value;
    image.getImageInfo(width, height, value);
    int sum = 0;
    int total = 0;
    for (int j = 0; j < 15; j++)
    {
        for (int i = 0; i < 15; i++)
        {
            if (x + i - 7 < 0 || y + j - 7 < 0 || x + i - 7 > width-1 || y + j - 7 > height-1)
            {
                continue;
            }
            total += mask15[j][i];
            image.getPixelVal(x+i-7, y+j-7, value);
            sum += value * mask15[j][i];
        }
    }
    return sum / total;
}

