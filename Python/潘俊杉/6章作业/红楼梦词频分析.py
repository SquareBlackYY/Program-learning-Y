import jieba
import re

text = open('红楼梦.txt', "r", encoding='utf-8').read()
new_text = re.findall(r"[\u4e00-\u9fa5]+", text)
new_text = ''.join(new_text)

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
for i in range(10):
    word, count = result[i]
    print("{:10}{:10}".format(word, count))
