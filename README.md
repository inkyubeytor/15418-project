# Parallel Tree Cost-Bounded Partitioning
Team Members: Abhishek Vijayakumar (abhishev@andrew.cmu.edu), Anna Cai (annacai@andrew.cmu.edu)

# Summary
We are going to implement a cost-bounded partitioning algorithm for weighted trees and optimize it for parallelism across multicore CPUs. This algorithm has high potential for application to voting district map analysis.

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

our LU partition code

GHC machines?

# Goals and Deliverables

## 75% Goals
something parallel that has existent speedup tm??

## 100% Goals
have a parallel implementation that scales to at least 4/8 cores?

## 125% Goals
make it into a package? lol

# Platform Choice
can we please have python .-.
if no python, c/c++ so we can make a python library out of it
we want redistricting researchers to be able to run it with the resources they usually use (i.e. personal computers, colab notebooks, university provided resources)

# Schedule
we definitely need to try to implement it in sequential first (unless we're allowed to do it in python)
then parallelize the actual algo
then maybe spanning tree shenanigans

