import datetime
import turtle

digit = {
    "0": [(0, 40), (40, 40), (40, 0), (40, -40), (0, -40), (0, 0), (0, 40)],
    "1": [(40, 40), (40, 0), (40, -40)],
    "2": [(0, 40), (40, 40), (40, 0), (0, 0), (0, -40), (40, -40)],
    "3": [(0, 40), (40, 40), (40, 0), (0, 0), (40, 0), (40, -40), (0, -40)],
    "4": [(0, 40), (0, 0), (40, 0), (40, 40), (40, 0), (40, -40)],
    "5": [(40, 40), (0, 40), (0, 0), (40, 0), (40, -40), (0, -40)],
    "6": [(40, 40), (0, 40), (0, 0), (0, -40), (40, -40), (40, 0), (0, 0)],
    "7": [(0, 40), (40, 40), (40, 0), (40, -40)],
    "8": [(0, 0), (40, 0), (40, 40), (0, 40), (0, 0), (0, -40), (40, -40), (40, 0)],
    "9": [(40, 0), (40, 40), (0, 40), (0, 0), (40, 0), (40, -40), (0, -40)],
    "y": [(15, 0)],
    "m": [(15, 0)],
    "d": [(15, 0)]
}

def drawDigit(d, n):
    for point in digit[d]:
        turtle.goto(point[0] - 300 + 60 * n, point[1])
        turtle.pendown()

turtle.setup(800, 350, 200, 200)
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
