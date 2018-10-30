#include <stdio.h>
#include <math.h>

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

void fft(float *data, unsigned long nn, int isign);
int main()
{
    int N = 128; //number of samples
    int u = 8; //period
    float data[N];

    printf("Original Data \n");
    for (int x = 0; x < N; x++)
    {
        data[x] = sin(6.28318530717959*u*x/N);
        printf("%g\n", data[x]);
    }
    printf("Forward FFT");
    fft(data, 2, -1);
    printf("----------------\n");
    for (int x = 0; x < N; x++)
    {
        printf("%g\n", data[x]);
    }
    printf("Inverse FFT");
    fft(data, 2, 1);
    printf("----------------\n");
    for (int x = 0; x < N; x++)
    {
        printf("%g\n", data[x]);
    }
    return 0;
}


/*
 * Note: This function has been modified so that the initial padding is not necessary
 * And it does scale by 1/N at the end of the forward FFT
 * *_data : Input Data
 * nn : 
 * isign : forward(-1) or backward(1) fft flag
 */
void fft(float *_data, unsigned long nn, int isign)
{
    unsigned long n,mmax,m,j,istep,i;
    double wtemp,wr,wpr,wpi,wi,theta;
    float tempr,tempi;
    float *data = _data - 1;
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
#undef SWAP
