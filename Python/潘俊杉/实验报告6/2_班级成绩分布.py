import matplotlib.pyplot as plt

scores = [80, 72, 73, 9, 50, 72, 69, 88, 61, 77, 85, 81, 83, 88, 66, 76,
          65, 77, 91, 66, 97, 66, 63, 75, 66, 77, 69, 55, 63, 40, 78]
grade = {"100-90分": 0, "89-80分": 0, "79-70分": 0, "69-60分": 0, "59分及其以下": 0}
for score in scores:
    if  score >= 90:
        grade["100-90分"] += 1
    elif score >= 80:
        grade["89-80分"] += 1
    elif score >= 70:
        grade["79-70分"] += 1
    elif score >= 60:
        grade["69-60分"] += 1
    else:
        grade["59分及其以下"] += 1
grade_list = list(grade.items())

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False
plt.plot([grade_list[i][0] for i in range(len(grade_list))], [grade_list[i][1] for i in range(len(grade_list))], "-o", color="orange", label="人数")
plt.legend(loc='lower center')
plt.title("成绩分布图")
plt.show()
