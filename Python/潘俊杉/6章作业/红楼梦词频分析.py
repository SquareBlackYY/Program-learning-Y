import jieba
import re
import matplotlib.pyplot as plt

text = open('红楼梦.txt', "r", encoding='utf-8').read()
# new_text = re.findall(r"[\u4e00-\u9fa5]+", text)
# new_text = ''.join(new_text)
new_text = re.sub(r"[\u3000-\u303F|\uFF00-\uFFEF|\W]+", "", text)

stopwords = open('stop_words.txt', 'r', encoding='utf-8').read()
new_stopwords = re.findall(r"[\u4e00-\u9fa5]+", stopwords)

words = jieba.lcut(new_text)
item = list(set(words))
word_dir = {}
for word in item:
    if word not in new_stopwords:
        word_dir[word] = words.count(word)

result = list(word_dir.items())
result.sort(key=lambda x: x[1], reverse=True)
result_name = [result[i][0] for i in range(20)]
result_count = [result[i][1] for i in range(20)]

# 下面两段是网上的处理中文乱码，但是程序报错
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False
plt.bar(result_name, result_count)
plt.title("红楼梦中 词频最高的前20个词汇")
plt.show()
