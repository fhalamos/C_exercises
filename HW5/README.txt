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

#P3
======================++++++++++PENDIENTEEEEEEEEEEEE!!!

#P4
GCD of markov-graph is 2

#P5
markov-graph is irreductible

#P6

Stationary probabilities for 1000000 in the graph:

Pi_1: 0.010021, Pi_2: 0.009395, Pi_3: 0.010806, Pi_4: 0.008903, Pi_5: 0.009626, Pi_6: 0.009493, Pi_7: 0.008414, Pi_8: 0.009686, Pi_9: 0.011523, Pi_10: 0.011336, Pi_11: 0.010993, Pi_12: 0.009907, Pi_13: 0.010402, Pi_14: 0.010618, Pi_15: 0.010133, Pi_16: 0.009588, Pi_17: 0.009636, Pi_18: 0.009608, Pi_19: 0.009735, Pi_20: 0.009677, Pi_21: 0.010029, Pi_22: 0.011950, Pi_23: 0.011085, Pi_24: 0.010365, Pi_25: 0.008941, Pi_26: 0.011275, Pi_27: 0.010236, Pi_28: 0.009748, Pi_29: 0.009529, Pi_30: 0.010387, Pi_31: 0.010445, Pi_32: 0.010506, Pi_33: 0.007587, Pi_34: 0.010422, Pi_35: 0.010067, Pi_36: 0.010717, Pi_37: 0.009699, Pi_38: 0.010104, Pi_39: 0.010368, Pi_40: 0.008375, Pi_41: 0.009282, Pi_42: 0.009378, Pi_43: 0.010458, Pi_44: 0.008973, Pi_45: 0.010904, Pi_46: 0.009275, Pi_47: 0.010874, Pi_48: 0.011386, Pi_49: 0.009658, Pi_50: 0.008274, Pi_51: 0.010962, Pi_52: 0.009219, Pi_53: 0.010878, Pi_54: 0.009186, Pi_55: 0.009209, Pi_56: 0.008844, Pi_57: 0.010632, Pi_58: 0.009438, Pi_59: 0.011164, Pi_60: 0.011070, Pi_61: 0.010273, Pi_62: 0.008916, Pi_63: 0.009407, Pi_64: 0.010412, Pi_65: 0.010002, Pi_66: 0.010417, Pi_67: 0.008737, Pi_68: 0.010862, Pi_69: 0.010101, Pi_70: 0.008844, Pi_71: 0.009478, Pi_72: 0.009586, Pi_73: 0.011089, Pi_74: 0.010113, Pi_75: 0.010379, Pi_76: 0.010306, Pi_77: 0.010197, Pi_78: 0.009967, Pi_79: 0.011075, Pi_80: 0.010619, Pi_81: 0.009978, Pi_82: 0.009221, Pi_83: 0.009294, Pi_84: 0.009391, Pi_85: 0.010906, Pi_86: 0.010196, Pi_87: 0.009145, Pi_88: 0.009974, Pi_89: 0.010698, Pi_90: 0.010288, Pi_91: 0.010454, Pi_92: 0.011268, Pi_93: 0.009758, Pi_94: 0.010292, Pi_95: 0.010781, Pi_96: 0.009060, Pi_97: 0.009693, Pi_98: 0.009456, Pi_99: 0.010318, Pi_100: 0.008650


What do the answer to the previous two questions tell you about the values that you calculated?

We know that in an aperiodic and irreducible graph, stationaries probabilities exist. But, the graph we are studying is not aperiodic and still has stationary probabilities. Hence, in an irreducible graph, aperiodicity is a sufficient, but not necessary, condition for existance of stationary probabilities.

#P7
-Call the program through /p7 n, where n is the dimension of squared matrix A.
-The matrix A and vector b are generated automatically with random numbers. If they happen to generate a sistem of equation with infinite or non solutions, just run the program again. In most cases this will not happen.

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

#P5
-https://www.geeksforgeeks.org/connectivity-in-a-directed-graph/

#P7
-https://www.codewithc.com/c-program-for-gauss-elimination-method/
