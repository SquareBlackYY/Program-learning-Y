# coding=gbk
import math
def quadratic(a,b,c):
    n=b*b-4*a*c
    if not isinstance((a,b,c),(int,float)):
        raise TypeError('bad operand type')
    if math.sqrt(n)<0:
        return 'not found'
    else:
        math.sqrt(n)
        x1=(-b+math.sqrt(n))/2/a
        x2=(-b-math.sqrt(n))/2/a
        return x1,x2
a = input()
b = input()
c = input()
print(quadratic(a,b,c))