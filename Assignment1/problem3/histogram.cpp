#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "image.h"

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);
void populateHistogram(int[], ImageType&);
void calHistogramProbabilities(double[], int[], ImageType&);
void calculateS(int[], double[]);
void calProbabilityZ(double[], double[], int[]);
void calculateV(int[], double[]);
void calculateZ(int[], int [], double[]);
void equalizeHistogram(ImageType&, int[], int[]);



int main(int argc, char *argv[])
{
    //int i, j; 
    int M, N, Q;
    bool type;
    //int val;

    int ranges[256]={0};//keeps count of pixel value occurances
    int s[256],z[256] = {0};
    double probabilitiesR[256], probabilitiesZ[256] ={0};//probability of certain pixel value occuring

    // read image header
    readImageHeader(argv[1], N, M, Q, type);

    // allocate memory for the image array
    ImageType image(N, M, Q);

    // read image
    readImage(argv[1], image);

    //populate the histogram
    populateHistogram(ranges, image);

    //calculate the values for each grey value
    calHistogramProbabilities(probabilitiesR, ranges, image);

    //calcuatles S values
    calculateS(s, probabilitiesR);

    //calculates probabilities for Z histogram
    calProbabilityZ(probabilitiesZ, probabilitiesR, s);

    //calcualtes V and uses it to calculate histogram Z
    calculateZ(z, s, probabilitiesZ);

    //equalize the image using histogram Z
    equalizeHistogram(image, ranges, z);

    writeImage(argv[2], image);
    return (1);
}

