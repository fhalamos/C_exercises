MPCS 51100 PSet 5
Starting Framework

Student Name: Felipe Alamos
Student Email: fhalamos@uchicago.edu

Notes:

#P1:
Work time serial:11.401428 sec, on small graph.

#P2:
Our dijkstra algorithms takes O((E+V)*LogV). E+V is the number of times we execute decreaseKey, which runs in LogV. If we do not use a priority queue, the algorithms would be O(V^2). 
If we run this algorithm V times (from all sources), but being able to paralleliz in p processors, each processor will run V/p times the algorithm. Hence, total complexity is O(V/p * (E+V)*LogV).

Work time parallel (openmp):6.708205 sec, on small graph.
The lack of syncronization between processes makes or parallel algorithm very efficient compared to the serial version.




--------------------

References:
#P1
-Used dijkstra.c from class dropbox as based
-Used https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/ for heap implementation

#P3
https://cse.buffalo.edu/faculty/miller/Courses/CSE633/Ye-Fall-2012-CSE633.pdf
http://heather.cs.ucdavis.edu/~matloff/OpenMP/Examples/NM/Dijkstra.c

#P4
-Used dfs.c from class dropbox as based
-https://www.geeksforgeeks.org/gcd-two-array-numbers/
