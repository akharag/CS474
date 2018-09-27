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
    int sfactor;

    // read image header
    readImageHeader(argv[1], N, M, Q, type);

    // allocate memory for the image array

    ImageType image(N, M, Q);

    // read image
    readImage(argv[1], image);

    cout << "Enter sizing factor: ";
    cin >> sfactor;

    if(sfactor <= N && sfactor <= M){
        for(i = 0; i < N-sfactor+1; i+=sfactor)
            for(j = 0; j < M-sfactor+1; j+=sfactor){
                image.getPixelVal(i, j, val); //sample pixel
                for(int fillRow = 0; fillRow < sfactor; fillRow++)
                    for(int fillCol = 0; fillCol < sfactor; fillCol++){
                        image.setPixelVal(i+fillRow, j+fillCol, val);//fill values up to fill factor away
                    }    
            }
        // write image
        writeImage(argv[2], image);
    }
    else{
        cout << "Sizing factor larger than image" << endl;
    }






    return (1);
}
