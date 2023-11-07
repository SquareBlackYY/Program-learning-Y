import re
import matplotlib.pyplot as plt
from collections import Counter

def plot_word_frequencies(filename, top_n=20):
    with open(filename, 'r', encoding='utf-8') as file:
        text = file.read()

    # 使用正则表达式匹配词语
    words = re.findall(r'\b\w+\b', text)

    # 去除没有语义的连词、介词等
    stopwords = ['的', '了', '和', '是', '在', '有', '我', '不', '他', '这', '为', '之', '以', '们', '于', '其', '中']
    words = [word for word in words if word not in stopwords]

    # 统计词频
    word_counts = Counter(words)

    # 获取词频最高的前top_n个词汇
    top_words = word_counts.most_common(top_n)
    top_words.reverse()  # 为了柱状图显示方便，将列表反转

    # 提取词汇和频次
    words, counts = zip(*top_words)

    # 绘制柱状图
    plt.barh(range(top_n), counts, tick_label=words)
    plt.xlabel('Frequency')
    plt.ylabel('Words')
    plt.title('Top 20 Word Frequencies in "红楼梦.txt"')
    plt.tight_layout()
    plt.show()

# 使用示例
filename = '红楼梦.txt'
plot_word_frequencies(filename)
