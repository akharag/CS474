#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "image.h"

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

int main(int argc, char *argv[])
{
    int i, j; 
    int M, N, Q;
    bool type;
    int val;
    int glevel = 0;

    // read image header
    readImageHeader(argv[1], N, M, Q, type);

    // allocate memory for the image array

    ImageType image(N, M, Q);

    // read image
    readImage(argv[1], image);

    while (!glevel)
    {
        cout<< "Enter Number of Grey Levels: \n"
            << "1) 128 \n"
            << "2) 64 \n"
            << "3) 32 \n"
            << "4) 16 \n"
            << "5) 8 \n"
            << "6) 4 \n"
            << "7) 2 \n";
        cin >> glevel;
        if (glevel < 1 || glevel > 7) glevel = 0;
    }

    for(i = 0; i < N; i++)
    {
        for(j = 0; j < M; j++)
        {
            image.getPixelVal(i, j, val);
            val >>= glevel;
            image.setPixelVal(i, j, val);
        }
    }
    image.setImageInfo(N, M, Q>>glevel);
    writeImage(argv[2], image);
    return (1);
}
