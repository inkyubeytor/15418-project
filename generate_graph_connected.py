import itertools

if __name__ == "__main__":
    divisors = [2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 18, 20]
    num_nodes = sum(divisors)
    print(num_nodes)
    for d in divisors:
        weight = 720 // d
        for i in range(d):
            print(weight)
    for a, b in itertools.combinations(range(num_nodes), 2):
        print(a, b)
