//
// Created by ancai on 4/11/2022.
//

#include <iostream>
#include <unordered_map>
using std::unordered_map;
#include <set>
using std::set;
#include <stack>
using std::stack;
#include <numeric>
#include <cstdlib>
#include <time.h>
#include "omp.h"

#ifndef GRAPH
#define GRAPH
#include "lib/graph.cpp"
#endif

#ifndef PARTITION
#define PARTITION
#include "spanning_tree.cpp"
//#include "sequential.cpp"
#include "parallelism_within_levels.cpp"
//#include "omp_within_child.cpp"
#endif

//#include "multiple_trees.cpp"
#include "multiple_trees_parallel.cpp"

#include "sample_graphs.cpp"


int test_graph() {
    Tree<float> T = get_T0();
    vector<int> traversal = T.dfs_postorder_nodes();
    for (int x: traversal) std::cout << x << " ";
    std::cout << std::endl;
    return 0;
}

int test_spanning_tree() {
    Graph<float> G0 = get_G0();
    Tree<float> BFSST = bfs_st(G0, 3);
    Tree<float> MST = min_st(G0, 3, simple_edge_weight);
    Tree<float> rand_ST = min_st(G0, 3, rand_edge_weight);
    return 0;
}

bool check_connectivity(Graph<float> G, set<int> nodes) {
    bool visited[G.size()];
    std::fill_n(visited, G.size(), false);
    int root = *nodes.begin();
    stack<int> frontier;
    frontier.push(root);
    visited[root] = true;
    size_t num_visited = 1;

    while (!frontier.empty()) {
        int node = frontier.top();
        frontier.pop();
        vector<int> neighbors = G.neighbors(node);
        for (int n : neighbors) {
            if (!visited[n] && nodes.find(n) != nodes.end()) {
                visited[n] = true;
                num_visited++;
                frontier.push(n);
            }
        }
    }
    return num_visited == nodes.size();
}

int check_partition(Graph<float> G, vector<int> assignment, int parts, cost_t lower, cost_t upper) {
    if (*assignment.begin() == -1) {
        printf("failed: no assignment\n\n");
        return -1;
    }

    rev_assignment_t rev_assign;
    for (size_t v = 0; v < assignment.size(); v++) {
        int part_num = assignment[v];
        if (part_num == -1) {
            printf("no assignment for vertex %lu\n", v);
//            printf("vertex %d cost: %f\n", v, G.weight(v));
//            printf("vertex %d neighbors: ", v);
//            for (auto vn: G.neighbors(v)) {
//                printf("%d ", vn);
//            }
//            printf("\n");
        }
        rev_assign[part_num].insert(v);
    }

    // check number of parts lol

    for (auto it = rev_assign.begin(); it != rev_assign.end(); ++it) {
        int part_num = it->first;
        set<int> nodes = it->second;
        if (!check_connectivity(G, nodes)) {
            printf("failed*: part %d not connected\n\n", part_num);
            return 1;
        }
        cost_t z = 0;
        for (auto node_it = nodes.begin(); node_it != nodes.end(); ++node_it) {
            z += G.weight(*node_it);
        }
        if (!(lower <= z && z <= upper)) {
            printf("failed*: part %d not in cost bound\n\n", part_num);
            return 1;
        }
    }
    printf("success\n");
    printf("\n");
    return 0;
}

int test_partition(Graph<cost_t> G, Tree<cost_t> T, int parts, cost_t lower, cost_t upper, std::string filename) {
    return check_partition(G, naive_partition(T, parts, lower, upper, filename), parts, lower, upper);
}

int test_partition_multiple(Graph<cost_t> G, int max_iter, int parts, cost_t lower, cost_t upper, std::string filename) {
    return check_partition(G, naive_partition_graph(G, max_iter, parts, lower, upper, filename), parts, lower, upper);
}

