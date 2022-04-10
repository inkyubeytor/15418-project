#include <vector>
using std::vector;
#include <utility>
using std::pair;
#include <unordered_map>
using std::unordered_map;

#include "lib/graph.cpp"

typedef cost_t float;

vector<int> naive_partition(Tree<cost_t> T, int parts, cost_t lower, cost_t upper) {
    // first vector: vertex index v
    // second vector: child index i for that vertex
    // third vector: parts index k
    vector<vector<vector<unordered_map<float, >>>>
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