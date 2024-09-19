#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int key(int p,int q);
int ifgcd1(int oula);
int generate_d(int e ,int oula);
int encrypt(int e,int n,int m);
int decrypt(int d,int n,int c);
//密钥生成
int key(int p,int q)
{   
    int n ,oula;
    n = p * q;
    oula = (p-1)*(q-1);
    return oula;
}
//判断两数是否互素
int ifgcd1(int oula)
{
    int randomNum;

    do {
        randomNum = rand() % 1000;
        if(randomNum<oula)
        {
            int smaller = (randomNum < oula) ? randomNum : oula;
            int gcd = 1;
            for (int i = 1; i <= smaller; i++) {
                if (randomNum % i == 0 && oula % i == 0) {
                    gcd = i;
                }
            }
            if (gcd == 1) {
                return randomNum;
            }
        }
        else
            continue;
    } while (1);
}
//计算私钥
int generate_d(int e ,int oula)
{
    int i;
    for(i = 0;i<oula;i++)
    {
        if((e*i)%oula == 1)
            break;
    }
    return i;
}
//加密
int encrypt(int e,int n,int m)
{
    int c;
    c = (int)(pow(m,e))%n;
    return c;
}
//解密
int decrypt(int d,int n,int c)
{
    int m;
    m=(int)(pow(c,d))%n;
    return m;
}
int main()
{
    int e,d,c,n,m = 30;
    int p=5,q=7;
    n = p*q;
    int oula= key(p,q);
    e = ifgcd1(oula);
    d = generate_d(e,oula);
    c = encrypt(e,n,m);
    m = decrypt(d,n,c);
    printf("%d",c);
    printf("%d",m);
}