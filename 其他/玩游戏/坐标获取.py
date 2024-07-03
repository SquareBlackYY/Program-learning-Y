import pyautogui
import keyboard

def print_mouse_position():
    while True:
        if keyboard.is_pressed('p'):  # 按下 'p' 键打印当前位置
            x, y = pyautogui.position()
            print(f"当前鼠标位置: ({x}, {y})")

print("移动鼠标到目标位置并按下 'p' 键以获取坐标。")
print_mouse_position()