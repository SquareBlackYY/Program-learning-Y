#include <stdio.h>

double Integral(double (*)(double), double, double);
double f1(double);
double f2(double);


int main()
{
    int a = 0, b = 1;
    printf("%.2f\n", Integral(f1, a, b));
    a = 0, b = 3;
    printf("%.2f\n", Integral(f2, a, b));
}

double Integral(double (*f)(double), double a, double b)
{
    double x, sum = 0, n = 0.001;
    for(x = a;x <= b;x += n)
        sum += f(x) * n;
    return sum;
}

double f1(double x)
{
    int h = x * x + 1;
    return h;
}

double f2(double x)
{
    int h = x / (1 + x * x);
    return h;
}