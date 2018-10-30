#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

#include "image.h"
//#include "fft.cpp"

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

int main(int argc, char *argv[])
{
    int i, j, mask_row, mask_col; 
    int M, N, Q, n, m, q;
    bool type;
    int val, new_val, mask_val;
    //int weight;

    // read image header
    readImageHeader(argv[1], N, M, Q, type);
    readImageHeader(argv[2], n, m, q, type);

    // allocate memory for the image and mask arrays

    ImageType image(N, M, Q);
    ImageType mask(n, m, q);
    ImageType output(N+2*n, M+2*m, Q);
    ImageType buffer(N+2*n, M+2*m, Q);

    // read image
    readImage(argv[1], image);
    readImage(argv[2], mask);

    //copy image with padding
    for(i = 0; i < N; i++)
        for(j =0; j < M; j++)
        {
            image.getPixelVal(i, j, val);
            buffer.setPixelVal(i, j, val);
        }

    //copy back to old image size
    for(i = 0; i < N; i++)
        for(j = 0; j < M; j++)
        {
            output.getPixelVal(i, j, val);
            image.setPixelVal(i, j, val);
        }

        // write image
        writeImage(argv[3], image);

    return (1);
}
