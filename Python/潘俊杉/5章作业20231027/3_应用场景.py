students = [
    {'name': 'Alice', 'age': 18, 'score': 85},
    {'name': 'Bob', 'age': 17, 'score': 92},
    {'name': 'Charlie', 'age': 16, 'score': 88},
    {'name': 'David', 'age': 19, 'score': 90},
]

sorted_students = sorted(students, key=lambda x: x['score'], reverse=True)

sorted_names = [student['name'] for student in sorted_students]
print(sorted_names)
