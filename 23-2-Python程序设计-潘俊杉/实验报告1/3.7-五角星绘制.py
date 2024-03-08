from turtle import *

fillcolor("red")
begin_fill()
while True:
    fd(200)
    right(144)
    if abs(pos()) < 1:
        break
end_fill()
done()
