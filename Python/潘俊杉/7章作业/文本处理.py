import jieba
import re
import csv
import matplotlib.pyplot as plt
from wordcloud import WordCloud

text = open('野猪公主.txt', "r", encoding='utf-8').read()
txt = re.sub(r'[^\u4e00-\u9fa5]', '', text)
words = jieba.lcut(txt)
stopwords = open("stop_words.txt", 'r', encoding='utf-8').read()
item = list(set(words))
word = {}
for i in words:
    if i not in stopwords:
        word[i] = word.get(i, 0) + 1
result = sorted(word.items(), key=lambda x: x[1], reverse=True)
items = {}
for word, count in result:
    items[word] = count
with open('results.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    for row in items.items():
        writer.writerow(row)
num = 0
for word in items:
    if items.get(word) >= 50:
        num += 1
    else:
        continue
wordcloud = WordCloud(background_color='white',
                      font_path='C:/Windows/Fonts/simhei.ttf',
                      max_words=num,
                      max_font_size=100,
                      width=500,
                      height=500).generate_from_frequencies(items)
plt.figure(figsize=(8, 8))
plt.imshow(wordcloud, interpolation='bilinear')
plt.axis('off')
plt.show()