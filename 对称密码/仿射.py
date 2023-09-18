def affine_cipher(text, key_a, key_b, mode):
    result = ""

    for char in text:
        if char.isalpha():  # 只对字母进行加密或解密
            if char.isupper():  # 大写字母
                base = ord('A')
            else:  # 小写字母
                base = ord('a')

            if mode == "encrypt":
                encrypted_char = chr((key_a * (ord(char) - base) + key_b) % 26 + base)
            else:
                # 计算乘法逆元
                inverse_key_a = 0
                for i in range(26):
                    if (key_a * i) % 26 == 1:
                        inverse_key_a = i
                        break
                decrypted_char = chr((inverse_key_a * (ord(char) - base - key_b)) % 26 + base)

            result += encrypted_char if mode == "encrypt" else decrypted_char
        else:
            result += char

    return result


key_a = int(input("请输入乘法密钥（整数）："))
key_b = int(input("请输入加法密钥（整数）："))
mode = input("请选择操作（encrypt加密，decrypt解密）：")
text = input("请输入要加密或解密的文本：")

if mode == "encrypt":
    result = affine_cipher(text, key_a, key_b, "encrypt")
    print("加密结果：", result)
elif mode == "decrypt":
    result = affine_cipher(text, key_a, key_b, "decrypt")
    print("解密结果：", result)
else:
    print("无效的操作")
