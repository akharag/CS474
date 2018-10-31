#include <stdio.h>
#include <stdlib.h>
#include "pgm_reader.h"

struct pgm_image *pgm_image_create_new(unsigned int x_dim, unsigned int y_dim, unsigned char format, unsigned char limit)
{
    struct pgm_image *pgm_return = (struct pgm_image *) malloc(sizeof(struct pgm_image));
    if (!pgm_return)
    {
        return (struct pgm_image *)NULL;
    }
    pgm_return->x                = x_dim;
    pgm_return->y                = y_dim;
    pgm_return->format           = format;
    pgm_return->limit            = limit;
    pgm_return->data             = (unsigned char *) malloc(sizeof(unsigned char) * x_dim * y_dim);
    if (!pgm_return->data)
    {
        free((void *) pgm_return);
        return (struct pgm_image *) NULL;
    }
    return pgm_return;
}
struct pgm_image *pgm_image_create_open(char * file)
{
    int file_type;
    int limit;
    unsigned long pixels;
    FILE *fin = fopen(file, "rb");
    if (fin)
    {
        struct pgm_image * pgm_return = (struct pgm_image *) malloc(sizeof(struct pgm_image));
        //finds file_type
        fscanf(fin, "P%d", &file_type);
        //skips comments
        while (getc(fin) != '\n');
        while (getc(fin) == '#')
        {
            while (getc(fin) != '\n');
        }
        fseek(fin, -1, SEEK_CUR);
        fscanf(fin, "%d %d\n", &pgm_return->x, &pgm_return->y );
        pixels = pgm_return->x * pgm_return->y;
        pgm_return->data = (unsigned char *) malloc(sizeof(unsigned char) * pixels);
        if (!pgm_return->data)
        {
            free((void *) pgm_return);
            fclose(fin);
            return (struct pgm_image *) NULL;
        }
        fscanf(fin, "%i\n", &limit);
        pgm_return->limit = (unsigned char)limit;
        if (file_type == 2)
        {
            for (unsigned long x = 0; x < pixels; x++)
            {
                int n;
                fscanf(fin, "%i", &n);
                pgm_return->data[x] = n;
            }
        }
        if (file_type == 5)
        {
            for (unsigned long x = 0; x < pixels; x++)
            {
                pgm_return->data[x] = getc(fin);
            }
        }
        pgm_return->format = file_type;
        fclose(fin);
        fin = NULL;
        return pgm_return;
    }
    else
    {
        return (struct pgm_image *) NULL;
    }
}

void pgm_image_destroy(struct pgm_image * object)
{
    if (object)
    {
        free((void *)object->data);
        free((void *)object);
    }
}

int pgm_image_save(struct pgm_image * object, char * file_name)
{
    if (!object)
    {
        return 0;
    }
    FILE * file_out = fopen(file_name, "w+");
    if (!file_out)
    {
        return 0;
    }
    fprintf(file_out, "P%d\n", object->format);
    fprintf(file_out, "%d %d\n", object->x, object->y);
    fprintf(file_out, "%d\n", object->limit);
    unsigned long pixels = object->x * object->y;
    if (object->format == 2)
    {
        for (unsigned long x = 0; x < pixels; x++)
        {
            fprintf(file_out, "%d ", object->data[x]);
        }
    }
    else
    {
        for (unsigned long x = 0; x < pixels; x++)
        {
            fprintf(file_out, "%c", object->data[x]);
        }
    }
    fclose(file_out);
    file_out = NULL;
    return 1;
}

unsigned char pgm_image_get_pixel(struct pgm_image *object, unsigned long x_position, unsigned long y_position, unsigned char *pixel_data)
{
    if (!object)
    {
        return 0;
    }
    *pixel_data = object->data[(y_position * object->x) + x_position];
    return 1;
}

int pgm_image_set_pixel(struct pgm_image *object, unsigned long x_position, unsigned long y_position, unsigned char pixel_data)
{
    if (!object)
    {
        return 0;
    }
    if (pixel_data > object->limit)
    {
        return 0;
    }
    object->data[(y_position * object->x) + x_position] = pixel_data;
}

int pgm_image_manipulate_batch(struct pgm_image *object, pgm_image_function function)
{
    if (!object)
    {
        return 0;
    }
    else
    {
        char *data = object->data;
        long long total = object->x * object->y;
        for (long long h = 0; h < total; h++)
        {
            data[h] = function(data[h]);
        }
    }
}

