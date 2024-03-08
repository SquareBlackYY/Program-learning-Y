import turtle as t

def drawSquare(size):
    for _ in range(4):
        t.fd(size)
        t.left(90)

def drawRose(x, y, level, size):
    t.penup()
    t.goto(x, y)
    t.pendown()
    if level % 2 == 0:
        t.right(45)
        x = x + size / 2
        y = y - size / 2
    else:
        t.left(45)
        x = x - size / pow(2, 0.5)
    drawSquare(size)
    if level != 1:
        drawRose(x, y, level - 1, size * pow(2, 0.5))

t.left(45)
drawRose(0, 0, 6, 20)
t.done()
