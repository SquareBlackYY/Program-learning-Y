import csv

def getScore():
    with open("成绩表.csv", "r", encoding="utf-8") as file:
        print(list(csv.reader(file)))

def countTotal():
    with open("成绩表.csv", "r", encoding="utf-8") as file:
        scores = list(csv.reader(file))
    scores[0].append('总评成绩')
    for i in range(len(scores) - 1):
        scores[i + 1].append(round(int(scores[i + 1][2]) * 0.2 + int(scores[i + 1][3]) * 0.2 + int(scores[i + 1][4]) * 0.6, 1))
    for i in range(len(scores)):
        print(scores[i])

def getRank():
    with open("成绩表.csv", "r", encoding="utf-8") as file:
        scores = list(csv.reader(file))
    scores[0].append('总评成绩')
    for i in range(len(scores) - 1):
        scores[i + 1].append(round(int(scores[i + 1][2]) * 0.2 + int(scores[i + 1][3]) * 0.2 + int(scores[i + 1][4]) * 0.6, 1))
    scores[0].append('排名')
    header = scores[0]
    scores = scores[1:]
    scores.sort(key=lambda x: (x[5], int(x[3])), reverse=True)
    for i, row in enumerate(scores):
        row.append(i + 1)
    scores.insert(0, header)
    for i in range(len(scores)):
        print(scores[i])

def writeRank():
    with open("成绩表.csv", "r", encoding="utf-8") as file:
        scores = list(csv.reader(file))
    scores[0].append('总评成绩')
    for i in range(len(scores) - 1):
        scores[i + 1].append(round(int(scores[i + 1][2]) * 0.2 + int(scores[i + 1][3]) * 0.2 + int(scores[i + 1][4]) * 0.6, 1))
    scores[0].append('排名')
    header = scores[0]
    scores = scores[1:]
    scores.sort(key=lambda x: (x[5], int(x[3])), reverse=True)
    for i, row in enumerate(scores):
        row.append(i + 1)
    scores.insert(0, header)
    with open("排名成绩表.csv", "w", encoding="utf-8", newline="") as file:
        writer = csv.writer(file)
        for row in scores:
            writer.writerow(row)

writeRank()
