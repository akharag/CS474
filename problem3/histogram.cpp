#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "image.h"

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);
void populateHistogram(int[], ImageType&);
void calHistogramProbabilities(double[], ImageType&);

int main(int argc, char *argv[])
{
    int i, j; 
    int M, N, Q;
    bool type;
    int val;

    int ranges[256]={0};//keeps count of pixel value occurances
    double probabilities[256];//probability of certain pixel value occuring

    // read image header
    readImageHeader(argv[1], N, M, Q, type);

    // allocate memory for the image array

    ImageType image(N, M, Q);

    // read image
    readImage(argv[1], image);

    //populate the histogram
    populateHistogram(ranges, image);

    //calculate the values for each grey value
    calHistogramProbabilities(probabilities, ranges);
    

    //writeImage(argv[2], image);  // Irrelevant when testing.
    return (1);
}

void populateHistogram(int hist[], ImageType& image)
{
    unsigned long pixels = N * M;

    for(i = 0; i < N; i++)
    {
        for(j = 0; j < M; j++)
        {
            image.getPixelVal(i, j, val);
            ranges[val]++;
        }
    }
}
void calHistogramProbabilities(double prob[], int hist[])
{
    for (int q = 0; q<256;q++)
    {
        prob[q] = (double)hist[q] / pixels;
        cout << "[" << q << "]:\t" << prob[q] << endl;
    }
}