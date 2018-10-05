#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <vector>

#include "image.h"

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

int (*function)(ImageType&, int, int);

int median7(ImageType&, int, int);
int median15(ImageType&, int, int);
int average_pixel7(ImageType&, int, int);
int average_pixel15(ImageType&, int, int);


int main(int argc, char *argv[])
{
    int i, j; 
    int M, N, Q;
    bool type;
    int word;

    function = average_pixel7;

    // read image header
    readImageHeader(argv[1], N, M, Q, type);

    // allocate memory for the image array

    ImageType image(N, M, Q);
    ImageType image2(N, M, Q);

    // read image
    readImage(argv[1], image);
    cout << "Choose a filter: "<<endl;
    cout << "1) Averaging 7x7"<<endl;
    cout << "2) Averaging 15x15"<<endl;
    cout << "3) Median 7x7"<<endl;
    cout << "4) Median 15x15"<<endl;
    cout << " > "<<endl;
    cin >> word;
    switch (word)
    {
    case 1:
        function = average_pixel7;
        break;
    case 2:
        function = average_pixel15;
        break;
    case 3:
        function = median7;
        break;
    case 4:
        function = median15;
        break;
    }

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

int median7(ImageType& image, int x, int y)
{
    int width;
    int height;
    int value;
    image.getImageInfo(width, height, value);
    int total = 49;
    vector<int> sort_vector;
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
            sort_vector.push_back(value);
        }
    }
    sort(sort_vector.begin(), sort_vector.end());
    return sort_vector[total/2];
}

int median15(ImageType& image, int x, int y)
{
    int width;
    int height;
    int value;
    image.getImageInfo(width, height, value);
    int total = 225;
    vector<int> sort_vector;
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
            sort_vector.push_back(value);
        }
    }
    sort(sort_vector.begin(), sort_vector.end());
    return sort_vector[total/2];
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