int test_sequential_simple() {
    printf("-----\n");
    printf("simple graphs\n");

    Graph<float> G0 = get_G0();
    Tree<float> T0 = get_T0();
    printf("test 0.1 (PASS):\n");
    test_partition(G0, T0, 3, 0.9, 3.1, "assignments/G0-1_simple.txt");

    Graph<float> G1 = get_G1();
    Tree<float> T1 = get_T1();
    printf("test 1.1 (PASS):\n");
    test_partition(G1, T1, 3, 4.0, 6.0, "assignments/G1-1_simple.txt");
    printf("test 1.2 (PASS):\n");
    test_partition(G1, T1, 2, 5.0, 10.0, "assignments/G1-2_simple.txt");
    printf("test 1.3 (FAIL):\n");
    test_partition(G1, T1, 1, 4.0, 6.0, "assignments/G1-3_simple.txt");
    printf("test 1.4 (FAIL):\n");
    test_partition(G1, T1, 4, 2.0, 3.0, "assignments/G1-4_simple.txt");
    printf("test 1.5 (FAIL):\n");
    test_partition(G1, T1, 5, 2.0, 3.0, "assignments/G1-5_simple.txt");

    Graph<float> G2 = get_G2();
    Tree<float> T2 = get_T2();
    printf("test 2.1 (PASS):\n");
    test_partition(G2, T2, 15, 1.0, 2.0, "assignments/G2-1_simple.txt");
    printf("test 2.2 (PASS):\n");
    test_partition(G2, T2, 5, 3.0, 3.0, "assignments/G2-2_simple.txt");
    printf("test 2.3 (FAIL):\n");
    test_partition(G2, T2, 4, 2.0, 3.0, "assignments/G2-3_simple.txt");

    Graph<float> G3 = get_G3();
    Tree<float> T3 = get_T3();
    printf("test 3.1 (PASS):\n");
    test_partition(G3, T3, 3, 5.5, 8.5, "assignments/G3-1_simple.txt");
    printf("test 3.2 (FAIL):\n");
    test_partition(G3, T3, 7, 0.5, 5.5, "assignments/G3-2_simple.txt");
    return 0;
}

int test_rand_st() {
    Graph<float> G0 = get_G0();
    Tree<float> T0 = rand_st(G0);
    printf("test 0.1 (PASS):\n");
    test_partition(G0, T0, 3, 0.9, 3.1, "assignments/G0-1_rand.txt");

    Graph<float> G1 = get_G1();
    Tree<float> T1 = rand_st(G1);
    printf("test 1.1 (PASS):\n");
    test_partition(G1, T1, 3, 4.0, 6.0, "assignments/G1-1_rand.txt");
    printf("test 1.2 (PASS):\n");
    test_partition(G1, T1, 2, 5.0, 10.0, "assignments/G1-2_rand.txt");
    printf("test 1.3 (FAIL):\n");
    test_partition(G1, T1, 1, 4.0, 6.0, "assignments/G1-3_rand.txt");
    printf("test 1.4 (FAIL):\n");
    test_partition(G1, T1, 4, 2.0, 3.0, "assignments/G1-4_rand.txt");
    printf("test 1.5 (FAIL):\n");
    test_partition(G1, T1, 5, 2.0, 3.0, "assignments/G1-5_rand.txt");

    Graph<float> G2 = get_G2();
    Tree<float> T2 = rand_st(G2);
    printf("test 2.1 (PASS):\n");
    test_partition(G2, T2, 15, 1.0, 2.0, "assignments/G2-1_rand.txt");
    printf("test 2.2 (PASS):\n");
    test_partition(G2, T2, 5, 3.0, 3.0, "assignments/G2-2_rand.txt");
    printf("test 2.3 (FAIL):\n");
    test_partition(G2, T2, 4, 2.0, 3.0, "assignments/G2-3_rand.txt");
    return 0;
}

int test_sequential_random() {
    printf("-----\n");
    printf("random graphs\n");

//    Graph<float> Gsmall1 = get_Gsmall1();
//    Tree<float> Tsmall1 = bfs_st(Gsmall1, 0);
//    printf("test small 1.1 (FAIL):\n");
//    test_partition(Gsmall1, Tsmall1, 5, 12.0, 20.0);
//    printf("test small 1.2 (PASS):\n");
//    test_partition(Gsmall1, Tsmall1, 5, 10.0, 22.0);
//    printf("test small 1.3 (FAIL):\n");
//    test_partition(Gsmall1, Tsmall1, 4, 15.0, 25.0);
//    printf("test small 1.4 (PASS):\n");
//    test_partition(Gsmall1, Tsmall1, 4, 10.0, 30.0);
//    printf("test small 1.5 (PASS):\n");
//    test_partition(Gsmall1, Tsmall1, 2, 35.0, 45.0);

    Graph<float> Gmed1 = get_Gmed1();
    Tree<float> Tmed1 = bfs_st(Gmed1, 0);
    printf("test med 1.1 (PASS):\n");
    test_partition(Gmed1, Tmed1, 10, 75.0, 125.0, "assignments/med1-1.txt");

    Graph<float> Glarge1 = get_Glarge1();
    Tree<float> Tlarge1 = bfs_st(Glarge1, 0);
//    printf("test large 1.1 (FAIL):\n");
//    test_partition(Glarge1, Tlarge1, 10, 900.0, 1100.0);
    printf("test large 1.2 (PASS):\n");
    test_partition(Glarge1, Tlarge1, 20, 400.0, 600.0, "assignments/large1-2.txt");
    return 0;
}

