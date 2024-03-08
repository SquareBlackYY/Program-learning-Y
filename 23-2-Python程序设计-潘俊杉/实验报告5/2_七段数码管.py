import datetime
import turtle

s = 20
digit = {
    "0": [(0, s), (s, s), (s, 0), (s, -s), (0, -s), (0, 0), (0, s)],
    "1": [(s, s), (s, 0), (s, -s)],
    "2": [(0, s), (s, s), (s, 0), (0, 0), (0, -s), (s, -s)],
    "3": [(0, s), (s, s), (s, 0), (0, 0), (s, 0), (s, -s), (0, -s)],
    "4": [(0, s), (0, 0), (s, 0), (s, s), (s, 0), (s, -s)],
    "5": [(s, s), (0, s), (0, 0), (s, 0), (s, -s), (0, -s)],
    "6": [(s, s), (0, s), (0, 0), (0, -s), (s, -s), (s, 0), (0, 0)],
    "7": [(0, s), (s, s), (s, 0), (s, -s)],
    "8": [(0, 0), (s, 0), (s, s), (0, s), (0, 0), (0, -s), (s, -s), (s, 0)],
    "9": [(s, 0), (s, s), (0, s), (0, 0), (s, 0), (s, -s), (0, -s)],
    "y": [(15, 0)],
    "m": [(15, 0)],
    "d": [(15, 0)]
}

def drawDigit(d, n):
    for point in digit[d]:
        turtle.goto(point[0] - 300 + 60 * n, point[1])
        turtle.pendown()

turtle.pensize(5)
turtle.hideturtle()
date = datetime.datetime.now().strftime('%Yy%mm%dd')
n = 0
for i in date:
    turtle.penup()
    if i == 'y':
        drawDigit(i, n)
        turtle.write('年', font=("Arial", 18, "normal"))
    elif i == "m":
        drawDigit(i, n)
        turtle.write('月', font=("Arial", 18, "normal"))
    elif i == "d":
        drawDigit(i, n)
        turtle.write('日', font=("Arial", 18, "normal"))
    else:
        drawDigit(i, n)
    n += 1
turtle.done()
