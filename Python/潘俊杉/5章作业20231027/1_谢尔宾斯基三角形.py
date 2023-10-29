import turtle

def draw_sierpinski(n, x, y, size):
    if n == 0:
        # 递归终止条件，当深度为0时，画一个等边三角形
        draw_triangle(x, y, size)
    else:
        # 递归调用，画三个小等边三角形
        draw_sierpinski(n-1, x, y, size/2)  # 左下角的小三角形
        draw_sierpinski(n-1, x + size/2, y, size/2)  # 右下角的小三角形
        draw_sierpinski(n-1, x + size/4, y + size/2, size/2)  # 顶部的小三角形

def draw_triangle(x, y, size):
    turtle.penup()
    turtle.goto(x, y)
    turtle.pendown()
    for _ in range(3):
        turtle.forward(size)
        turtle.right(120)

turtle.setup(800, 600)
turtle.speed(0)
draw_sierpinski(4, -200, -200, 400)
turtle.done()