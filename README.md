# Parallel Cost-Bounded Graph Partitioning
Team Members: Abhishek Vijayakumar (abhishev@andrew.cmu.edu), Anna Cai (annacai@andrew.cmu.edu)

# Summary
We are going to implement a cost-bounded partitioning algorithm for weighted graphs and optimize it for parallelism across multicore CPUs. Given a graph with vertex weights, this algorithm creates a fixed number of graph partitions such that the sum of costs in each partition falls within a set interval. This algorithm can be used in the automatic division of precincts into voting districts, and is thus highly relevant to district map analysis.

# Background
The algorithm at its core is an adapation of a weighted tree partitioning algorithm by Ito et al. to general graphs through the initial computation of a spanning tree of the given graph (minimum or otherwise). The algorithm by Ito et al. to compute a partition of tree *T* with *p* connected components of weight bounded by (*l*, *u*) is as follows:

Define a LU partition of a tree to be one with at most *p* components where each component is connected and meets the weight bound, and a near-extendable partition to be one that meets all the requirements of a LU partition with the exception of the component containing the root, whose weight can be less than the lower bound. To compute the near-extendable partitions of a subtree at vertex *v*, we are given the set of near-extendable partitions for each child *v<sub>i</sub>* of *v*, where the children are arbitrarily numbered. Consider the tree with root *v* and including all subtrees at children numbered up to and including *i* and its "predecessor" (i need a better name for this), the same tree but up to child *i*-1. The near-extendable partitions of this tree can be computed by merging near-extendable partitions of the predecessor and near-extendable partitions of the subtree at *v<sub>i</sub>* such that the total number of components is at most *p*. These partitions can be cased on whether the component containing *v<sub>i</sub>* is merged with the component containing *v*. If yes, the merged component must meet the upper bound; otherwise, the component containing *v<sub>i</sub>* must meet both the lower and upper bounds.

## Pseudocode - TODO

# The Challenge

## Workload

The problem has very high divergent execution if the work division is done by dividing the graph to be partitioned. Each execution context would then be concerned with entirely different portions of the graph. If we divide work along the innermost loop, we may be able to leverage convergent execution at the cost of much higher communication (frequent parallel reductions).

The memory accesses of the program have potentially high locality. Because the algorithm builds up the partitioning iteratively by adding nodes, it will always add nodes on the frontier between the set of included nodes and the rest of the graph. If the graph is ordered such that adjacent nodes in the graph are located together in memory, adjacent memory accesses may occur very close to each other.

The amount of communication (both the scale and frequency) depend on the division of work. If work is divided along the innermost axis of parallelism (the computation for how to add the next node to a partition), there may be very high-frequency, fine-grained communication. If we instead partition work by dividing the graph to be partitioned, the communication will consist of aggregating the final sub-partitions, a much larger message size at a much smaller frequency.

## Constraints - TODO
What are the properties of the system that make mapping the workload to it challenging?

One constraint we have is in the system's representation of the graph. If we use a system with a 

# Resources
The paper from which the original algorithms come from is https://link.springer.com/article/10.1007/s00453-010-9485-y
Partitioning a Weighted Tree into Subtrees with Weights in a Given Range
Ito et al.

We have previously implemented the "naive," sequential version of this algorithm in Python at https://github.com/inkyubeytor/lupartition, and we will likely use this as our starting point.

In order to test our parallel solution, we would require at a minimum access to the GHC machines. If possible, we would also like access to the PSC machines in order to test with higher thread counts.

# Goals and Deliverables

## 75% Goals - TODO
something parallel that has existent speedup tm??

## 100% Goals - TODO
have a parallel implementation that scales to at least 4/8 cores?

"justification as to why you can achieve it"

demo: speedup graphs? demonstrate on real map(s)?

## 125% Goals - TODO
scale to even more cores (like PSC level number of cores)

analysis of different spanning tree selections on performance (map quality) and runtime (focus on runtime)

make it into a package? lol

# Platform Choice

Our first choice of language would be CPython, so that we could work off of our sequential solution and focus on parallelization. In this case, we would use the multiprocessing library. If this is not an option for our project, we plan to work in C++, as we have covered several implementations of parallelism in C++. We would use either MPI or OpenMP to parallelize our solution, depending on which we feel is more suited to the portions of the code we feel that we can parallelize effectively.

We plan to deploy our solution on multicore CPUs (our computers, the GHC machines, and PSC).


# Schedule - TODO: more details on weeks 2 and 3

Week of 2022-03-27: Create efficient sequential implementation. If in C++, prioritize the naive algorithm. If in Python, consider implementing the faster "interval set" algorithm as well.

Week of 2022-04-03: Implement parallelism in the computation for a single node.

Week of 2022-04-10 (Checkpoint): Experiment with different methods of work division for parallelism within the partition computation.

Week of 2022-04-17: Implement parallel minimum spanning tree computations and experiment with choosing spanning trees that make valid partitions more likely to exist.

Week of 2022-04-24: Implement parallelism by attempting to compute the problem for sub-partitions of the graph and combining them.

