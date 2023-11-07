import re
characters = {
    "Hamlet",
    "Claudius",
    "Polonius",
    "Ofelia",
    "Laertes",
    "Horatio",
    "Rosencrantz",
    "Guildenstern",
    "Marcellus"
}
count = {}
txt = open('Hamlet.txt', 'r').read()
txt = re.findall(r'[a-zA-Z]+', txt.lower())
for i in characters:
    count[i] = txt.count(i.lower())
print(dict(sorted(count.items())))