import jieba

text = open('红楼梦.txt', "r", encoding='utf-8').read()

words = jieba.lcut(text)
word_dir = {}
for word in words:
    if len(word) == 1:
        continue
    elif word in ["贾母", "老太太", "老祖宗", "史太君"]:
        word_dir["贾母"] = word_dir.get("贾母", 0) + 1
    elif word in ["贾珍", '珍哥儿', '大哥哥']:
        word_dir["贾珍"] = word_dir.get("贾珍", 0) + 1
    elif word in ["贾政", "老爷"]:
        word_dir["贾政"] = word_dir.get("贾政", 0) + 1
    elif word in ["宝二爷", '宝玉', '贾宝玉']:
        word_dir["贾宝玉"] = word_dir.get("贾宝玉", 0) + 1
    elif word in ["王熙凤", "熙凤", "凤辣子", "贾琏"]:
        word_dir["王熙凤"] = word_dir.get("王熙凤", 0) + 1
    elif word in ['紫鹃', '鹦哥']:
        word_dir["紫鹃"] = word_dir.get("紫鹃", 0) + 1
    elif word in ['翠缕', '缕儿']:
        word_dir["翠缕"] = word_dir.get("翠缕", 0) + 1
    elif word in ['香菱', '甄英莲']:
        word_dir["香菱"] = word_dir.get("香菱", 0) + 1
    elif word in ['豆官', '豆童']:
        word_dir["豆官"] = word_dir.get("豆官", 0) + 1
    elif word in ["林黛玉", "潇湘妃子", "林丫头", "林妹妹", "黛玉"]:
        word_dir["林黛玉"] = word_dir.get("林黛玉", 0) + 1
    elif word in ["薛宝钗", "宝姑娘", "宝丫头", "蘅芜君", "宝姐姐", '宝钗']:
        word_dir["薛宝钗"] = word_dir.get("薛宝钗", 0) + 1
    elif word in ["甄费", '甄士隐']:
        word_dir["甄费"] = word_dir.get("甄费", 0) + 1

result = list(word_dir.items())
result.sort(key=lambda x: x[1], reverse=True)
for i in range(12):
    word, count = result[i]
    print("{:10}{:>10}".format(word, count))