import time

for i in range(10):
    print(f"Progress: {i+1}/10", end="\r")
    time.sleep(1)

print("Done!")
