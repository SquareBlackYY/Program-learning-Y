import re
import random

# 输入待处理文本
#text = input("请输入文本:")
text = ("aefwgrfbdvewgrhdtfngbdvsegrfbcxdvsegrfbcxdzscafedgrfbcxdzsefrgcvbxdzsefrvcxdzsefrgvbcxdfvgrcxdfgbvcxdfgbvcxdfbcgvd")
# 去除非字母字符
text = re.sub(r'[^A-Z]', '', text)
# 将文本转换为数字列表
text_nums = [ord(c) - ord('A') for c in text]
# 计算需要添加的填充字符个数
padding = len(text_nums) % 64
# 若长度不符则随机填充末尾
if padding > 0:
    text_nums += [random.randint(0, 25) for _ in range(64 - padding)]
# 将文本分段
text_slice = [text[i:i+64] for i in range(0, len(text), 64)]