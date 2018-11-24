#pragma once
#ifndef PGM_FFT_H
#define PGM_FFT_H
#include "pgm_reader.h"

#define COMP_RI 1 // real/imaginary
#define COMP_MP -1 // magnitude/phase

#define FORWARD_FFT -1
#define BACKWARD_FFT 1

struct pgm_complex
{
    int sign;
    unsigned int n;
    unsigned int m;
    double _Complex *data;
};

void fft(double *data, unsigned long nn, int isign);
struct pgm_complex * pgm_complex_new(struct pgm_image *);
void                 pgm_complex_delete(struct pgm_complex *);
void                 pgm_complex_convert(struct pgm_complex *, int);
void                 pgm_complex_to_real(struct pgm_complex *, struct pgm_image *);
char                 pgm_complex_compose(struct pgm_complex *, struct pgm_image *, struct pgm_image *);
char                 pgm_complex_decompose(struct pgm_complex *, struct pgm_image *, struct pgm_image *);
char                 pgm_complex_fft(struct pgm_complex *, int);
void                 pgm_complex_print(struct pgm_complex *);
void                 pgm_complex_save(struct pgm_complex *, char*);
void                 pgm_image_shift(struct pgm_image *);
#endif
