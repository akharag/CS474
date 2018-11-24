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
    printf("Opening: boy_noisy.pgm...\n" );
    struct pgm_image *image = pgm_image_create_open("./input_files/boy_noisy.pgm");
    if (!image)
    {
        printf("Error opening file\n");
        return -1;
    }
    struct pgm_image *mask  = pgm_image_create_open("./input_files/mask.pgm");
    struct pgm_complex *c_image = pgm_complex_new(image);
    pgm_complex_fft(c_image, FORWARD_FFT);
    struct pgm_image *r_image = pgm_image_create_new(image->x, image->y, image->format, image->limit);
    struct pgm_image *i_image = pgm_image_create_new(image->x, image->y, image->format, image->limit);
    pgm_complex_decompose(c_image, r_image, i_image);
    pgm_image_shift(r_image);
    if(!pgm_image_save(r_image, "./output_files/mags.pgm")) printf("Error: magnitude shifted failed\n");
    pgm_image_and(r_image, mask);
    pgm_image_shift(r_image);
    if(!pgm_image_save(r_image, "./output_files/magu.pgm")) printf("Error: magnitude unshifted failed\n");
    if(!pgm_image_save(i_image, "./output_files/phase.pgm")) printf("Error: phase failed\n");
    pgm_complex_compose(c_image, r_image, i_image);
    pgm_complex_fft(c_image, BACKWARD_FFT);
    pgm_complex_to_real(c_image, image);
    if(!pgm_image_save(image, "./output_files/out.pgm")) printf("Error: output image failed\n");
    pgm_complex_save(c_image, "./output_files/out.txt");
    pgm_image_destroy(image);
    pgm_image_destroy(r_image);
    pgm_image_destroy(i_image);
    pgm_complex_delete(c_image);
    return 0;
}

