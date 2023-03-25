#include <stdio.h>
int main()
{
    char ch;
    int line,fstlinenum;
    double b;
    printf(">>请分别输入一个符号，金字塔的行数，以及第一行的符号数<<\n");
    scanf("%c",&ch);
    fflush(stdin);
    line = getchar();
    fflush(stdin);
    fstlinenum = getchar();
    if ((line!=(line>='0'||line<='9'))||(fstlinenum!= (fstlinenum>='0'||fstlinenum<='9')))
    {
        printf(">>请重新输入行数和第一行符号数量以保证你输入的是数字<<\n");
    }
    
    line = line-'0';
    fstlinenum = fstlinenum-'0';

    
    b=(fstlinenum+1)/2.0;
    if (line == 1)      
    {
        if(fstlinenum>80)
        {
            printf(">>你输入的首行符号的个数不能超过80<<");
            
        }  
    }
    else if (line>1)
    {
        if (fstlinenum+2*line-2>80)
        {
            printf(">>您输入首行符号的个数或者行数太大<<");
            
        }
    }
    for (int i = b; i<= line; i++)
    {
        for (int j = 1; j <= 2*line-1; j++)
        {
            if (j<=line-i ||j>=line+i)
            {
                printf(" ");
            }
            else
            {
                printf("%c",ch);
            }
        }
    printf("\n");
}   
return 0;   
}