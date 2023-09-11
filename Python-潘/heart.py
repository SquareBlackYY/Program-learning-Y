import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 定义心形函数
def heart_func(x, y, z):
    return (x**2 + (9/4)*y**2 + z**2 - 1)**3 - x**2*z**3 - (9/80)*y**2*z**3

    # 生成网格点
    x, y, z = np.meshgrid(np.linspace(-1,1,200), np.linspace(-1,1,200), np.linspace(-1,1,200))
    f = heart_func(x, y, z)

    # 绘制3D图
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    contours = ax.contour(x[:,:,0], y[:,:,0], f[:,:,100], levels=[0], alpha=1, colors='red', linestyles='solid')
    for i in range(len(contours.collections)):
        p = contours.collections[i].get_paths()[0]
            v = p.vertices
                ax.plot(v[:,0], v[:,1], v[:,2], 'ko', markersize=3)
                plt.show()