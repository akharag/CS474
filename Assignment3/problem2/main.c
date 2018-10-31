#include <stdio.h>
#include <math.h>
#include "pgm_reader.h"
#include "pgm_fft.h"

int main(int argc, char **argv)
{
    if (argc < 5)
    {
        printf("Five arguments needed: <Command Name> <Input File> <Output File> <unshifted magnitude> <shifted magnitude>\n");
        return -666;
    }
    struct pgm_image *image     = pgm_image_create_open(argv[1]);
    struct pgm_image *r_image   = pgm_image_create_new(512, 512, 5, 255); // Magnitude
    struct pgm_image *i_image   = pgm_image_create_new(512, 512, 5, 255); // Phase
    struct pgm_complex *c_image = pgm_complex_new(image);
// The fun begins
    if(pgm_complex_fft(c_image, FORWARD_FFT)) printf("Forward passed\n");
    if(pgm_complex_decompose(c_image, r_image, i_image)) printf("Decomposition passed \n");
    pgm_complex_print(c_image);
    if(pgm_complex_fft(c_image,  BACKWARD_FFT)) printf("Reverse passed\n");
//So endeth the fun
    pgm_complex_to_real(c_image, image);
    pgm_image_save(image, argv[2]);
    pgm_image_save(r_image, argv[3]);
    pgm_image_shift(r_image);
    pgm_image_save(r_image, argv[4]);
    pgm_image_destroy(image);
    pgm_image_destroy(r_image);
    pgm_image_destroy(i_image);
    pgm_complex_delete(c_image);
    return 0;
}

