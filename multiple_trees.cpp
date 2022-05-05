#include <iostream>
#include <vector>
using std::vector;
#include <numeric>
#include "omp.h"

#ifndef GRAPH
#define GRAPH
#include "lib/graph.cpp"
#endif

#ifndef PARTITION
#define PARTITION
#include "spanning_tree.cpp"
//#include "sequential.cpp"
//#include "parallelism_within_levels.cpp"
#include "omp_within_child.cpp"
#endif

vector<int> naive_partition_graph(Graph<float> graph, int max_iter, int parts, cost_t lower, cost_t upper, std::string assignment_filename) {
    for (int iter = 0; iter < max_iter; iter++) {
        Tree<float> tree = rand_st(graph);
        std::string new_filename = assignment_filename.substr(0, assignment_filename.length() - 4) + "_iter" + std::to_string(iter) + ".txt";
        vector<int> assignment = naive_partition(tree, parts, lower, upper, new_filename);
        if (*assignment.begin() != -1) {
            printf("success on iter %d\n", iter);
            return assignment;
        }
    }
    return {-1};
}
