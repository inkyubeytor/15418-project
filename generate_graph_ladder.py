if __name__ == "__main__":
    num_nodes = 10
    print(num_nodes)
    for i in range(num_nodes):
        if i % 2 == 0:
            print(10)
        else:  # odd
            print(7)
    for i in range(num_nodes // 2):
        print(2 * i, 2 * i + 1)
    for i in range(num_nodes - 2):
        print(i, i + 2)
