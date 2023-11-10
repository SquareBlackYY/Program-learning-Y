import turtle

def koch(size, n):
    if n == 0:
        turtle.fd(size)
    else:
        koch(size / 3, n - 1)
        turtle.left(60)
        koch(size / 3, n - 1)
        turtle.right(120)
        koch(size / 3, n - 1)
        turtle.left(60)
        koch(size / 3, n - 1)

turtle.setup(800, 800)
turtle.speed(20)
turtle.penup()
turtle.goto(-300, 200)
turtle.pendown()
turtle.pensize(2)
n = eval(input("请输入科赫雪花阶数:"))
for i in range(3):
    koch(500, n)
    turtle.right(120)
turtle.done()
