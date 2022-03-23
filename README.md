# Parallel Cost-Bounded Graph Partitioning
Team Members: Abhishek Vijayakumar (abhishev@andrew.cmu.edu), Anna Cai (annacai@andrew.cmu.edu)

# Summary
We are going to implement a cost-bounded partitioning algorithm for weighted graphs and optimize it for parallelism across multicore CPUs. Given a graph with vertex weights, this algorithm creates a fixed number of graph partitions such that the sum of costs in each partition falls within a set interval. This algorithm can be used in the automatic division of precincts into voting districts, and is thus highly relevant to district map analysis.

# Background
describe algo and paper

redistricting applications (direct sampling as opposed to MCMC)

# The Challenge

## Workload
what are the dependencies, what are its memory access characteristics? (is there locality? is there a high communication to computation ratio?), is there divergent execution?
idk this requires thinking

## Constraints
What are the properties of the system that make mapping the workload to it challenging?

# Resources
https://link.springer.com/article/10.1007/s00453-010-9485-y
Partitioning a Weighted Tree into Subtrees with Weights in a Given Range
Ito et al.

We have previously implemented an inefficient, sequential version of this algorithm in Python at https://github.com/inkyubeytor/lupartition.

In order to test our parallel solution, we would require at a minimum access to the GHC machines. If possible, we would also like access to the PSC machines in order to test with higher thread counts.

# Goals and Deliverables

## 75% Goals
something parallel that has existent speedup tm??

## 100% Goals
have a parallel implementation that scales to at least 4/8 cores?

## 125% Goals
make it into a package? lol

# Platform Choice

Our first choice of language would be CPython, so that we could work off of our sequential solution and focus on parallelization. If this is not an option for our project, we plan to work in C++, as we have covered several implementations of parallelism in C++. We would use either MPI or OpenMP to parallelize our solution, depending on which we feel is more suited to the portions of the code we feel that we can parallelize effectively.

We plan to deploy our solution on multicore CPUs (our computers, the GHC machines, and PSC).

can we please have python .-.
if no python, c/c++ so we can make a python library out of it
we want redistricting researchers to be able to run it with the resources they usually use (i.e. personal computers, colab notebooks, university provided resources)

# Schedule
we definitely need to try to implement it in sequential first (unless we're allowed to do it in python)
then parallelize the actual algo
then maybe spanning tree shenanigans

