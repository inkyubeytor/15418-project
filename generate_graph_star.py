import random

if __name__ == "__main__":
    num_branches_main = 20
    num_branches_side = 100
    num_nodes = 1 + num_branches_main + num_branches_main * num_branches_side
    print(num_nodes)
    print(random.random() * 4 + 1)
    for i in range(num_branches_main):
        print(random.random() * 4 + 1)
    for i in range(num_branches_main * num_branches_side):
        print(random.random() * 4 + 1)
    for i in range(num_branches_main):
        print(0, i + 1)
        for j in range(num_branches_side):
            print(i + 1, num_branches_main + 1 + num_branches_side * i + j)
