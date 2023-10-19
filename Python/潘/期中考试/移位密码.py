def encrypt(plain_text, shift):
    encrypted_text = ""
    for char in plain_text:
        encrypted_char = chr((ord(char) + shift) % 128)
    encrypted_text += encrypted_char
    return encrypted_text

plain_text = input("请输入明文:")
shift = int(input("请输入不大于26的数字x:"))

encrypted_text = encrypt(plain_text, shift)
print("密文:", encrypted_text)
