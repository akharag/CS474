#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <cmath>

#include "image.h"

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

int sharpen(ImageType&, int, int, char);


int main(int argc, char *argv[])
{
    int i, j; 
    int M, N, Q;
    bool type;
    int word;

    char mask = 'p';

    // read image header
    readImageHeader(argv[1], N, M, Q, type);

    // allocate memory for the image array

    ImageType image(N, M, Q);
    ImageType image2(N, M, Q);

    // read image
    readImage(argv[1], image);
    cout << "Choose a filter: "<<endl;
    cout << "1) Prewitt"<<endl;
    cout << "2) Sobel"<<endl;
    cout << "3) Laplacian"<<endl;
    cout << " > "<<endl;
    cin >> word;
    switch (word)
    {
    case 1:
        mask = 'p';//prewitt
        break;
    case 2:
        mask = 's';//sobel
        break;
    case 3:
        mask = 'l';//laplacian
        break;
    }

    for(i = 0; i < N; i++)
    {
        for(j = 0; j < M; j++)
        {
            image2.setPixelVal(i, j, sharpen(image, i, j, mask));
        }
        
    }
    writeImage(argv[2], image2);
    return 0;
}

int sharpen(ImageType& image, int x, int y, char mask)
{
    int width;
    int height;
    int value;
    int total_1, total_2, total;

    int p_mask_1[3][3] = {-1,-1,-1,
                           0, 0, 0,
                           1, 1, 1};
    int p_mask_2[3][3] = {-1, 0, 1,
                          -1, 0, 1,
                          -1, 0, 1};
    int s_mask_1[3][3] = {-1,-2,-1,
                           0, 0, 0,
                           1, 2, 1};
    int s_mask_2[3][3] = {-1, 0, 1,
                          -2, 0, 2,
                          -1, 0, 1};
    int mask_l[3][3] = { 0, 1, 0,
                         1,-4, 1,
                         0, 1, 0};
    
    total_1 = 0;
    total_2 = 0;
    total = 0;

    image.getImageInfo(width, height, value);

    switch(mask)
    {
    case 'p':
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (x + i - 1 < 0 || y + j - 1 < 0 || x + i - 1 > width-1 || y + j - 1 > height-1)
                {
                    continue;
                }
                image.getPixelVal(x+i-1, y+j-1, value);
                total_1 += p_mask_1[i][j]*value;
                total_2 += p_mask_2[i][j]*value;
            }   
        }
        total = sqrt(total_1*total_1+total_2*total_2);
        if(total > 255)
            total = 255;
        break;
    case 's':
        
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (x + i - 1 < 0 || y + j - 1 < 0 || x + i - 1 > width-1 || y + j - 1 > height-1)
                {
                    continue;
                }
                image.getPixelVal(x+i-1, y+j-1, value);
                total_1 += s_mask_1[i][j]*value;
                total_2 += s_mask_2[i][j]*value;
            }   
        }
        
        total = sqrt(total_1*total_1+total_2*total_2);
        if(total > 255)
            total = 255;
        break;
    case 'l':

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (x + i - 1 < 0 || y + j - 1 < 0 || x + i - 1 > width-1 || y + j - 1 > height-1)
                {
                    continue;
                }
                image.getPixelVal(x+i-1, y+j-1, value);
                total += mask_l[i][j]*value;
            }   
        }
        if(total > 255)
            total = 255;
        break; 
    }

    
    return total;
}

