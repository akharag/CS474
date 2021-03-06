#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

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
    //int weight;

    // read image header
    readImageHeader(argv[1], N, M, Q, type);
    readImageHeader(argv[2], n, m, q, type);

    // allocate memory for the image and mask arrays

    ImageType image(N, M, Q);
    ImageType mask(n, m, q);
    ImageType output(N+n, M+m, Q);
    ImageType buffer(N+n, M+m, Q);

    // read image
    readImage(argv[1], image);
    readImage(argv[2], mask);

    //copy image with padding
    for(i = 0; i < N; i++)
    {
        for(j =0; j < M; j++)
        {
            image.getPixelVal(i, j, val);
            buffer.setPixelVal(i, j, val);
        }
    }
    int largest = 0;
    double array [N][M];
    //start correlation 
    for(i = 1; i < N ; i++)
    {
        for(j = 1; j < M - 2; j++)
        {
            new_val = 0;
            for(mask_row = 0; mask_row < n; mask_row++)
            {
                for(mask_col = 0; mask_col < m; mask_col++)
                {
                    buffer.getPixelVal(i+mask_row, j+mask_col, val);
                    mask.getPixelVal(mask_row, mask_col, mask_val); 
                    //g(i,j) = sum of w(i,j)*f(i,j)
                    new_val += val*mask_val;
                }
            }
            if (new_val > largest)
            {
                largest = new_val;
            }
            array[i][j] = new_val;
        }  
    }  
    for(i = 1; i < N ; i++)
    {
        for(j = 1; j < M - 2; j++)
        {
            array[i][j] /= largest;
            array[i][j] *= 255;
            cout << array[i][j] << endl;
        }  
    }  
    

    //copy back to old image size
    for(i = 0; i < N; i++)
        for(j = 0; j < M; j++)
        {
            image.setPixelVal(i, j, int(array[i][j]));
        }

        // write image
        writeImage(argv[3], image);

    return (1);
}
