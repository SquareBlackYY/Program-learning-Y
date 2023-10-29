import turtle


def draw_sierpinski(n, x, y, size):
    if n == 0:
        draw_triangle(x + size / 4, y + size / 4 * pow(3, 0.5), size / 2)
    else:
        # 中心
        draw_triangle(x + size / 4, y + size / 4 * pow(3, 0.5), size / 2)
        # 左下角
        draw_sierpinski(n - 1, x, y, size / 2)
        # 右下角
        draw_sierpinski(n - 1, x + size / 2, y, size / 2)
        # 顶部
        draw_sierpinski(n - 1, x + size / 4, y + size / 4 * pow(3, 0.5), size / 2)


def draw_triangle(x, y, size, direct=-1, color="white"):
    turtle.penup()
    turtle.goto(x, y)
    turtle.pendown()
    turtle.fillcolor(color)
    turtle.begin_fill()
    for i in range(3):
        turtle.forward(size)
        turtle.seth((i + 1) * 120 * direct)
    turtle.end_fill()


turtle.speed(0)
draw_triangle(-200, -200, 400, 1, "red")
draw_sierpinski(4, -200, -200, 400)
turtle.done()
