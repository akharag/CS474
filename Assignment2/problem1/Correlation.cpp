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
    int i, j, mask_row, mask_col; 
    int M, N, Q, n, m, q;
    bool type;
    int val, new_val, mask_val;
    int weight;

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
            buffer.setPixelVal(i+n, j+m, val);
        }

    //start correlation 
    for(i = n/2; i < N ; i++)
        for(j = n/2; j < M - 2; j++)
        {
            new_val = 0;
            for(mask_row = -n/2; mask_row < n/2; mask_row++)
                for(mask_col = -m/2; mask_col < m/2; mask_col++)
                {
                    buffer.getPixelVal(i+mask_row, j+mask_col, val);
                    mask.getPixelVal(n/2+mask_row, n/2+mask_col, mask_val); 
                    //g(i,j) = sum of w(i,j)*f(i,j)
                    new_val = new_val + val*mask_val;
                }
            output.setPixelVal(i, j, new_val);
        }  

    //copy back to old image size
    for(i = 0; i < N; i++)
        for(j = 0; j < M; j++)
        {
            output.getPixelVal(i+n, j+m, val);
            image.setPixelVal(i, j, val);
        }

        // write image
        writeImage(argv[3], image);

    return (1);
}
