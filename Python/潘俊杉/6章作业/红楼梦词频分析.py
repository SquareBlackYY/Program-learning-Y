import jieba
import re
import matplotlib.pyplot as plt

text = open("红楼梦.txt", "r", encoding="utf-8").read()
new_text = re.sub(r"[\u3000-\u303F|\uFF00-\uFFEF|\W]+", "", text)

stopwords = open("stop_words.txt", "r", encoding="utf-8").read()
new_stopwords = re.findall(r"[\u4e00-\u9fa5]+", stopwords)

words = jieba.lcut(new_text)
item = list(set(words))
word_dir = {}
for word in item:
    if word not in new_stopwords:
        word_dir[word] = words.count(word)

result = list(word_dir.items())
result.sort(key=lambda x: x[1], reverse=True)

plt.rcParams["font.sans-serif"] = ["Microsoft Yahei"]
plt.bar([result[i][0] for i in range(20)], [result[i][1] for i in range(20)])
plt.title("《红楼梦》中词频最高的前20个词汇")
plt.show()
