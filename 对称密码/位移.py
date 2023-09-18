def caesar_cipher(text, key, mode):
    result = ""
    key = key % 26  # 确保密钥在0-25的范围内

    for char in text:
        if char.isalpha():  # 只对字母进行加密或解密
            if char.isupper():  # 大写字母
                base = ord('A')
            else:  # 小写字母
                base = ord('a')

            if mode == "encrypt":
                encrypted_char = chr((ord(char) - base + key) % 26 + base)
            else:
                encrypted_char = chr((ord(char) - base - key) % 26 + base)

            result += encrypted_char
        else:
            result += char

    return result


key = int(input("请输入密钥（整数）："))
mode = input("请选择操作（encrypt加密，decrypt解密）：")
text = input("请输入要加密或解密的文本：")

if mode == "encrypt":
    result = caesar_cipher(text, key, "encrypt")
    print("加密结果：", result)
elif mode == "decrypt":
    result = caesar_cipher(text, key, "decrypt")
    print("解密结果：", result)
else:
    print("无效的操作")
