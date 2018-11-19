MPCS 51100 PSet 2
Starting Framework

Include any notes on your implementation
as well as your results for problem 6
in this file.

Student Name: Felipe Alamos
Student Email: fhalamos@uchicago.edu

Notes:

-This is my first and only homework to be submitted within 72 hrs of the due date.

******************
Problem 6 Results:
******************

Naive BST Dictionary
Time to import (sec): 3.423149
Avg. time per find (us): 74.936320

Self Balancing BST Dictionary
Time to import (sec): 0.129016
Avg. time per find (us): 1.473660

Discussion:

Results behave as expected.

In the case of the Naive BST Dictionary, due to the fact that the list is ordered, the tree ends up being a Linked List. Hence, the trees height is O(n), so each insertion and each find operation take O(n). Total time to import alphabetized_dictionary is then O(n^2), and time to find an element is O(n).

In the case of the Self Balancing BST, due to the fact that the tree keeps balancing itself every time an insertion occurs, its height is kept of O(log n). Hence, time to insert an element is O(log n), so it takes O(n * log n) to import the whole alphabetized dictionary - which is clearly smaller in time than O(n^2). The time to find a single element is also dependent on the height of the tree, so it takes O(log n), which is smaller than O(n).

This explanation is consistent with the works times the program took to accomplish the operations under both trees.

*Write calculate_find_time in console to run the random find operations.

******************
Problem 8 Results:
******************

The implemented algorithm runs in time O(log(m*n))+O(m). It takes O(log(m*n)) to find the lower bounds elements, and we take O(m) to save the results for each array. This is clearly better than time O(m*log(n)) of the original algorithm.

Regarding to memory use, the algorithm creates, in addition to the previously used memory in the standard Grid struct, an array of n*m elements - called mega_array in the code. In this array we save nodes, one for each of the numbers of the different original arrays. Each of those nodes save some information (such as number, to which original array it belonged, and its position in that original array) and a list of m integers. This integers are the id of m closest nodes  to the node we are studying, one for each of the original arrays. . Hence, the new struct uses O(m.n) + O(m.n.m) = O(n.m^2) memory, which is bigger than the original O(m.n) memory used.    


******************
REFERENCES
******************


*P2
-Reference: https://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion/
-https://riveridea.wordpress.com/2015/06/16/iterative-method-to-check-if-the-binary-search-tree-is-valid/