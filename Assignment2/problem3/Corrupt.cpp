#include <cstdlib>
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
    int percent = 50;
    int M, N, Q;
    bool type;

    srand(time(0));
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
            if (rand() % 100 < percent)
            {
                image2.setPixelVal(i, j, (rand()%2)?0:Q);
            }
            else
            {
                int val;
                image.getPixelVal(i, j, val);
                image2.setPixelVal(i, j, val);
            }
        }
        
    }
    writeImage(argv[2], image2);
    return 0;
}

