def max_score(score):
    return max(score)

def average_score(score):
    return sum(score) / len(score)

def median_score(score):
    return score[len(score) // 2] if len(score) % 2 == 1 else (score[len(score) // 2] + score[len(score) // 2 - 1]) / 2

score = sorted(list(map(float, input("请输入成绩，空格隔开：").split())))

print("最高分：", max_score(score))
print("平均分：", average_score(score))
print("中位数：", median_score(score))
