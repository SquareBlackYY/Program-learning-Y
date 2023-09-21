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
    'a': 'j',
    'b': 't',
    'c': 'h',
    'd': 'y',
    'e': 'q',
    'f': 's',
    'g': 'l',
    'h': 'c',
    'i': 'z',
    'j': 'd',
    'k': 'p',
    'l': 'e',
    'm': 'a',
    'n': 'x',
    'o': 'o',
    'p': 'k',
    'q': 'b',
    'r': 'f',
    's': 'v',
    't': 'n',
    'u': 'w',
    'v': 'g',
    'w': 'm',
    'x': 'i',
    'y': 'u',
    'z': 'r',
}

input_file = 'text.txt'  # 输入文件名
output_file = 'output.txt'  # 输出文件名

replace_letters(input_file, output_file, replacements)