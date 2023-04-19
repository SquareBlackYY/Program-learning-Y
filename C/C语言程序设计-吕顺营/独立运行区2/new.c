#include <stdio.h>
int strLen(char a[]);
main()
{
    char b[10] = {'Y', 'O', 'U', '\0', 'y', 'o', 'u'},
         a[3] = {'y', 'o', 'u'},
         c[10] = "good";
    int i = 0;
    printf("Please input your name:");
    // a[i]=getchar();
    /*scanf("%c",&a[i]);
    while(a[i]!='\n')
    {
      i++;
      //a[i]=getchar();
      scanf("%c",&a[i]);
    }
    //a[i]='\0';

    a[3]='Y';*/
    /*
    for(i=0;a[i]!='\0';i++)
      //putchar(a[i]);
      printf("%c",a[i]);*/
    // scanf("%s",a);
    // printf("%s",a);
    gets(a);
    printf("%s", a);
    puts(b);
    printf(",Good morning!%d\n", strLen(a));
}

int strLen(char a[])
{
    int i = 0, len = 0;
    while (a[i++] != '\0')
        len++;
    return len;
}
