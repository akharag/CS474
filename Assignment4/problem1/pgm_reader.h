#pragma once

#ifndef PGM_READER_H
#define PGM_READER_H

#define PGM_RAW 5
#define PGM_ASCII 2

struct pgm_image
{
    unsigned int x;
    unsigned int y;
    unsigned char format;
    unsigned char limit;
    unsigned char * data;
};

typedef char (*pgm_image_function)(char);

struct pgm_image *pgm_image_create_new(unsigned int, unsigned int, unsigned char, unsigned char);
struct pgm_image *pgm_image_create_open(char *);
void              pgm_image_destroy(struct pgm_image *);
int               pgm_image_save(struct pgm_image * object, char * file);
unsigned char     pgm_image_get_pixel(struct pgm_image *, unsigned long, unsigned long, unsigned char*);
int               pgm_image_set_pixel(struct pgm_image *, unsigned long, unsigned long, unsigned char);
int               pgm_image_manipulate_isolated(struct pgm_image *, pgm_image_function);
int               pgm_image_manipulate_batch(struct pgm_image *, pgm_image_function);
int               pgm_image_and(struct pgm_image *, struct pgm_image *);

#endif
