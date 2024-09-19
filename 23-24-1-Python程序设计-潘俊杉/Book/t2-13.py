from turtle import *

for i in range(9):
    penup()
    fd(10)
    left(90)
    pendown()
    circle((i + 1) * 10, 360)
    right(90)
done()
