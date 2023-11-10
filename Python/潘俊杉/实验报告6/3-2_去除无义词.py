import re

text = open("EnReport.txt", "r", encoding="utf-8").read()
exclude = ["the", "and", "to", "of", "a", "in", "that", "not", "but", "with", "for", "he", "s", "it"]
ls = re.findall(r"[a-zA-Z]+", text)
item = list(set(ls))
word_dict = {}
for word in item:
    word_dict[word] = ls.count(word)
for word in exclude:
    del word_dict[word]
result = list(word_dict.items())
result.sort(key=lambda x: x[1], reverse=True)
for i in range(10):
    word, count = result[i]
    print("{:15}{:>5}".format(word, count))
