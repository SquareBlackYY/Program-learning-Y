def replace_letters(input_file, output_file, replacements):
    with open(input_file, 'r', encoding='utf-8') as file:
        content = file.read()

    replaced_content = ''
    for char in content:
        if char.isalpha():
            if char.isupper():
                replaced_char = replacements.get(char.lower(), char)
                replaced_content += replaced_char.upper()
            else:
                replaced_char = replacements.get(char, char)
                replaced_content += replaced_char
        else:
            replaced_content += char

    with open(output_file, 'w') as file:
        file.write(replaced_content)

    print("替换完成！替换后的内容如下：")
    print(replaced_content)

# 替换映射关系，将26个字母替换成指定字母
replacements = {
    'a': 'd',
    'b': 's',
    'c': 'g',
    'd': 'j',
    'e': 'w',
    'f': 'b',
    'g': 'a',
    'h': 'y',
    'i': 'p',
    'j': 'h',
    'k': 'u',
    'l': 't',
    'm': 'q',
    'n': 'c',
    'o': 'p',
    'p': 'n',
    'q': 'm',
    'r': 'o',
    's': 'e',
    't': 'k',
    'u': 'f',
    'v': 'r',
    'w': 'i',
    'x': 'l',
    'y': 'v',
    'z': 'z',
}

input_file = 'text.txt'  # 输入文件名
output_file = 'output.txt'  # 输出文件名

replace_letters(input_file, output_file, replacements)