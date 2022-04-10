#include <vector>
using std::vector;
#include <utility>
using std::pair;
#include <unordered_map>
using std::unordered_map;
#include <unordered_set>
using std::unordered_set;

#include "lib/graph.cpp"


typedef float cost_t;

vector<int> naive_partition(Tree<cost_t> tree, int parts, cost_t lower, cost_t upper) {
    // first vector: vertex index v
    // second vector: child index i for that vertex
    // third vector: parts index k
    // unordered_map: z -> pair<s1, s2>
    // s1 maps k' -> z' (the z', k' pairs earlier)
    // s2 is just k',   (the None, k' pairs earlier)
    vector<vector<vector<
        unordered_map<cost_t,
            pair<
                unordered_map<int, unordered_set<cost_t>>,
                unordered_set<int>
            >
        >
    >>> dp_table;

    // TODO: initialize entire empty dp table

    unordered_set<int> processed;
    vector<int> postorder = tree.dfs_postorder_nodes();
    int root = postorder.back();
    for (int& v: postorder) {
        // create children set
        const vector<int>& neighbors = tree.neighbors(v);
        unordered_set<int> children;
        for (const int& neighbor: neighbors) {
            if (processed.count(neighbor)) children.insert(neighbor);
        }

        // create parts_0 dict


    }
    vector<int> out;
    return out;
}


// def naive_partition(tree, key, parts, lower, upper):
//    dp_tree = copy_partition(tree, key)
//
//    # dp_tree                          - whole tree
//    # dp_tree.nodes[v]                 - vertex v
//    # dp_tree.nodes[v]["table"][i] - vertex v, child i is a dict with values
//    #   "vertex": v_i,
//    #   "parts": k -> Dict { z -> Set{ (z', k') for s1 | (None, k') for s2 } } }
//
//    # Traverse the graph bottom up, defining children as
//    # already-processed neighbors
//    processed = set()
//    root = None
//    for v in nx.dfs_postorder_nodes(dp_tree):
//        children = set(dp_tree.neighbors(v)) & processed

//        parts_0 = {k: {} for k in range(2, parts + 1)}
//        parts_0[1] = {dp_tree.nodes[v]["weight"]: {(None, 0)}}
//        dp_tree.nodes[v]["table"] = [{"vertex": None, "parts": parts_0}]
//        for child in children:
//            parts_dict = {}
//            for k in range(1, parts + 1):
//                z_dict = {}
//                # S1 stuff
//                for k_prime in range(1, k + 1):
//                    left = dp_tree.nodes[v]["table"][-1]["parts"][k_prime]
//                    right = dp_tree.nodes[child]["table"][-1]["parts"][
//                        k - k_prime + 1]
//                    for a in left:
//                        for b in right:
//                            try:
//                                z_dict[a + b].add((a, k_prime))
//                            except KeyError:
//                                z_dict[a + b] = {(a, k_prime)}
//                # S2 stuff
//                for k_prime in range(1, k):
//                    left = dp_tree.nodes[v]["table"][-1]["parts"][k_prime]
//                    right = dp_tree.nodes[child]["table"][-1]["parts"][
//                        k - k_prime]
//                    for b in right:
//                        if lower <= b <= upper:
//                            for a in left:
//                                try:
//                                    z_dict[a].add((None, k_prime))
//                                except KeyError:
//                                    z_dict[a] = {(None, k_prime)}
//                parts_dict[k] = z_dict
//            dp_tree.nodes[v]["table"].append(
//                {"vertex": child, "parts": parts_dict})
//        processed.add(v)
//        root = v
//
//    # backtracking
//    try:
//        next(filter(lambda y: lower <= y <= upper,
//                    dp_tree.nodes[root]["table"][-1]["parts"][parts]))
//    except StopIteration:
//        return None
//
//    assignment = {root: 0}
//    input_queue = [(root, None, parts, len(dp_tree.nodes[v]["table"]) - 1, 0)]
//
//    def process(v, z, k, i, v_part_num):
//        if i == 0:
//            return
//
//        if z is None:  # make a new partition
//            z = next(filter(lambda y: lower <= y <= upper,
//                            dp_tree.nodes[v]["table"][i]["parts"][k]))
//
//        vp = dp_tree.nodes[v]["table"][i]["vertex"]
//        zp, kp = dp_tree.nodes[v]["table"][i]["parts"][k][z].pop()
//        if zp is None:
//            new_part_num = 1 + max(assignment.values(), default=0)
//            assignment[vp] = new_part_num
//            input_queue.append((v, z, kp, i - 1, v_part_num))
//            input_queue.append((vp, None, k - kp,
//                                len(dp_tree.nodes[vp]["table"]) - 1,
//                                new_part_num))
//        else:
//            assignment[vp] = v_part_num
//            input_queue.append((v, zp, kp, i - 1, v_part_num))
//            input_queue.append((vp, z - zp, k - kp + 1,
//                                len(dp_tree.nodes[vp]["table"]) - 1,
//                                v_part_num))
//
//    while input_queue:
//        process(*input_queue.pop())
//
//    print(assignment)
//    return assignment

int main() {
    vector<float> weights = {0.0, 1.0, 2.0, 3.0};
    vector<int> n0 = {1, 2};
    vector<int> n1 = {};
    vector<int> n2 = {3};
    vector<int> n3 = {};
    vector <vector<int>> adj = {n0, n1, n2, n3};
    Tree<float> tree(adj, weights, 0);
    vector<int> partition = naive_partition(tree, 2, 2.9, 3.1);
    return 0;
}
