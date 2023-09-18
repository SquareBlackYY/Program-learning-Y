def caesar_cipher(text, key, mode):
    result = ""
    key = key % 26

    for char in text:
        if char.isalpha():
            if char.isupper():
                base = ord('A')
            else:
                base = ord('a')

            if mode == "e":
                encrypted_char = chr((ord(char) - base + key) % 26 + base)
            else:
                encrypted_char = chr((ord(char) - base - key) % 26 + base)

            result += encrypted_char
        else:
            result += char

    return result


key = int(input("请输入密钥："))
mode = input("请选择操作（e:加密，d:解密）：")
text = input("请输入要加密或解密的文本：")

if mode == "e":
    result = caesar_cipher(text, key, "e")
    print("加密结果：", result)
elif mode == "d":
    result = caesar_cipher(text, key, "d")
    print("解密结果：", result)
else:
    print("无效的操作")
