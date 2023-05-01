#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define MaxSize 100      // 数组最大长度
#define Digit 1          // 坐标数字位数
#define MirrorPossible 2 // 单个反射装置最大可移动位置数

void Welcome();
void InputData(int *, int *, int[][MaxSize], int[], int[][MaxSize], int *, int *);
void PrintMap(int[][MaxSize]);
void CreatNextMap(int (*)[][MaxSize], int (*)[][MaxSize]);
void Move(int *, int *, int);
void FindMirror(int (*)[][MaxSize], int x, int y);
void AnswerCheck(int (*)[][MaxSize]);
void PrintAnswer(int (*)[][MaxSize]);

int xMAX = 0, yMAX = 0;

int main()
{
    int Laser, Direction, Mirror[MaxSize][MaxSize] = {0}, Lantern[MaxSize];
    int Map[MaxSize][MaxSize] = {0};
    int NumOfMirror = 1, NumOfLantern = 1;

    // 欢迎界面
    Welcome();

    // 初始化
    InputData(&Laser, &Direction, Mirror, Lantern, Map, &NumOfMirror, &NumOfLantern);

    // 打印初始地图
    PrintMap(Map);

    // 核心运算程序
    int x = Laser / (int)pow(10, Digit);
    int y = Laser % (int)pow(10, Digit);
    int NextMap[MaxSize][MaxSize];
    CreatNextMap(&Map, &NextMap);
    int tx = x, ty = y;
    while (true)
    {
        Move(&tx, &ty, Direction);
        if (NextMap[tx + 1][ty + 1] == 2)
        {
            NextMap[tx + 1][ty + 1] = -NextMap[tx + 1][ty + 1];
            FindMirror(&NextMap, tx, ty);
        }
        else if (NextMap[tx + 1][ty + 1] == 3)
            NextMap[tx + 1][ty + 1] = 0;
        else if (NextMap[tx + 1][ty + 1] < 0)
            break;
    }

    printf("结束\n");
    return 0;
}

// 欢迎界面
void Welcome()
{
    printf("==========================================\n"
           "《崩坏：星穹铁道》 匣里流光解密计算器 V1.0\n"
           "·当前数组最大长度为：%d\n"
           "·请注意，以左下角为坐标原点(0,0)\n"
           " 目标位置为(2,4)时，请输入204\n"
           "·输入激光器时附带方向，正北为0，顺时针递增\n"
           " 若位于(3,6)且朝向东南，请输入306 4\n"
           "·若反射装置可在(2,4)，(2,5)两处移动\n"
           " 请输入2050204或2040205\n"
           "==========================================\n",
           MaxSize);
}

// 初始化
void InputData(int *Laser, int *Direction, int Mirror[][MaxSize], int Lantern[], int Map[][MaxSize], int *NumOfMirror, int *NumOfLantern)
{
    int i, j, x, y;

    // 初始化反射装置数组
    for (i = 0; i < MaxSize; i++)
        for (j = 0; j < MirrorPossible; j++)
            Mirror[i][j] = -1;

    // 输入数据
    printf("·请输入激光器位置和方向：");
    *Laser = 40, *Direction = 6;
    // scanf("%d %d", Laser, Direction);
    printf("·请输入反射装置位置：");
    Mirror[0][0] = 20, Mirror[1][0] = 1, Mirror[2][0] = 31, Mirror[3][0] = 23, *NumOfMirror = 4;
    // for (i = 0; scanf("%d", &Mirror[i][0]) && getchar() != '\n'; i++, (*NumOfMirror)++);
    printf("·请输入灯笼位置：\n");
    Lantern[0] = 21, Lantern[1] = 22, Lantern[2] = 12, Lantern[3] = 32, *NumOfLantern = 4;
    // for (i = 0; scanf("%d", &Lantern[i]) && getchar() != '\n'; i++, (*NumOfLantern)++);

    // 可移动反射装置坐标处理
    for (i = 0; i < *NumOfMirror; i++)
        if (Mirror[i][0] >= (int)pow(10, Digit * 2))
        {
            Mirror[i][1] = Mirror[i][0] / (int)pow(10, Digit * 2);
            Mirror[i][0] %= (int)pow(10, Digit * 2);
        }

    // 计算边界
    if (*Laser / (int)pow(10, Digit) > xMAX)
        xMAX = *Laser / (int)pow(10, Digit);
    if (*Laser % (int)pow(10, Digit) > yMAX)
        yMAX = *Laser % (int)pow(10, Digit);
    for (i = 0; i < *NumOfMirror; i++)
        for (j = 0; j < MirrorPossible; j++)
        {
            if (Mirror[i][j] / (int)pow(10, Digit) > xMAX)
                xMAX = Mirror[i][j] / (int)pow(10, Digit);
            if (Mirror[i][j] % (int)pow(10, Digit) > yMAX)
                yMAX = Mirror[i][j] % (int)pow(10, Digit);
        }
    for (i = 0; i < *NumOfLantern; i++)
    {
        if (Lantern[i] / (int)pow(10, Digit) > xMAX)
            xMAX = Lantern[i] / (int)pow(10, Digit);
        if (Lantern[i] % (int)pow(10, Digit) > yMAX)
            yMAX = Lantern[i] % (int)pow(10, Digit);
    }

    // 生成围墙
    for (x = 0; x <= xMAX + 2; x++)
        for (y = 0; y <= yMAX + 2; y++)
            if (x == 0 || y == 0 || x == (xMAX + 2) || y == (yMAX + 2))
                Map[x][y] = -1;

    // 生成地图
    Map[*Laser / (int)pow(10, Digit) + 1][*Laser % (int)pow(10, Digit) + 1] = 1;
    for (i = 0; i < *NumOfMirror; i++)
    {
        Map[Mirror[i][0] / (int)pow(10, Digit) + 1][Mirror[i][0] % (int)pow(10, Digit) + 1] = 2;
        for (j = 1; j < MirrorPossible; j++)
            if (Mirror[i][j] != -1)
                Map[Mirror[i][j] / (int)pow(10, Digit) + 1][Mirror[i][j] % (int)pow(10, Digit) + 1] = 4;
    }
    for (i = 0; i < *NumOfLantern; i++)
        Map[Lantern[i] / (int)pow(10, Digit) + 1][Lantern[i] % (int)pow(10, Digit) + 1] = 3;
}

