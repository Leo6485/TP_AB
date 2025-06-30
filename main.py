machines = [
[5, 3, 2, 4],
[2, 4, 3, 6],
[4, 6, 5, 2]
]

def calc(path, machines):
    result = [0]*5
    for item in path:
        result[0] = result[0] + machines[item][0]
        for i in range(1, 4):
            result[i] = max(result[i-1] + machines[item][i], result[i+1])
        print(result)

calc([2, 1, 0], machines)