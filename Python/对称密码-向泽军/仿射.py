def affine_cipher(text, key_a, key_b, mode):
    result = ""

    for char in text:
        if char.isalpha():
            if char.isupper():
                base = ord('A')
            else: 
                base = ord('a')

            if mode == "e":
                encrypted_char = chr((key_a * (ord(char) - base) + key_b) % 26 + base)
            else:
                inverse_key_a = 0
                for i in range(26):
                    if (key_a * i) % 26 == 1:
                        inverse_key_a = i
                        break
                decrypted_char = chr((inverse_key_a * (ord(char) - base - key_b)) % 26 + base)

            result += encrypted_char if mode == "e" else decrypted_char
        else:
            result += char

    return result

while(True):
    key_a = input("请输入乘法密钥：")
    if key_a.isdigit() and eval(key_a) in [2, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25]:
        key_a = eval(key_a)
        break
    else:
        print("错误")
        continue

key_b = int(input("请输入加法密钥："))
mode = input("请选择操作（e:加密，d:解密）：")
text = input("请输入要加密或解密的文本：")

if mode == "e":
    result = affine_cipher(text, key_a, key_b, "e")
    print("加密结果：", result)
elif mode == "d":
    result = affine_cipher(text, key_a, key_b, "d")
    print("解密结果：", result)
else:
    print("无效的操作")
