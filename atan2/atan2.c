#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

float atan2_approx1(float y, float x)
{
    float thrqtr_pi = 3.0f * M_PI_4;
    float abs_y = fabs(y) + 1e-10f;
    float r, angle;

    if (x < 0.0f)
    {
        r = (x + abs_y) / (abs_y - x);
        angle = thrqtr_pi;
    }
    else
    {
        r = (x - abs_y) / (x + abs_y);
        angle = M_PI_4;
    }
    angle += (0.1963f * r * r - 0.9817f) * r;
    if (y < 0.0f)
        return -angle;
    else
        return angle;
}

float atan2_approx2(float y, float x)
{
    if (x == 0.0f)
    {
        if (y > 0.0f)
            return M_PI_2;
        
        if (y == 0.0f)
            return 0.0f;
        
        return -M_PI_2;
    }

    float atan;
    float z = y / x;

    if (fabs(z) < 1.0f)
    {
        atan = z / (1.0f + 0.28f * z * z);
        if (x < 0.0f)
        {
            if (x < 0.0f)
                return atan - M_PI;
            return atan + M_PI;
        }
    }
    else
    {
        atan = M_PI_2 - z / (z * z + 0.28f);
        if (y < 0.0f)
            return atan - M_PI;
    }
    return atan;
}

int main()
{
    for (float y = 0; y < 2 * M_PI; y += 0.5f)
    {
        for (float x = 0; x < 2 * M_PI; x += 0.5)
        {
            printf("atan2(%f, %f) = %f\n", y, x, atan2(y, x));
            printf("atan2_approx1(%f, %f) = %f\n", y, x, atan2_approx1(y, x));
            printf("atan2_approx2(%f, %f) = %f\n", y, x, atan2_approx2(y, x));
        }
    }

    return 0;
}