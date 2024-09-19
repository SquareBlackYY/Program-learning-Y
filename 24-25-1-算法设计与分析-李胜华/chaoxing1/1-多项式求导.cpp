#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;

    double factor[n + 1];
    for (int i = 0; i <= n; ++i)
        cin >> factor[i];

    for (int i = n; i > 0; --i)
    {
        if (factor[n - i] != 0)
        {
            if (factor[n - i] < 0)
                printf("%.2f", factor[n - i] * i);
            else if (factor[n - i] > 0)
            {
                if (i != n)
                    printf("+");
                printf("%.2f", factor[n - i] * i);
            }

            if (i - 1 != 0)
                printf("x^%d", i - 1);
        }
    }
    printf("\n");

    return 0;
}
