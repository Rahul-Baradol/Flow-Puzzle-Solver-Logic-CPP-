# Flow Puzzle Solver in C++

## About solver.cpp
This is the optimized version as compared to other two solutions.
Here Connected Component Labelling (CCL) with Disjoint Set is being used along with recursion.
Here redundant calculations are avoided by checking if a board state is solvable further, and discarded 
beforehand if not solvable.

## About solver - recursive dfs.cpp

This is a brute-force solution that was wrote at the start of the project.
I used plain recursion to solve the puzzle which used to check all the possible board states and used to pick the one where the puzzle got solved.

## About solver - iterative dfs using stack.cpp

This is the same solution as in 'solver - recursive dfs.cpp' except
here i am simulating the dfs using stack. It is a little slow though even being iterative.