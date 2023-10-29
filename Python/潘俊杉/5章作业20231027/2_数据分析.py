def get_max_score(scores):
    return max(scores)

def get_average_score(scores):
    return sum(scores) / len(scores)

def get_median_score(scores):
    sorted_scores = sorted(scores)
    n = len(sorted_scores)
    if n % 2 == 0:
        # 列表长度为偶数，取中间两个数的平均值作为中位数
        return (sorted_scores[n//2-1] + sorted_scores[n//2]) / 2
    else:
        # 列表长度为奇数，直接取中间数作为中位数
        return sorted_scores[n//2]

scores = []
while True:
    score = input("请输入成绩（输入 q 结束）：")
    if score == 'q':
        break
    scores.append(float(score))

if scores:
    print("最高分：", get_max_score(scores))
    print("平均分：", get_average_score(scores))
    print("中位数：", get_median_score(scores))
else:
    print("没有输入成绩。")