// 打印初始地图
void PrintMap(int Map[][MaxSize])
{
    printf("==========================================\n"
           "生成地图：");
    int x, y;
    for (y = yMAX + 2; y >= 0; y--)
    {
        for (x = 0; x <= xMAX + 2; x++)
        {
            if (Map[x][y] == -1) // 坐标系
            {
                if (x == 0 && y == 0)
                    printf("  ");
                else if (x == 0 && y < yMAX + 2)
                    printf("%d ", y - 1);
                else if (y == 0 && x < xMAX + 2)
                    printf("%d ", x - 1);
            }
            else if (Map[x][y] == 0)
                printf("  "); // 空
            else if (Map[x][y] == 1)
                printf("▲ "); // 激光器
            else if (Map[x][y] == 2)
                printf("♦ "); // 反射装置
            else if (Map[x][y] == 3)
                printf("● "); // 灯笼
            else if (Map[x][y] == 4)
                printf("○ "); // 反射装置可移动到的位置
        }
        printf("\n");
    }
    printf("==========================================\n");
}

// 复制新地图
void CreatNextMap(int (*Map)[][MaxSize], int (*NextMap)[][MaxSize])
{
    int i, j;
    for (i = 0; i < MaxSize; i++)
        for (j = 0; j < MaxSize; j++)
            (*NextMap)[i][j] = (*Map)[i][j];
}

// 光标移动
void Move(int *tx, int *ty, int Direction)
{
    switch (Direction)
    {
    case 0:
        (*ty)++;
        break;
    case 1:
        (*tx)++;
        (*ty)++;
        break;
    case 2:
        (*tx)++;
        break;
    case 3:
        (*tx)++;
        (*ty)--;
        break;
    case 4:
        (*ty)--;
        break;
    case 5:
        (*tx)--;
        (*ty)--;
        break;
    case 6:
        (*tx)--;
        break;
    case 7:
        (*tx)--;
        (*ty)++;
        break;
    }
}

// 找反射装置
void FindMirror(int (*Map)[][MaxSize], int x, int y)
{
    int NextMap[MaxSize][MaxSize];
    CreatNextMap(Map, &NextMap);
    int tx = x, ty = y, Direction = 0;
    for (; Direction <= 7;)
    {
        Move(&tx, &ty, Direction);
        //printf("(%d,%d)的值为%d\n", tx, ty, NextMap[tx + 1][ty + 1]);
        if (NextMap[tx + 1][ty + 1] == 2)
        {
            NextMap[tx + 1][ty + 1] = -NextMap[tx + 1][ty + 1];
            FindMirror(&NextMap, tx, ty);
        }
        else if (NextMap[tx + 1][ty + 1] == 3)
            NextMap[tx + 1][ty + 1] = 0;
        else if (NextMap[tx + 1][ty + 1] < 0 || NextMap[tx + 1][ty + 1] == 1)
        {
            AnswerCheck(&NextMap);
            tx = x;
            ty = y;
            Direction++;
            (*Map)[tx + 1][ty + 1] -= 10;
            CreatNextMap(Map, &NextMap);
            continue;
        }
    }
}

// 结果检查
void AnswerCheck(int (*Map)[][MaxSize])
{
    int x, y, RemainLantern = 0;
    for (x = 1; x <= xMAX + 1; x++)
        for (y = 1; y <= yMAX + 1; y++)
            if ((*Map)[x][y] == 3)
                RemainLantern++;
    if (RemainLantern == 0)
        PrintAnswer(Map);
}

// 结果输出
void PrintAnswer(int (*Map)[][MaxSize])
{
    printf("输出结果：");
    int x, y;
    for (y = yMAX + 2; y >= 0; y--)
    {
        for (x = 0; x <= xMAX + 2; x++)
        {
            if ((*Map)[x][y] == -1) // 坐标系
            {
                if (x == 0 && y == 0)
                    printf("  ");
                else if (x == 0 && y < yMAX + 2)
                    printf("%d ", y - 1);
                else if (y == 0 && x < xMAX + 2)
                    printf("%d ", x - 1);
            }
            else if ((*Map)[x][y] == 0) // 空
                printf("  ");
            else if ((*Map)[x][y] == 1) // 激光器
                printf("▲ ");
            else if ((*Map)[x][y] == 3) // 灯笼
                printf("● ");
            else if ((*Map)[x][y] == 4) // 反射装置可移动到的位置
                printf("○ ");
            else if ((*Map)[x][y] / 10 <= 0) // 反射装置方向
            {
                switch ((*Map)[x][y] / 10)
                {
                case 0:
                    printf("↑ ");
                    break;
                case -1:
                    printf("↗ ");
                    break;
                case -2:
                    printf("→ ");
                    break;
                case -3:
                    printf("↘ ");
                    break;
                case -4:
                    printf("↓ ");
                    break;
                case -5:
                    printf("↙ ");
                    break;
                case -6:
                    printf("← ");
                    break;
                case -7:
                    printf("↖ ");
                    break;
                }
            }
        }
        printf("\n");
    }
    printf("==========================================\n");
}