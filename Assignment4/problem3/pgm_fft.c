#include <math.h>
#include <stdlib.h>
#include <complex.h>
#include <stdio.h> // For DEBUGGING PURPOSES
#include "pgm_fft.h"

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
#define PI 3.141592
/*
 * Note: This function has been modified so that the initial padding is not necessary
 * And it does scale by 1/N at the end of the forward FFT
 */
void fft(double *_data, unsigned long nn, int isign)
{
    unsigned long n,mmax,m,j,istep,i;
    double wtemp,wr,wpr,wpi,wi,theta;
    double tempr,tempi;
    double *data = _data - 1;
    n=nn << 1;
    j=1;
    for (i=1;i<n;i+=2) 
    {
        if (j > i) 
        {
            SWAP(data[j],data[i]);
            SWAP(data[j+1],data[i+1]);
        }
        m=n >> 1;
        while (m >= 2 && j > m) 
        {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    mmax=2;
    while (n > mmax) 
    {
        istep=mmax << 1;
        theta=isign*(6.28318530717959/mmax);
        wtemp=sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi=sin(theta);
        wr=1.0;
        wi=0.0;
        for (m=1;m<mmax;m+=2) 
        {
            for (i=m;i<=n;i+=istep) 
            {
                j=i+mmax;
                tempr=wr*data[j]-wi*data[j+1];
                tempi=wr*data[j+1]+wi*data[j];
                data[j]=data[i]-tempr;
                data[j+1]=data[i+1]-tempi;
                data[i] += tempr;
                data[i+1] += tempi;
            }
            wr=(wtemp=wr)*wpr-wi*wpi+wr;
            wi=wi*wpr+wtemp*wpi+wi;
        }
        mmax=istep;
    }
    if (isign == -1)
    {
        for (int ffa=0; ffa < nn*2; ffa++)
        {
            _data[ffa] /= nn;
        }
    }
}

struct pgm_complex * pgm_complex_new(struct pgm_image *image)
{
    if (!image)
    {
        return (struct pgm_complex *)NULL;
    }
    struct pgm_complex *comp_img = (struct pgm_complex *) malloc(sizeof(struct pgm_complex));
    if (!comp_img)
    {
        return (struct pgm_complex *)NULL;
    }
    comp_img->sign = COMP_RI;
    comp_img->n    = image->x;
    comp_img->m    = image->y;
    comp_img->data = (double _Complex *) malloc(sizeof(double _Complex) * image->x * image->y);
    if (!comp_img)
    {
        free((void *) comp_img);
        return (struct pgm_complex *)NULL;
    }
    unsigned long long total = image->x * image->y;
    for (unsigned long long q = 0; q < total; q++)
    {
        comp_img->data[q] = (double)image->data[q]/*(double)image->limit*/ + 0.0 * I;
       // printf("%llu \n", q);
    }
    return comp_img;
}
void pgm_complex_delete(struct pgm_complex *c_image)
{
    if (c_image)
    {
        free((void *) c_image->data);
        c_image->data = NULL;
        free((void *) c_image);
    }
    return;
}

void pgm_complex_to_real(struct pgm_complex *c_image, struct pgm_image *image)
{
    if (!c_image || !image || !c_image->data)
    {
        return;
    }
    if (image->x != c_image->n || image->y != c_image->m)
    {
        if (image->data)
        {
            free((void *) image->data);
        }
        image->x = c_image->n;
        image->y = c_image->m;
        image->data = (char *) malloc(c_image->n * c_image->m);
    }
    unsigned long long total = c_image->n * c_image->m;
    for (unsigned long long q = 0; q < total; q++)
    {
        image->data[q] = (char) creal(c_image->data[q]);
    }
}

char pgm_complex_decompose(struct pgm_complex *c_image, struct pgm_image *r_image, struct pgm_image *i_image)
{
    if (!c_image || !r_image || !i_image || !c_image->data)
    {
        return 0;
    }
    if (r_image->x != c_image->n || r_image->y != c_image->m)
    {
        if (r_image->data)
        {
            free((void *) r_image->data);
        }
        r_image->x = c_image->n;
        r_image->y = c_image->m;
        r_image->data = (char *) malloc(c_image->n * c_image->m);
    }
    if (i_image->x != c_image->n || i_image->y != c_image->m)
    {
        if (i_image->data)
        {
            free((void *) i_image->data);
        }
        i_image->x = c_image->n;
        i_image->y = c_image->m;
        i_image->data = (char *) malloc(c_image->n * c_image->m);
    }
    unsigned long long total = c_image->n * c_image->m;
    double r_scale=125.0f;
    double i_scale=255.0/6.28318530717959;
    //double r_scale=127.0;
    //double i_scale=127.0;
    for (unsigned long long q = 0; q < total; q++)
    {
        r_image->data[q] = (char) floor(r_scale*log10(1+cabs(c_image->data[q])));
        i_image->data[q] = (char) floor(i_scale*(carg(c_image->data[q])+3.1415926535897932384626));
    }
    return 1;
}
char pgm_complex_compose(struct pgm_complex *c_image, struct pgm_image *mag, struct pgm_image *phase)
{
    if (!c_image || !mag || !phase || !mag->data || !phase->data)
    {
        return 0;
    }
    if (mag->x != phase->x || mag->y != phase->y)
    {
        return 0;
    }
    if (mag->x != c_image->n || mag->y != c_image->m)
    {
        if (c_image->data)
        {
            free((void *) c_image->data);
        }
        c_image->n = mag->x;
        c_image->m = mag->y;
        c_image->data = (double _Complex *) malloc(c_image->n * c_image->m);
    }
    unsigned long long total = c_image->n * c_image->m;
    for (unsigned long long q = 0; q < total; q++)
    {
        double m_v = pow(10, mag->data[q]/125.0) - 1;
        double p_v = phase->data[q] * 6.28318530717959/255.0 - 3.1415926535897932384626;
        c_image->data[q] = m_v * cexp(p_v * I);
    }
    return 1;
}

char pgm_complex_fft(struct pgm_complex * c_image, int isign)
{
    if (!c_image)
    {
        return 0;
    }
    if (isign == c_image->sign)
    {
        return 0;
    }
    c_image->sign = isign;
    // For Now, assume it is powers of two, will fix in future version
    double *data = (double *)malloc (sizeof(double) * 2 * c_image->n);
    if (!data) return 0;
    for (unsigned long long j = 0; j < c_image->m; j++)
    {
        for(unsigned long long i = 0; i < c_image->n; i++)
        {
            data[2*i]     = creal(c_image->data[i+c_image->n*j]);
            data[2*i + 1] = cimag(c_image->data[i+c_image->n*j]);
        }
        fft(data, c_image->n, isign);
        for(unsigned long long i = 0; i < c_image->n; i++)
        {
            c_image->data[i+c_image->n*j] = data[2*i] + data[2*i + 1]*I ;
        }
    }
    free((void *)data);
    data = (double *)malloc (sizeof(double) * 2 * c_image->m);
    if (!data) return 0;
    for (unsigned long long i = 0; i < c_image->n; i++)
    {
        for(unsigned long long j = 0; j < c_image->m; j++)
        {
            data[2*j]     = creal(c_image->data[i+c_image->n*j]);
            data[2*j + 1] = cimag(c_image->data[i+c_image->n*j]);
        }
        fft(data, c_image->m, isign);
        for(unsigned long long j = 0; j < c_image->m; j++)
        {
            c_image->data[i+c_image->n*j] = data[2*j] + data[2*j + 1]*I ;
        }
    }
    free((void *) data);
    return 1;
}
void pgm_complex_print(struct pgm_complex *c_img)
{
    unsigned long long p = c_img->n * c_img->m;
    for (unsigned long long h = 0; h < p; h++)
    {
        printf("[%lf,%lf]\n", cabs(c_img->data[h]), carg(c_img->data[h]));
    }
}

void pgm_complex_save(struct pgm_complex *c_img, char *file)
{
    FILE *ptr = fopen(file, "w");
    unsigned long long p = c_img->n * c_img->m;
    for (unsigned long long h = 0; h < p; h++)
    {
        fprintf(ptr,"[%lf,%lf]\n", 125.0*log10(1+cabs(c_img->data[h])), carg(c_img->data[h]));
    }
    fclose(ptr);
}

void pgm_image_shift(struct pgm_image *image)
{
    if (!image)
    {
        return;
    }
    if (!image->data)
    {
        return;
    }
    unsigned long long total = image->x * image->y;
    char *data = (char *) malloc(total);
    for (unsigned long long y = 0; y < image->y; y++)
    {
        unsigned int v = (y + (image->y / 2)) % image->y;
        for (unsigned long long x=0; x < image->x; x++)
        {
            unsigned int u = (x + (image->x / 2)) % image->x;
            data[x + image->x*y] = image->data[u+image->x*v];
        }
    }
    for (unsigned long long p =0; p < total; p++)
    {
        image->data[p] = data[p];
    }
    free((void *) data);
}

void pgm_c_image_shift(struct pgm_complex *image)
{
    if (!image)
    {
        return;
    }
    if (!image->data)
    {
        return;
    }
    unsigned long long total = image->n * image->m;
    char *data = (char *) malloc(sizeof(double _Complex) * total);
    for (unsigned long long y = 0; y < image->m; y++)
    {
        unsigned int v = (y + (image->m / 2)) % image->m;
        for (unsigned long long x=0; x < image->n; x++)
        {
            unsigned int u = (x + (image->n / 2)) % image->n;
            data[x + image->n*y] = image->data[u+image->n*v];
        }
    }
    for (unsigned long long p =0; p < total; p++)
    {
        image->data[p] = data[p];
    }
    free((void *) data);
}

void motionblur(struct pgm_complex* c_image)
{
    for(unsigned int i = 0; i < c_image->n; i++)
        for(unsigned int j=0; j < c_image->m; j++)
        {
            c_image->data[j*c_image->n + i] *= H(0.1,0.1,1,i,j);
        }      
}

void inverseFiltering(struct pgm_complex* c_image)
{
    for(unsigned int i = 0; i < c_image->n; i++)
        for(unsigned int j=0; j < c_image->m; j++)
        {
            c_image->data[j*c_image->n + i] /= H(0.1,0.1,1,i,j);
        }  
}

void wienerfilter(struct pgm_complex* c_image, float k)
{
    for(unsigned int i = 0; i < c_image->n; i++)
        for(unsigned int j=0; j < c_image->m; j++)
        {
            double _Complex h = H(0.1,0.1,1,i,j);
            double _Complex H2 = cpow(cabs(h),2);
            c_image->data[j*c_image->n + i] =  (H2/(H2+k))*(c_image->data[j*c_image->n + i]/h) 
        }
}

float box_muller(float m, float s)  /* normal random variate generator */
{                       /* mean m, standard deviation s */
    float x1, x2, w, y1;
    
    static float y2;
    static int use_last = 0;

    if (use_last)               /* use value from previous call */
    {
        y1 = y2;
        use_last = 0;
    }
    else
    {
        do {
            x1 = 2.0 * (float)rand()/RAND_MAX - 1.0;
            x2 = 2.0 * (float)rand()/RAND_MAX - 1.0;
            w = x1 * x1 + x2 * x2;
        } while ( w >= 1.0 );

        w = sqrt( (-2.0 * log( w ) ) / w );
        y1 = x1 * w;
        y2 = x2 * w;
        use_last = 1;
    }

    return( m + y1 * s );
}

double _Complex H(double a, double b, int T, int u, int v)
{
    return T*sin(PI*(u*a+v*b))*cexp(I*PI*(u*a+v*b))/(PI*(u*a+v*b));
}

#undef SWAP
