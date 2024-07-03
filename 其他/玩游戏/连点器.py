import ctypes
import time

# 获取窗口句柄
def get_window_handle(window_title):
    return ctypes.windll.user32.FindWindowW(None, window_title)

# 将窗口设置为前台窗口
def set_foreground_window(hwnd):
    ctypes.windll.user32.SetForegroundWindow(hwnd)

# 发送鼠标左键点击事件
def click(hwnd, x, y):
    lParam = y << 16 | x
    ctypes.windll.user32.PostMessageW(hwnd, 0x0201, 0x0001, lParam)  # WM_LBUTTONDOWN
    ctypes.windll.user32.PostMessageW(hwnd, 0x0202, 0x0000, lParam)  # WM_LBUTTONUP

# 示例使用
window_title = "绝区零"  # 替换为你的游戏窗口标题
x, y = 1103, 947  # 替换为你想要点击的位置

while True:
    hwnd = get_window_handle(window_title)
    if hwnd:
        set_foreground_window(hwnd)
        time.sleep(4)  # 等待窗口激活
        click(hwnd, x, y)
    else:
        print("找不到窗口")

    print("点击完成")

