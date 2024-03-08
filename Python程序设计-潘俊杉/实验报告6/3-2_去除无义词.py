import re

with open("EnReport.txt", "r", encoding="utf-8") as text:
    text_list = re.findall(r"[a-zA-Z]+", text.read())
exclude = ["the", "and", "to", "of", "a", "in", "that", "not", "but", "with", "for", "he", "s", "it"]

word = {}
for i in text_list:
    if i not in exclude:
        word[i] = word.get(i, 0) + 1

result = list(word.items())
result.sort(key=lambda x: x[1], reverse=True)
for i in range(10):
    word, count = result[i]
    print("{:12}{:>5}".format(word, count))
