from turtle import *

penup()
left(30)

for i in range(6):
    fd(100)
    left(60)

    pendown()
    for j in range(3):
        fd(100)
        left(120)
    penup()

    left(120)
    fd(100)
    right(120)

done()