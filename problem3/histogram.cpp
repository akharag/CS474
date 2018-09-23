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

void populateHistogram(int hist[], ImageType& image)
{
    int N, M, Q, val;
    image.getImageInfo(N,M,Q);
    unsigned long pixels = N * M;

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {
            image.getPixelVal(i, j, val);
            hist[val]++;
        }
    }
}
void calHistogramProbabilities(double prob[], int hist[], ImageType& image)
{
    int N, M, Q;
    image.getImageInfo(N,M,Q);
    unsigned long pixels = N * M;

    for (int q = 0; q<256;q++)
    {
        prob[q] = (double)hist[q] / pixels;
        cout << "[" << q << "]:\t" << prob[q] << endl;
    }
}
void calculateS(int s[], double probR[])
{
    double calc[256] ={0};
    calc[0] = probR[0];
    for(int i = 1; i < 256; i++)
    {
        calc[i] = calc[i-1] + probR[i];
        s[i] = calc[i]*255;
        cout << "[s" << i << "]:\t" << s[i] << endl;

    }
}
void calProbabilityZ(double probZ[], double probR[], int s[])
{
    for(int i = 0; i < 256; i++)
        for(int k = 0; k < 256; k++)
            if(s[k] == i)
                probZ[i] += probR[k];
    for(int i = 0; i < 256; i++)
    {
        cout << "[z" << i << "]:\t" << probZ[i] << endl;

    }
}
void calculateV(int v[], double probZ[])
{
    double calc[256] ={0};
    calc[0] = probZ[0];
    for(int i = 1; i < 256; i++)
    {
        calc[i] = calc[i-1] + probZ[i];
        v[i] = calc[i]*255;
        cout << "[v" << i << "]:\t" << v[i] << endl;

    }
}
void calculateZ(int z[], int s[], double probZ[])
{
    int v[256] = {0};
    calculateV(v, probZ);
    for(int i =0; i< 256; i++)
    {
        for(int k = 0; k < 256; k++)
        {
            if(s[i]==v[k])
            {
                z[i]=v[k];
                break;
            }
        }
        cout << "[z" << i << "]:\t" << z[i] << "\t";
        cout << "[s" << i << "]:\t" << s[i] << endl;

    }
}
void equalizeHistogram(ImageType& image, int r[], int z[])
{
    int value;
    for(int i = 0; i < 256; i++)
    {
        for(int j = 0; j< 256; j++)
        {
            image.getPixelVal(i, j, value);
            image.setPixelVal(i,j,z[value]);
        }
    }
}
