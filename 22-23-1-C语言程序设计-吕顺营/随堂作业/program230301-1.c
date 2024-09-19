#include <stdio.h>
int main()
{
    int male, female;
    for(male = 0;male <= 45;male++)
        for(female = 0;male + female <= 45;female++)
            if(male * 4 + female * 3 + (45-male-female) / 2 == 98 && (male + female) % 2 == 1)
                printf("男人%d人，女人%d人，小孩%d人\n", male, female, 45 - male - female);
}