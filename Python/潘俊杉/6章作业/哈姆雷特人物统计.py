import re
from collections import Counter

def count_character_occurrences(filename):
    character_names = {
        'Hamlet': '哈姆莱特',
        'Claudius': '克劳狄斯',
        'Polonius': '波洛捏斯',
        'Ofelia': '奥菲利娅',
        'Laertes': '雷欧提斯',
        'Horatio': '霍拉旭',
        'Rosencrantz': '罗生克兰',
        'Guildenstern': '盖登思邓',
        'Marcellus': '马赛洛'
    }

    with open(filename, 'r', encoding='utf-8') as file:
        text = file.read()

    # 使用正则表达式匹配人物名字
    pattern = r'\b(?:' + '|'.join(character_names.values()) + r')\b'
    matches = re.findall(pattern, text)

    # 统计人物名字出现的次数
    character_counts = Counter(matches)

    # 按姓名的字母顺序排序
    sorted_counts = sorted(character_counts.items(), key=lambda x: x[0])

    return sorted_counts

# 使用示例
filename = 'Hamlet.txt'
character_occurrences = count_character_occurrences(filename)
for character, count in character_occurrences:
    print(f'{character}: {count}')
