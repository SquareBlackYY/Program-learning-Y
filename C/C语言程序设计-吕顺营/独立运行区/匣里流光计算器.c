#include <stdio.h>
#include <stdbool.h>

#define MaxLength 10 // 数组最大长度

void Welcome();
void InputData(int *, int[], int[], int[][MaxLength + 2], int *, int *, int *, int *);
void PrintMap(int[][MaxLength + 2], int, int);
void Answer(int);

int main()
{
    int FirstLaser, Laser[MaxLength], Lantern[MaxLength], Map[MaxLength + 2][MaxLength + 2] = {0}, xMAX = 0, yMAX = 0, NumOfLaser = 1, NumOfLantern = 1;
    Welcome();
    InputData(&FirstLaser, Laser, Lantern, Map, &xMAX, &yMAX, &NumOfLaser, &NumOfLantern);
    PrintMap(Map, xMAX, yMAX);
    return 0;
}

void Welcome()
{
    printf("==========================================\n"
           "《崩坏：星穹铁道》 匣里流光解密计算器 V1.0\n"
           "·当前数组最大长度为：%d\n"
           "·请注意，以左下角为坐标原点(0,0)\n"
           "  目标位置为(2,4)时，请输入204\n"
           "==========================================\n",
           MaxLength);
}

// 初始化
void InputData(int *FirstLaser, int Laser[], int Lantern[], int Map[][MaxLength + 2], int *xMAX, int *yMAX, int *NumOfLaser, int *NumOfLantern) // 输入地图信息，并初始化地图数据
{
    int i, x, y;

    // 输入数据
    printf("·请输入初始激光器位置：\n");
    *FirstLaser = 0;
    // scanf("%d", FirstLaser);

    printf("·请输入激光器位置：\n");
    Laser[0] = 204;
 
    // for (i = 0; scanf("%d", &Laser[i]) && getchar() != '\n'; i++, *NumOfLaser++);
    printf("·请输入灯笼位置：\n");
    Lantern[0] = 508;
    // for (i = 0; scanf("%d", &Lantern[i]) && getchar() != '\n'; i++, *NumOfLantern++);

    if (*FirstLaser / 100 > *xMAX)
        *xMAX = *FirstLaser / 100;
    if (*FirstLaser % 100 > *yMAX)
        *yMAX = *FirstLaser % 100;
    for (i = 0; i < *NumOfLaser; i++)
    {
        if (Laser[i] / 100 > *xMAX)
            *xMAX = Laser[i] / 100;
        if (Laser[i] % 100 > *yMAX)
            *yMAX = Laser[i] % 100;
    }
    for (i = 0; i < *NumOfLantern; i++)
    {
        if (Lantern[i] / 100 > *xMAX)
            *xMAX = Lantern[i] / 100;
        if (Lantern[i] % 100 > *yMAX)
            *yMAX = Lantern[i] % 100;
    }

    // 生成围墙
    for (x = 0; x <= *xMAX + 2; x++)
        for (y = 0; y <= *yMAX + 2; y++)
        {
            if (x == 0 || y == 0 || x == *xMAX + 2 || y == *yMAX + 2)
                Map[x][y] = -1;
        }

    // 生成地图
    Map[*FirstLaser / 100 + 1][*FirstLaser % 100 + 1] = 1;
    for (i = 0; i < *NumOfLaser; i++)
        Map[Laser[i] / 100 + 1][Laser[i] % 100 + 1] = 2;
    for (i = 0; i < *NumOfLantern; i++)
        Map[Lantern[i] / 100 + 1][Lantern[i] % 100 + 1] = 3;
}

// 打印地图
void PrintMap(int Map[][MaxLength + 2], int xMAX, int yMAX)
{

    int x, y;
    for (y = yMAX + 2; y >= 0; y--)
    {
        for (x = 0; x <= xMAX + 2; x++)
        {
            if (Map[x][y] == -1) // 坐标系
            {
                if (x == 0 && y == 0)
                    printf("  ");
                else if (x = 0 && y < yMAX + 2)
                    printf("%d ", y - 1);
                else if (y = 0 && x < xMAX + 2)
                    printf("%d ", x - 1);
            }
            if (Map[x][y] == 0)
                printf("  "); // 空
            else if (Map[x][y] == 1)
                printf("▲ "); // 初始激光器
            else if (Map[x][y] == 2)
                printf("♦ "); // 激光器
            else if (Map[x][y] == 3)
                printf("● "); // 灯笼
        }
        printf("\n");
    }
}

void Answer(int Code) // 输出结果
{
    if (Code == true)
    {
        printf("位置\t");
    }
}