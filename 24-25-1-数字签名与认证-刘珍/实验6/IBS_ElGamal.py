from charm.toolbox.pairinggroup import PairingGroup, G1, ZR, pair


class Signature:
    def __init__(self) -> None:
        self.R = None
        self.S = None

    def __str__(self) -> str:
        return f"IBS_ElGamal签名:\nR: {self.R}\nS: {self.S}"


class System:
    # 初始化
    def __init__(self) -> None:
        self.group = PairingGroup('SS512')
        # 系统主私钥 s, 属于 ZR 群
        self.s = self.group.random()
        # P 是 G1 的一个生成元
        self.P = self.group.random(G1)
        # P_pub 是系统的主公钥, 计算 s * P
        self.P_pub = self.s * self.P


class User:
    # 初始化
    def __init__(self, id: str, system: System) -> None:
        self.ID: str = id
        self.system: System = system
        # 用户ID的公开钥 Q_ID = H1(ID)
        self.Q_ID = self.system.group.hash(self.ID, G1)
        # 用户ID的秘密钥 d_ID = s * Q_ID
        self.d_ID = self.system.s * self.Q_ID

    # 签名
    def sign(self, msg: str) -> Signature:
        sig = Signature()
        # 选择随机数 k
        k = self.system.group.random()
        # 计算 R = k * P
        sig.R = k * self.system.P
        # 计算 H2(m)， x_R
        H2_m = self.system.group.hash(msg)
        x_R = int(sig.R.__str__().strip("[]").split(", ")[0])
        # 计算S = k^-1 * (H2(m) * P + x_R * d_ID)
        sig.S = (k ** -1) * (H2_m * self.system.P + self.d_ID ** x_R)
        return sig

    # 验签
    def verify(self, prover: 'User', msg: str, sig: Signature) -> bool:
        # 计算 H2(m)， x_R
        H2_m = self.system.group.hash(msg)
        x_R = int(sig.R.__str__().strip("[]").split(", ")[0])
        return pair(sig.R, sig.S) == (pair(self.system.P, self.system.P) ** H2_m) * (pair(self.system.P_pub, prover.Q_ID) ** x_R)


def IBS_ElGamal():
    # 初始化系统和用户
    system = System()
    prover = User("Alice", system)
    verifier = User("Bob", system)

    # 消息
    message = "基于身份的ElGamal签名测试"

    # 签名
    sig = prover.sign(message)
    print(sig)

    # 验签
    if verifier.verify(prover, message, sig):
        print("验签成功")


IBS_ElGamal()
