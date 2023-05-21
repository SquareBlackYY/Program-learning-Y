// ch6_21.c

#include <stdio.h>

void ch6_21()
{
    int timeA = 0, timeB = 0, timeC = 0; // A、B、C三个军舰开始放炮的时间
    int count = 0;                       // 记录观众听到礼炮声的次数

    while (count < 21)
    {
        // 比较三个军舰下一次放炮的时间，并找到最早的一个
        int minTime = (timeA < timeB) ? timeA : timeB;
        minTime = (minTime < timeC) ? minTime : timeC;

        // 如果有一个军舰在最早放炮时间之前已经放炮了，
        // 就把计数器count加1，并且以该军舰的放炮时间为准
        if (timeA == minTime)
        {
            count += 1;
            timeA += 5;
        }
        if (timeB == minTime)
        {
            count += 1;
            timeB += 6;
        }
        if (timeC == minTime)
        {
            count += 1;
            timeC += 7;
        }
    }

    printf("观众总共可以听到%d次礼炮声。\n", count);
}
