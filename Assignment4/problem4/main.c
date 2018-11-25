#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pgm_reader.h"
#include "pgm_fft.h"

int main(int argc, char **argv)
{
    char flag = 1;
    char value = 0;
    printf("Opening: girl.pgm...\n" );
    struct pgm_image *image = pgm_image_create_open("./input_files/girl.pgm");
    if (!image)
    {
        printf("Error opening file\n");
        return -1;
    }
    struct pgm_complex *c_image = pgm_complex_new(image);
    // STEP 1
    pgm_complex_log(c_image);
    // STEP 2
    pgm_complex_fft(c_image, FORWARD_FFT);
    // STEP 3
    pgm_complex_hp(c_image);
    pgm_complex_save(c_image, "./output_files/out.txt");
    // STEP 4
    pgm_complex_fft(c_image, BACKWARD_FFT);
    // STEP 5
    pgm_complex_exp(c_image);
    pgm_complex_to_real(c_image, image);
    if(!pgm_image_save(image, "./output_files/out.pgm")) printf("Error: output image failed\n");
    pgm_image_destroy(image);
    pgm_complex_delete(c_image);
    return 0;
}

