#include <stdio.h>
#include <math.h>
#include "pgm_reader.h"
#include "pgm_fft.h"

int main()
{
    struct pgm_image *image = pgm_image_create_open("./input_files/lenna.PGM");
    if (!image)
    {
        printf("Error opening file\n");
        return -1;
    }
    struct pgm_complex *c_image = pgm_complex_new(image);
    pgm_complex_fft(c_image, FORWARD_FFT);
    struct pgm_image *r_image = pgm_image_create_new(image->x, image->y, image->format, image->limit);
    struct pgm_image *i_image = pgm_image_create_new(image->x, image->y, image->format, image->limit);
    pgm_complex_decompose(c_image, r_image, i_image);
    pgm_c_image_shift(c_image);
    //motionblur(c_image);
    pgm_complex_save(c_image, "./output_files/lenna_complex.txt");
    pgm_c_image_shift(c_image);
    pgm_complex_fft(c_image, BACKWARD_FFT);
    pgm_complex_to_real(c_image, image);
    pgm_image_noise(image);
    pgm_image_save(image, "./output_files/lenna_blur.pgm");


    return 0;
}


