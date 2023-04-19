#include <stdio.h>

float f1(float);
float f2(float);
float Integral(float (*)(float), float, float);

int main()
{
    printf("y1 = %f\n", Integral(f1, 0.0, 1.0));
    printf("y2 = %f\n", Integral(f2, 0.0, 3.0));
    return 0;
}

float f1(float x)
{
    return x * x + 1;
}

float f2(float x)
{
    return x / (x * x + 1);
}

float Integral(float (*f)(float), float a, float b)
{
    float x, sum = 0, n = 0.0001;
    for(x = a;x < b;x += n)
        sum += f(x) * n;
    return sum;
}