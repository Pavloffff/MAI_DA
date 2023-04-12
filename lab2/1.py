import random

for i in range(10):
    letter = chr(random.randint(97, 122))
    number = random.randint(1, 100)
    print("+", letter, number)