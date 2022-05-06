import networkx as nx
import matplotlib.pyplot as plt
import pydot
# import json


def read_tree(filename):
    with open(filename, "r") as f:
        lines = f.readlines()
    G = nx.Graph()
    num_nodes = int(lines[0])
    G.add_nodes_from(range(num_nodes))
    for i in range(num_nodes):
        G.nodes[i]["weight"] = float(lines[i + 1])
    for line in lines[num_nodes + 1:]:
        a, b = line.split()
        G.add_edge(int(a), int(b))
    return G


def read_assignment(filename):
    assignment = {}
    with open(filename, "r") as f:
        for line in f.readlines():
            node, part = line.split()
            assignment[int(node)] = int(part)
    return assignment


if __name__ == "__main__":
    colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', '#8c564b', '#e377c2', '#7f7f7f', '#bcbd22', '#17becf']
    g = nx.read_gpickle("large1.pkl")
    # g = read_tree("star_random.txt")
    assignment = read_assignment("large1_threads_reorder.txt")
    plt.figure(figsize=(12, 10))
    # nx.draw(g, nx.drawing.nx_pydot.graphviz_layout(g, "circo"),
    #         node_size=[g.nodes[node]["weight"]*2 for node in list(g)])
    nx.draw(g, nx.drawing.nx_pydot.graphviz_layout(g),
            node_size=[g.nodes[node]["weight"]*2 for node in list(g)],
            node_color=[colors[assignment[node] % len(colors)] for node in list(g)])
    plt.show()
    # with open("small1.json", "w") as f:
    #     json.dump(nx.readwrite.json_graph.cytoscape.cytoscape_data(g), f)
