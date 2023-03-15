import random

test_cases = []
for i in range(1000000):
    num = random.randint(1, 999999)
    string = ''.join(random.choices('abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789', k=64))
    test_cases.append((num, string))

with open('t.txt', 'w') as f:
    for i, test_case in enumerate(test_cases):
        f.write(f'{test_case[0]}\t{test_case[1]}\n')