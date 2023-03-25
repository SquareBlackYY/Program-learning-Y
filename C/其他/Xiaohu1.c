#include <stdio.h>
#define Max 500

int main()
{
    int Count, CountN, Ten;
    char *String[Max][1];
    int Number[Max];
    char *Return;
    for (Count = 0, CountN = 0, Ten = 1; scanf("%c", &String[Count][1]) != EOF; Count++)
    {
        if (String[Count][1] >= '0' && String[Count][1] <= '9')
        {
            if (CountN = 0)
                Number[CountN] = (int)(String[Count][1] - '0') * Ten;
            else
            {
                if (String[Count - 1][1] >= '0' && String[Count - 1][1] <= '9')
                    Ten *= 10;
                    Number[CountN] = (int)(String[Count][1] - '0') * Ten;
            }
        }
        else if (String[Count][1] == ',')
        {
            CountN++;
            Ten = 1;
        }
    }
    printf(" ");
    return 0;
}

// if((String[Count] >= 'a' && String[Count] <= 'z') || (String[Count] >= 'A' && String[Count] >= 'Z'))