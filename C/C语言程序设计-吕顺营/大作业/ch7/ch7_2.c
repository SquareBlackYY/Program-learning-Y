// ch7_2.c

#include <stdio.h>
int hour, minute, second; // 定义全局变量
void Update()
{
    second++;
    if (second == 60)
    {
        second = 0;
        minute++;
    }
    if (minute == 60)
    {
        minute = 0;
        hour++;
    }
    if (hour == 24)
        hour = 0;
}
void Display()
{
    printf("%02d:%02d:%02d\n", hour, minute, second);
}
void Delay()
{
    int t;
    for (t = 0; t < 100000000; t++); // 用循环体为空语句的循环实现延时
}
int ch7_2(void)
{
    int i;
    hour = minute = second = 0;
    for (i = 0; i < 1000000; i++) // 利用循环结构、控制时钟运行的时间
    {
        Update();  // 更新时、分、秒显示值
        Display(); // 显示时、分、秒
        Delay();   // 模拟延迟时间为1秒
    }
    return 0;
}