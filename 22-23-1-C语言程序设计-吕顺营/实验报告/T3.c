#include <stdio.h>
#include <malloc.h>
#include <string.h>

int main()
{
    int num_of_students, i, j;
    char **s, temp[40];
    
    // 读入学生人数
    printf("请输入学生人数：");
    scanf("%d", &num_of_students);
    getchar();

    // 分配空间
    s = (char **)malloc(sizeof(char *) * num_of_students);
    for (i = 0; i < num_of_students; i++)
        s[i] = (char *)malloc(sizeof(char) * 40);

    // 读入姓名
    for (i = 0; i < num_of_students; i++)
    {
        printf("请输入第%d位学生姓名：", i + 1);
        gets(s[i]);
    }

    // 排序
    do
    {
        for (i = 0, j = 0; i < num_of_students - 1; i++)
            if (strcmp(s[i], s[i + 1]) > 0)
            {
                strcpy(temp, s[i]);
                strcpy(s[i], s[i+1]);
                strcpy(s[i+1], temp);
                j++;
            }
    } while (j != 0);

    // 输出结果
    printf("姓名排序结果：\n");
    for (i = 0; i < num_of_students; i++)
        puts(s[i]);

    // 释放空间
    for (i = 0; i < num_of_students; i++)
        free(s[i]);
    free(s);

    return 0;
}