int test_nontree() {
//    Graph<float> Gsmall = get_Gconnectedsmall();
//    Tree<float> Tsmall = rand_st(Gsmall);
//    printf("test fully connected small:\n");
//    test_partition(Gsmall, Tsmall, 2, 19.5, 21.5);

//    Graph<float> Gmed = get_Gconnectedmed();
//    Tree<float> Tmed = rand_st(Gmed);
//    printf("test fully connected med:\n");
//    test_partition(Gmed, Tmed, 12, 719.5, 720.5);

    Graph<float> Gladder = get_Gladder500();
    Tree<float> Tladder = rand_st(Gladder);
    printf("test ladder 500:\n");
    test_partition(Gladder, Tladder, 50, 77.0, 93.0, "assignments/ladder500_rand.txt");
//    test_partition(Gladder, Tladder, 250, 16.0, 18.0, "assignments/ladder500_rand.txt");

//    Graph<float> Gladder = get_Gladder1000();
//    Tree<float> Tladder = rand_st(Gladder);
//    printf("test ladder 1000:\n");
//    test_partition(Gladder, Tladder, 500, 16.0, 18.0);
    return 0;
}

int test_multiple_trees() {
//    Graph<float> Gsmall = get_Gconnectedsmall();
//    printf("test fully connected small:\n");
//    test_partition_multiple(Gsmall, 8, 2, 19.5, 21.5, "assignments/connected_small_rand.txt");

//    Graph<float> Gmed = get_Gconnectedmed();
//    printf("test fully connected med:\n");
//    test_partition_multiple(Gmed, 40, 12, 400, 800, "assignments/connected_med_rand.txt");

    Graph<float> Gladder = get_Gladder500();
    printf("test ladder 500:\n");
    test_partition_multiple(Gladder, 40, 50, 77.0, 93.0, "assignments/ladder500_rand.txt");
    return 0;
}

int bench_tree() {
    Graph<float> Glarge1 = get_Glarge1();
    Tree<float> Tlarge1 = bfs_st(Glarge1, 0);
    printf("test large 1.1 (FAIL):\n");
    test_partition(Glarge1, Tlarge1, 10, 900.0, 1100.0, "assignments/large1-1.txt");
    printf("test large 1.2 (PASS):\n");
    test_partition(Glarge1, Tlarge1, 20, 400.0, 600.0, "assignments/large1-2.txt");

    Graph<float> Glarge2 = get_Glarge2();
    Tree<float> Tlarge2 = bfs_st(Glarge2, 0);
    printf("test large 2.1 (PASS):\n");
    test_partition(Glarge2, Tlarge2, 20, 400.0, 600.0, "assignments/large2-1.txt");

    Graph<float> Glarge3 = get_Glarge3();
    Tree<float> Tlarge3 = bfs_st(Glarge3, 0);
    printf("test large 3.1 (PASS):\n");
    test_partition(Glarge3, Tlarge3, 20, 300.0, 600.0, "assignments/large3-1.txt");

    Graph<float> Gladder1000 = get_Gladder1000();
    Tree<float> Tladder1000 = bfs_st(Gladder1000, 0);
    printf("test ladder 1000 (PASS):\n");
    test_partition(Gladder1000, Tladder1000, 30, 200.0, 400.0, "assignments/ladder1000.txt");

    return 0;
}

int main() {
    omp_set_num_threads(8);
    srand(time(NULL));
//    printf("testing graph\n");
//    test_graph();
//    printf("testing spanning tree\n");
//    test_spanning_tree();
//    printf("testing random spanning tree\n");
//    test_rand_st();
    printf("testing sequential algo\n");
//    test_sequential_simple();
//    test_sequential_random();
    bench_tree();
//    printf("testing non-tree graphs\n");
//    test_nontree();
//    test_multiple_trees();
    return 0;
};
