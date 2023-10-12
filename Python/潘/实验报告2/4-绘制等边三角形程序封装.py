from turtle import *

def drawTr(pen_color, pen_size = 1):
    color(pen_color)
    pensize(pen_size)
    for i in range(3):
        seth(i * 120)
        fd(50)

color("grey")
pensize(80)
for i in range(3):
    seth(i * 120)
    fd(100)

drawTr("green")
goto(50, 0)
drawTr("gold")
penup()
goto(25, 25 * (3**0.5))
pendown()
drawTr("seashell")

done()
