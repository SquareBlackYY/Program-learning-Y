from charm.toolbox.pairinggroup import PairingGroup, ZR, G1, GT, pair
import hashlib

class Signature:
    def __init__(self):
        self.R = None
        self.S = None

    def __str__(self):
        return f"Signature R: {self.R}\nSignature S: {self.S}"

class System:
    def __init__(self):
        self.group = PairingGroup('SS512')
        # 初始化生成元和系统参数
        self.P = self.group.random(G1)
        self.s = self.group.random(ZR)
        self.P_pub = self.P ** self.s

class User:
    def __init__(self, user_id, system):
        self.id = user_id
        self.system = system
        # 将ID哈希到群元素Q_ID
        self.Q_ID = self.hash_to_element_G1(self.id)
        # 计算私钥d_ID = s * Q_ID
        self.d_ID = self.Q_ID ** self.system.s

    def createSignature(self, message, sig):
        # 选择随机数k
        k = self.system.group.random(ZR)
        # 计算R = k * P
        sig.R = self.system.P ** k
        # 计算H2(m)
        H2_m = self.hash_to_scalar_ZR(message)
        # 计算H2(m) * P
        H2_mP = self.system.P ** H2_m
        # 提取R的哈希值xR
        xR = self.hash_to_scalar_ZR(sig.R)
        # 计算S = k^-1 * (H2(m) * P + xR * d_ID)
        xR_dID = self.d_ID ** xR
        temp = H2_mP * xR_dID
        k_inv = k ** -1
        sig.S = temp ** k_inv

    def verifySignature(self, prover, message, sig):
        # 计算H2(m)
        H2_m = self.hash_to_scalar_ZR(message)
        # 计算H2(m) * P
        H2_mP = self.system.P ** H2_m
        # 提取R的哈希值xR
        xR = self.hash_to_scalar_ZR(sig.R)
        # 计算Q_ID^xR
        Q_ID_xR = prover.Q_ID ** xR
        # 计算右侧的临时值
        right_temp = H2_mP * Q_ID_xR
        # 左侧计算：e(R, S)
        left = pair(sig.R, sig.S)
        # 右侧计算：e(P, H2(m) * P + Q_ID^xR)
        right = pair(self.system.P, right_temp)
        # 比较左侧和右侧
        return left == right

    def hash_to_element_G1(self, data):
        h = hashlib.sha256(data.encode()).digest()
        return self.system.group.hash(h, G1)

    def hash_to_scalar_ZR(self, data):
        try:
            data_bytes = self.system.group.serialize(data)
        except Exception:
            data_bytes = data.encode()
        h = hashlib.sha256(data_bytes).digest()
        return self.system.group.hash(h, ZR)

def elgamalSignatureID():
    # 初始化系统和用户
    system = System()
    prover = User("Alice", system)
    verifier = User("Bob", system)

    # 需要签名的消息
    message = "这只是一个测试"

    # 创建签名并输出签名
    sig = Signature()
    prover.createSignature(message, sig)

    print("签名为:")
    print(sig)

    # 验证签名并输出结果
    if verifier.verifySignature(prover, message, sig):
        print("验签成功")
    else:
        print("验签失败")

if __name__ == "__main__":
    elgamalSignatureID()