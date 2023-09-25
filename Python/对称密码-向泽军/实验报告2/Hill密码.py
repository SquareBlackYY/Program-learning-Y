import numpy as np

def encrypt_hill_cipher(plaintext, key):
    # 将明文转换为大写，并去除空格和标点符号
    plaintext = plaintext.upper().replace(" ", "").replace(".", "").replace(",", "")

    # 补齐明文长度为3的倍数
    while len(plaintext) % 3 != 0:
        plaintext += "X"

    # 将明文分组为3个字母一组
    groups = [plaintext[i:i+3] for i in range(0, len(plaintext), 3)]

    # 加密每个分组
    ciphertext = ""
    for group in groups:
        # 将分组转换为向量
        vector = np.array([ord(ch) - ord("A") for ch in group])

        # 使用密钥矩阵进行加密
        encrypted_vector = np.dot(key, vector) % 26

        # 将加密后的向量转换为字母
        encrypted_group = "".join([chr(val + ord("A")) for val in encrypted_vector])

        ciphertext += encrypted_group

    return ciphertext

def decrypt_hill_cipher(ciphertext, key):
    # 计算密钥矩阵的逆矩阵
    inv_key = np.linalg.inv(key)
    inv_key = np.round(inv_key * np.linalg.det(key)).astype(int) % 26

    # 将密文分组为3个字母一组
    groups = [ciphertext[i:i+3] for i in range(0, len(ciphertext), 3)]

    # 解密每个分组
    plaintext = ""
    for group in groups:
        # 将分组转换为向量
        vector = np.array([ord(ch) - ord("A") for ch in group])

        # 使用逆矩阵进行解密
        decrypted_vector = np.dot(inv_key, vector) % 26

        # 将解密后的向量转换为字母
        decrypted_group = "".join([chr(val + ord("A")) for val in decrypted_vector])

        plaintext += decrypted_group

    return plaintext

# 定义密钥矩阵
key = np.array([[6, 24, 1], [13, 16, 10], [20, 17, 15]])

# 明文
plaintext = "There will be people willing to help you ward off all the evil in the world."

# 加密
ciphertext = encrypt_hill_cipher(plaintext, key)
print("密文:", ciphertext)

# 解密
decrypted_plaintext = decrypt_hill_cipher(ciphertext, key)
print("解密后的明文:", decrypted_plaintext)
