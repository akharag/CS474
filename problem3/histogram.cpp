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

    int ranges[256]={0};
    double probabilities[256];

    // read image header
    readImageHeader(argv[1], N, M, Q, type);

    // allocate memory for the image array

    ImageType image(N, M, Q);

    // read image
    readImage(argv[1], image);

    unsigned long pixels = N * M;

    for(i = 0; i < N; i++)
    {
        for(j = 0; j < M; j++)
        {
            image.getPixelVal(i, j, val);
            ranges[val]++;
        }
    }
    
    for (int q = 0; q<256;q++)
    {
        probabilities[q] = (double)ranges[q] / pixels;
        cout << "[" << q << "]:\t" << probabilities[q] << endl;
    }
    //writeImage(argv[2], image);  // Irrelevant when testing.
    return (1);
}
