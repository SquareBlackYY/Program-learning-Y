text = open("input.txt").read()
text = text.upper()
text.replace(t,' ')
ls = text.split()
lt = list(set(ls))
d = {}
for word_in_text in lt:
    d[word_in_text] = text.count(word_in_text)
print(d)