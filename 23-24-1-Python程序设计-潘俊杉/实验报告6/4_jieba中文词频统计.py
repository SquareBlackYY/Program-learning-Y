import jieba

with open("红楼梦.txt", "r", encoding="utf-8") as text:
    words = jieba.lcut(text.read())

word_dict = {}
for word in words:
    if len(word) == 1:
        continue
    elif word in ["贾母", "老太太", "老祖宗", "史太君"]:
        word_dict["贾母"] = word_dict.get("贾母", 0) + 1
    elif word in ["贾珍", '珍哥儿', '大哥哥']:
        word_dict["贾珍"] = word_dict.get("贾珍", 0) + 1
    elif word in ["贾政", "老爷"]:
        word_dict["贾政"] = word_dict.get("贾政", 0) + 1
    elif word in ["宝二爷", '宝玉', '贾宝玉']:
        word_dict["贾宝玉"] = word_dict.get("贾宝玉", 0) + 1
    elif word in ["王熙凤", "熙凤", "凤辣子", "贾琏"]:
        word_dict["王熙凤"] = word_dict.get("王熙凤", 0) + 1
    elif word in ['紫鹃', '鹦哥']:
        word_dict["紫鹃"] = word_dict.get("紫鹃", 0) + 1
    elif word in ['翠缕', '缕儿']:
        word_dict["翠缕"] = word_dict.get("翠缕", 0) + 1
    elif word in ['香菱', '甄英莲']:
        word_dict["香菱"] = word_dict.get("香菱", 0) + 1
    elif word in ['豆官', '豆童']:
        word_dict["豆官"] = word_dict.get("豆官", 0) + 1
    elif word in ["林黛玉", "潇湘妃子", "林丫头", "林妹妹", "黛玉"]:
        word_dict["林黛玉"] = word_dict.get("林黛玉", 0) + 1
    elif word in ["薛宝钗", "宝姑娘", "宝丫头", "蘅芜君", "宝姐姐", '宝钗']:
        word_dict["薛宝钗"] = word_dict.get("薛宝钗", 0) + 1
    elif word in ["甄费", '甄士隐']:
        word_dict["甄费"] = word_dict.get("甄费", 0) + 1

result = list(word_dict.items())
result.sort(key=lambda x: x[1], reverse=True)
for i in range(12):
    word, count = result[i]
    print("{:10}\t{:>5}".format(word, count))
