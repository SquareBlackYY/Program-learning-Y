from turtle import *

def drawTr(p_color, p_size = 1, d_size = 50):
    color(p_color)
    pensize(p_size)
    for i in range(3):
        seth(i * 120)
        fd(d_size)

drawTr("grey", 80, 100)
drawTr("green")
goto(50, 0)
drawTr("gold")
penup()
goto(25, 25 * (3**0.5))
pendown()
drawTr("seashell")

done()
