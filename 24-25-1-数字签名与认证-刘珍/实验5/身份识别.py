import hashlib
import math
import random


def string_to_int(s: str) -> int:
    """
    将字符串转换为大整数。通过将字符串编码为字节码，然后将这些字节码按大端字节序转换为整数。

    :param s: 要转换的字符串。

    :return: 转换后的大整数，其中字符串的每个字符都被视为对应的字节码，再按大端字节序组合成整数。
    """
    return int.from_bytes(s.encode(), 'big')


def int_to_string(n: int) -> str:
    """
    将大整数转换为字符串。通过将整数按照大端字节序转回字节码，再将字节码解码为字符串。

    :param n: 要转换的大整数。

    :return: 转换后的字符串，其中整数的每一部分都对应于一个字节，再解码为字符。
    """
    return n.to_bytes((n.bit_length() + 7) // 8, 'big').decode()


# 定义主机类
class Host:
    """
    表示协议中的一个主机对象。
    """

    # RSA 问题相关参数
    p: int = 97
    q: int = 89
    n: int = p * q

    def __init__(self, name: str) -> None:
        """
        初始化 Host 对象并为主机设置名称。

        :param name: 主机的名称，用于标识和区分不同主机。
        """

        self.name: str = name

        # 会话列表，键格式:"name protocol"
        self.session_list: dict[str, dict[str, int]] = {}
        self.trust_user: set[str] = set()

    def __str__(self) -> str:
        """
        返回用户对象的自定义字符串表示，用于 print() 输出。
        """

        # 处理信任用户列表
        if self.trust_user:
            trusted_users = '\n'.join([f"     - {user}" for user in self.trust_user])
        else:
            trusted_users = 'None'

        return (f"主机 [{self.name}]\n"
                f" - 身份识别信任列表：\n{trusted_users}\n")

    def send_message(self, recipient: 'Host', message: str) -> None:
        """
        向接收者发送消息。

        :param recipient: 接收消息的主机对象。
        :param message: 要发送的消息字符串，格式为 "协议名 步骤号 键1 值1 键2 值2 ..."，所有部分用空格分隔。
        """

        # 调用接收者的接收消息函数
        recipient.receive_message(self, message)

    def receive_message(self, sender: 'Host', message: str) -> None:
        """
        接收来自发送者的消息，并根据步骤号解析和处理该消息。

        :param sender: 发送消息的主机对象。
        :param message: 收到的消息字符串，格式为 "协议名 步骤号 键1 值1 键2 值2 ..."，所有部分用空格分隔。
        """

        # 将消息字符串按空格分割
        parts: list[str] = message.split()

        # 提取协议名、步骤号
        protocol : str = parts[0]
        step: int = int(parts[1])

        # 将键值对存入字典
        data: dict[str, str] = {}
        for i in range(2, len(parts), 2):
            data[parts[i]] = parts[i + 1]

        # 打印格式化的消息输出
        print(f"[{self.name}] 收到来自 {sender.name} 的消息：")
        print(f" - 协议： {protocol}")
        print(f" - 步骤： {step}")

        # 仅在 data 不为空时打印参数信息
        if data:
            print(" - 参数：")
            for key, value in data.items():
                print(f"     - {key} : {value}")

        # 用于存储对应用户和协议会话的相关数据
        session_data: dict[str, int] = self.session_list.setdefault(f"{sender} {protocol}", {})

        # 仅在 session_data 不为空时打印参数信息
        if session_data:
            print(f"[{self.name}] 本机当前会话参数：")
            for key, value in session_data.items():
                print(f"     - {key} : {value}")
        print()

        # 执行下一步操作
        if protocol == "Fiat-Shamir":
            response = self.fiat_shamir(sender.name, step, data, session_data)
        elif protocol == "Feige-Fiat-Shamir":
            response = self.feige_fiat_shamir(sender.name, step, data, session_data)
        elif protocol == "Guillou-Quisquater":
            response = self.guillou_quisquater(sender.name, step, data, session_data)
        else:
            response = ''

        # 如果 response 不为空字符串，则向对方发送回应
        if response != '':
            self.send_message(sender, response)

    def fiat_shamir(self, sender: str, step: int, data: dict[str, str], session_data: dict[str, int]) -> str:
        """
        Fiat-Shamir 身份识别协议

        :param sender: 与本对象进行协议的另一个对象。
        :param step: 当前的步骤号，必须是一个整数，指示协议的执行阶段。
        :param data: 一个包含键值对的字典，存储与该步骤相关的参数数据。
        :param session_data: 一个包含键值对的字典，存储协议相关数据。

        :return: 返回需要回复的消息。
        """

        protocol: str = "Fiat-Shamir"

        # 根据步骤号处理相应逻辑
        # Prover
        if step == 0:
            s = session_data['s'] = random.randint(1, self.n - 1)
            v = session_data['v'] = pow(s, 2, self.n)

            r = session_data['r'] = random.randint(1, self.n - 1)
            x: int = pow(r, 2, self.n)

            return f"{protocol} {step + 1} v {v} x {x}"

        # Verifier
        elif step == 1:
            session_data['v']: int = int(data['v'])
            session_data['x']: int = int(data['x'])

            random_bit = session_data['random_bit'] = random.randint(0, 1)

            return f"{protocol} {step + 1} random_bit {random_bit}"

        # Prover
        elif step == 2:
            random_bit: int = int(data['random_bit'])

            y: int = session_data['r'] if random_bit == 0 else session_data['r'] * session_data['s']

            return f"{protocol} {step + 1} y {y}"

        elif step == 3:
            y: int = int(data['y'])

            random_bit: int = session_data['random_bit']

            if pow(y, 2, self.n) == session_data['x'] if random_bit == 0 else session_data['x'] * session_data['v']:
                self.trust_user.add(f"{sender}({protocol})")

            return ''

        else:
            return ''

    def feige_fiat_shamir(self, sender: str, step: int, data: dict[str, str], session_data: dict[str, int]) -> str:
        """
        Feige-Fiat-Shamir 身份识别协议

        :param sender: 与本对象进行协议的另一个对象。
        :param step: 当前的步骤号，必须是一个整数，指示协议的执行阶段。
        :param data: 一个包含键值对的字典，存储与该步骤相关的参数数据。
        :param session_data: 一个包含键值对的字典，存储协议相关数据。

        :return: 返回需要回复的消息。
        """

        protocol: str = "Feige-Fiat-Shamir"

        # 根据步骤号处理相应逻辑
        # Prover
        if step == 0:
            k = session_data['k'] = random.randint(1, 10)

            for i in range(k):
                while True:
                    s: int = random.randint(1, self.n - 1)
                    if math.gcd(s, self.n) == 1:
                        session_data[f"s_{i}"]: int = s
                        session_data[f"v_{i}"]: int = pow(s, 2, self.n)
                        break

            r = session_data['r'] = random.randint(1, self.n - 1)
            x: int = pow(r, 2, self.n)

            return f"{protocol} {step + 1} x {x} k {k} " + " ".join(f"v_{i} {session_data[f'v_{i}']}" for i in range(k))

        # Verifier
        elif step == 1:
            k = session_data['k'] = int(data['k'])
            session_data['x']: int = int(data['x'])

            for i in range(k):
                session_data[f'v_{i}'] = int(data[f'v_{i}'])

            random_bits: int = 0
            for i in range(k):
                random_bits = (random_bits << 1) + random.randint(0, 1)
            session_data["random_bits"]: int = random_bits

            return f"{protocol} {step + 1} random_bits {bin(random_bits)[2:].zfill(k)}"

        # Prover
        elif step == 2:
            random_bits: int = int(data['random_bits'])

            y: int = session_data['r']

            k: int = session_data['k']
            for i in range(k):
                y = (y * pow(session_data[f"s_{i}"], (random_bits >> (k - i - 1)) & 1)) % self.n

            return f"{protocol} {step + 1} y {y}"

        # Verifier
        elif step == 3:
            y: int = int(data['y'])

            k: int = session_data['k']
            x: int = session_data['x']
            random_bits: int = session_data['random_bits']

            v_a_product: int = x
            for i in range(k):
                v_a_product = (v_a_product * pow(session_data[f'v_{i}'], (random_bits >> (k - i - 1)) & 1)) % self.n

            if pow(y, 2, self.n) == v_a_product:
                self.trust_user.add(f"{sender}({protocol})")

            return ''

        else:
            return ''

    def guillou_quisquater(self, sender: str, step: int, data: dict[str, str], session_data: dict[str, int]) -> str:
        """
        Guillou-Quisquater 身份识别协议

        :param sender: 与本对象进行协议的另一个对象。
        :param step: 当前的步骤号，必须是一个整数，指示协议的执行阶段。
        :param data: 一个包含键值对的字典，存储与该步骤相关的参数数据。
        :param session_data: 一个包含键值对的字典，存储协议相关数据。

        :return: 返回需要回复的消息。
        """

        protocol: str = "Guillou-Quisquater"

        # 根据步骤号处理相应逻辑
        # Prover
        if step == 0:
            e = session_data['e'] = 5
            r = session_data['r'] = random.randint(1, self.n - 1)

            x: int = pow(r, e, self.n)

            phi: int = (self.p - 1) * (self.q - 1)

            while True:
                # 在 name 后添加 4 位随机数字
                random_suffix = ''.join(random.choices("0123456789", k=4))
                name_with_suffix = f"{self.name}{random_suffix}"

                # 将 name_with_suffix 转换为整数并存储为 I_p
                I_p = session_data['I_p'] = int.from_bytes(name_with_suffix.encode(), byteorder="big")

                # 计算哈希后的 J_p，并取模
                J_p = int.from_bytes(hashlib.sha256(name_with_suffix.encode()).digest(), byteorder="big") % self.n

                # 检查是否与 phi 互素
                if math.gcd(J_p, phi) == 1:
                    session_data['J_p'] = J_p
                    break

            session_data['S_p'] = pow(J_p, -pow(e, -1, phi), self.n)

            return f"{protocol} {step + 1} I_p {I_p} x {x}"

        # Verifier
        elif step == 1:
            e = session_data['e'] = 5

            session_data['I_p'] = int(data['I_p'])
            session_data['x'] = int(data['x'])

            u = session_data['u'] = random.randint(1, e)

            return f"{protocol} {step + 1} u {u}"

        # Prover
        elif step == 2:
            u = session_data['u'] = int(data['u'])

            y = (session_data['r'] * pow(session_data['S_p'], u, self.n)) % self.n

            return f"{protocol} {step + 1} y {y}"

        # Verifier
        elif step == 3:
            y = session_data['y'] = int(data['y'])

            name_with_suffix = session_data['I_p'].to_bytes((session_data['I_p'].bit_length() + 7) // 8, byteorder="big").decode()
            J_p: int = int.from_bytes(hashlib.sha256(name_with_suffix.encode()).digest(), byteorder="big") % self.n

            if (pow(J_p, session_data['u'], self.n) * pow(y, session_data['e'], self.n)) % self.n == session_data['x']:
                self.trust_user.add(f"{sender}({protocol})")

            return ''

        else:
            return ''


# 创建两个用户
prover = Host('Prover')
verifier = Host('Verifier')

# 协议执行
verifier.send_message(prover, "Fiat-Shamir 0")
verifier.send_message(prover, "Feige-Fiat-Shamir 0")
verifier.send_message(prover, "Guillou-Quisquater 0")

# 状态输出
print(verifier)