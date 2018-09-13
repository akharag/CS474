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
    int glevel;

    // read image header
    readImageHeader(argv[1], N, M, Q, type);

    // allocate memory for the image array

    ImageType image(N, M, Q);

    // read image
    readImage(argv[1], image);

    cout << "Enter Number of Grey Levels: ";
    cin >> glevel;



    if(glevel <= 256){
        for(i = 0; i < N; i++)
            for(j = 0; j < M; j++){
                image.getPixelVal(i, j, val);
                val = val - (256 % glevel);
                image.setPixelVal(i, j, val)
            }
        /*for(i = 0; i < N-glevel+1; i+=glevel)
            for(j = 0; j < M-glevel+1; j+=glevel){
                image.getPixelVal(i, j, val); //sample pixel
                for(int fillRow = 0; fillRow < glevel; fillRow++)
                    for(int fillCol = 0; fillCol < glevel; fillCol++){
                        image.setPixelVal(i+fillRow, j+fillCol, val);//fill values up to fill factor away
                    }    
            }*/
        // write image
        writeImage(argv[2], image);
    }
    else{
        cout << "Grey Levels Must be Less than 256" << endl;
    }






    return (1);
}
