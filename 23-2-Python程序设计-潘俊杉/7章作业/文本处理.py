import jieba
import re
import csv
import matplotlib.pyplot as plt
from wordcloud import WordCloud

with open("红楼梦.txt", "r", encoding="utf-8") as file:
    words = jieba.lcut(re.sub(r"[^\u4e00-\u9fa5]", "", file.read()))

with open("stop_words.txt", "r", encoding="utf-8") as file:
    stop_words = file.read()

word = {}
for i in words:
    if i not in stop_words:
        word[i] = word.get(i, 0) + 1
word = dict(sorted(word.items(), key=lambda x: x[1], reverse=True))

with open("results.csv", "w", encoding="utf-8", newline="") as file:
    writer = csv.writer(file)
    for row in word.items():
        writer.writerow(row)

count = 0
for key, value in word.items():
    if value > 50:
        count += 1

wordcloud = WordCloud(
    background_color="white",
    font_path="C:/Windows/Fonts/simhei.ttf",
    max_words=count,
    max_font_size=100,
    width=500,
    height=500,
).generate_from_frequencies(word)
plt.imshow(wordcloud, interpolation="bilinear")
plt.axis("off")
plt.show()
