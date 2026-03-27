path = input('Введите путь (input.txt): ')
if path  == '': path = 'input.txt'

with open(path) as f:
    numbers = list(map(int, f.readline().split()))
    print(numbers)
    print('Сумма:', sum(numbers))
    print('Количество:', len(numbers))
    print('Минимум:', min(numbers))
    print('Максимум:', max(numbers))
    print('Среднее:', round(sum(numbers) / len(numbers), 2))
