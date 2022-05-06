from random import randint, uniform, seed
from typing import Dict, Hashable, List, Union

import networkx as nx


def generate_tree(nodes: int,
                  lower: Union[float, int],
                  upper: Union[float, int],
                  integral: bool,
                  key: str) \
        -> nx.Graph:
    """
    Generates a networkx vertex-weighted tree.
    :param nodes: The number of vertices to generate.
    :param lower: The (positive) lower bound on weight to randomly assign to
        the nodes.
    :param upper: The upper bound on weight to randomly assign to the nodes.
    :param integral: Whether to use integral weights.
    :param key: The key to use for node attributes.
    :return:
    """
    gen = randint if integral else uniform
    tree = nx.generators.random_tree(nodes, seed=15418)
    for vertex in tree.nodes:
        tree.nodes[vertex][key] = gen(lower, upper)
    return tree


if __name__ == "__main__":
    seed(15418)
    tree = generate_tree(200, 1, 9, True, "weight")
    print(len(tree.nodes))
    # print(" ".join([str(tree.nodes[v]["weight"]) for v in tree.nodes]))
    for v in tree.nodes:
        print(tree.nodes[v]["weight"])
    for a, b in tree.edges:
        print(a, b)
    nx.write_gpickle(tree, "med1.pkl")
