# G1, G2是阶为素数q的椭圆曲线上的加法群
# GT是有限域上阶为q的乘法群
# ZR通常指的整数模q的环
from charm.toolbox.pairinggroup import PairingGroup, G1, G2, GT, ZR 

# 创建配对组，其中SS256是一种特定的配对群参数
# 通常在密码学中用于生成双线性对
# 这里的“SS”指的是“Smart and Sorensen”方法，一种基于椭圆曲线的加密协议
# 256 则指的是安全级别，即使用的素数的位数。
group = PairingGroup('SS512')

# 生成椭圆曲线群G1中元素
g = group.random(G1)

id = 'aaa'

# 将身份哈希成G1中元素
group.hash(id, G1)

class KGC:
    def __init__(self, group_obj):
        self.group = group_obj
        self.p = self.group.randomGen(G1)

    
