#哈姆雷特词频统计
import re
characters = {
    "hamlet",
    "claudius",
    "polonius",
    "ofelia",
    "laertes",
    "horatio",
    "rosencrantz",
    "guildenstern",
    "marcellus"
}
count = {}
txt = open('哈姆雷特.txt','r').read().lower()
txt = re.findall(r'[a-z]+',txt)
for i in characters:
    count[i] = txt.count(i)
print(dict(sorted(count.items())))