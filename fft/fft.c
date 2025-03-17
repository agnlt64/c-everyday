#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <stdbool.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _MSC_VER
#define cfloat _Fcomplex
#define cfromreal(x) _FCbuild(x, 0)
#define cfromimag(z) _FcBuild(0, z)
#define mulcc _FCmulcc
#define addcc(z1, z2) _FCbuild(crealf(z1) + crealf(z2), cimagf(z1) + cimagf(z2))
#define subcc(z1, z2) _FCbuild(crealf(z1) - crealf(z2), cimagf(z1) - cimagf(z2))
#else
#define cfloat complex float
#define cfromreal(x) (x)
#define cfromimag(z) (z*I)
#define mulcc(z1, z2) ((z1) * (z2))
#define addcc(z1, z2) ((z1) + (z2)) 
#define subcc(z1, z2) ((z1) - (z2)) 
#endif

bool is_pow_2(int x)
{
    return (x != 0) && (x & (x - 1)) == 0;
}

void fft(cfloat* in, cfloat* out, size_t n, int stride)
{
    if (n <= 0)
    {
        fprintf(stderr, "input size must be >= 0\n");
        exit(1);
    }

    if (n == 1)
    {
        out[0] = in[0];
        return;
    }

    if (!is_pow_2(n))
    {
        fprintf(stderr, "error: size of input must be a power of 2\n");
        exit(1);
    }

    cfloat* tmp = malloc(n * sizeof(cfloat));
    for (size_t i = 0; i < n; i++) tmp[i] = out[i];

    fft(in, tmp, n/2, stride*2);
    fft(in + stride, tmp + n/2, n/2, stride*2);

    for (size_t k = 0; k < n/2; k++)
    {
        float t = (float)k / (n/2);
        cfloat v = mulcc(cexpf(cfromimag(-M_PI * t)), tmp[k + n/2]);
        cfloat e = tmp[k];
        out[k] = addcc(e, v);
        out[k + n/2] = subcc(e, v);
    }
    free(tmp);
}

int main()
{
    int n = 8;
    cfloat in[] = {0, 1, 2, 3, 4, 5, 6, 7};
    cfloat* out = malloc(sizeof(cfloat) * n);

    fft(in, out, n, 1);

    printf("fft = [");
    for (size_t i = 0; i < n; i++)
        printf("%g+%gi ", crealf(out[i]), cimagf(out[i]));
    printf("]\n");

    free(out);
    return 0;
}