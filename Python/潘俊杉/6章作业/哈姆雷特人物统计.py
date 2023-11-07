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
txt = open('Hamlet.txt','r').read().lower()
txt = re.findall(r'[a-z]+',txt)
for i in characters:
    count[i] = txt.count(i)
print(dict(sorted(count.items())))