// ch14_1.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ch14_1()
{
    int choice;        // 存放玩家的选择
    FILE *fp;          // 文件指针，用于读取迷宫地图
    char fileName[20]; // 存放迷宫文件名
    char **maze;       // 存放迷宫地图
    int i, j;          // 循环变量
    int row, col;      // 迷宫地图的行数和列数
    int score = 0;     // 分数
    int x, y;          // 玩家当前所在位置
    int ex, ey;        // 迷宫出口位置

    while (1) // 循环，直到玩家选择退出
    {
        printf("请选择要玩的迷宫难度（输入1、2选择难度，输入0退出游戏）：");
        scanf("%d", &choice);

        if (choice == 0)
        {
            printf("游戏结束，谢谢您的参与！\n");
            break; // 退出循环，结束游戏
        }
        else if (choice == 1)
        {
            strcpy(fileName, "maze1.txt"); // 选择迷宫1
        }
        else if (choice == 2)
        {
            strcpy(fileName, "maze2.txt"); // 选择迷宫2
        }
        else
        {
            printf("输入错误，请重新输入！\n");
            continue; // 继续循环，等待正确的输入
        }

        // 打开迷宫文件
        fp = fopen(fileName, "r");
        if (fp == NULL)
        {
            printf("无法打开迷宫文件！\n");
            continue; // 继续循环，等待正确的文件名
        }

        // 读取迷宫地图
        fscanf(fp, "%d %d", &row, &col);
        maze = (char **)malloc(row * sizeof(char *));
        for (i = 0; i < row; i++)
        {
            maze[i] = (char *)malloc(col * sizeof(char));
            for (j = 0; j < col; j++)
            {
                fscanf(fp, " %c", &maze[i][j]);

                if (maze[i][j] == '@') // 记录玩家起始位置
                {
                    x = i;
                    y = j;
                }
                else if (maze[i][j] == 'O') // 记录得分点数量
                {
                    score += 10;
                }
                else if (maze[i][j] == 'E') // 记录出口位置
                {
                    ex = i;
                    ey = j;
                }
            }
        }
        fclose(fp);

        while (1) // 循环，直到玩家找到出口
        {
            // 显示迷宫地图和玩家分数
            printf("玩家分数：%d\n", score);
            for (i = 0; i < row; i++)
            {
                for (j = 0; j < col; j++)
                {
                    printf("%c", maze[i][j]);
                }
                printf("\n");
            }

            // 输入玩家移动指令
            char command;
            printf("请输入w、s、a、d控制移动，输入q退出游戏：");
            scanf(" %c", &command);

            // 处理玩家移动指令
            switch (command)
            {
            case 'w':
                // TODO: 向上移动
                break;
            case 's':
                // TODO: 向下移动
                break;
            case 'a':
                // TODO: 向左移动
                break;
            case 'd':
                // TODO: 向右移动
                break;
            case 'q':
                printf("游戏结束，谢谢您的参与！\n");
                // TODO: 释放动态内存
                break;
            default:
                printf("输入错误，请重新输入！\n");
                break;
            }

            // TODO: 判断玩家是否走到出口，更新分数
        }
    }
}