#include <stdio.h>

#define MaxLength 50 //字符串长度
//待完成功能：输入无视大小写 当前仅输入小写
//待完成功能：输出大小写控制 当前仅输出小写

int main()
{
    int InputText[MaxLength], OutputText[MaxLength], KeyText[MaxLength], TextLength, KeyLength, i, ModeChoice;
    char Letter, ReUseChoice;
    for(;;)
    {
        printf("0 加密\n"
               "1 解密\n"
               "请选择：");
        scanf("%d", &ModeChoice);
        getchar();
        if(ModeChoice == 0) printf("请输入明文：");
        else printf("请输入密文：");
        for(TextLength = 0;scanf("%c", &Letter) && Letter != '\n' && TextLength < MaxLength;TextLength++)
            InputText[TextLength] = Letter - 97;
        printf("请输入密钥：");
        for(KeyLength = 0;scanf("%c", &Letter) && Letter != '\n' && KeyLength < MaxLength;KeyLength++)
            KeyText[KeyLength] = Letter - 97;
        if(ModeChoice == 0)
        {
            printf("密文为：");
            for(i = 0;i < TextLength;i++)
            {
                OutputText[i] = (InputText[i] + KeyText[i % KeyLength]) % 26 + 97;
                printf("%c", OutputText[i]);
            }
        }
        else
        {
            printf("明文为：");
            for(i = 0;i < TextLength;i++)
            {
                if(InputText[i] < KeyText[i % KeyLength])
                    InputText[i] += 26;
                OutputText[i] = InputText[i] - KeyText[i % KeyLength] + 97;
                printf("%c", OutputText[i]);
            }
        }
        printf("\n");
        printf("继续使用？(y/n)");
        scanf("%c", &ReUseChoice);
        if(ReUseChoice == 'n')
            break;
    }
    return 0;
}