import re

with open("EnReport.txt", "r", encoding="utf-8") as text:
    text_list = re.findall(r"[a-zA-Z]+", text.read())

word = {}
for i in text_list:
    word[i] = word.get(i, 0) + 1

result = list(word.items())
result.sort(key=lambda x: x[1], reverse=True)
for i in range(10):
    word, count = result[i]
    print("{:12}{:>5}".format(word, count))
