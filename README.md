# Project-Euler
## Project Euler #250

This is my solution to Project Euler #250 problem version posted on HackerRank.
Link : https://www.hackerrank.com/contests/projecteuler/challenges/euler250/problem

The problem is to find the number of non-empty subsets from 1<sup>1</sup> to N<sup>N</sup> or {1<sup>1</sup>, 2<sup>2</sup>, 3<sup>3</sup>, ....N<sup>N</sup>}, the sum of whose elements is divisible
by some given K.

1 <= N <= 10<sup>400</sup>    so for biggest N a set would be {1<sup>1</sup>, 2<sup>2</sup>, 3<sup>3</sup>, ....(10<sup>400</sup>)<sup>10<sup>400</sup></sup>}

3 <= K <= 50

There are few main mathematical insights to be made here some somewhat obvious, other less so:
- We are dealing with numerous subsets of a large set, but we do not need any data about each subset except its sum. 
  - So storing and remembering subsets is unnecessary, and would be impossible (millions of subsets, each containing thousands or millions
		of numbers). **All you need to store/remember are subset sums.**
- Furthermore we do not need all the sums as well as we are only interested in count of sums that are divisible by K. Of course storing count of only
	divisible sums is not possible because a sum of some subset that is not divisible can form a divisible sum when new number is introduced and
	new subset formed with old one. But we still need to **store only the sums of all modulus values of K.** 
	- For instance if K = 8 and subset sum is 39, when we add 4 as a new number, we get 43. *43 % 8 = 3*. But we could have already had 
		modulus of 39 beforehand as a sum with the same final result. 39 % 8 = 7 so at the begining our subset sum is 7. We add 4 to subset, 
		thus new sum is 7 + 4 = 11. And *11 % 8 = 3*. Same thing.  *Modulo operations are distributive.*
  - So instead of storing counters for milions of diferent sums, we only need to store counters for numbers up to max K, which is 50 here.
- **Bottom line, instead of storing billions of subsets which would be unfeasable, or counters for billions of sums, we only need to store maximum 50 counters.** 
- Each new *a<sub>th</sub>* number added to form subsets(their sums) is *a<sup>a</sup>*. By the same rule as before, we only need the mod K of that number. But to
	get the modulo you have to calculate the number first. There are optimized methods of calculating power using memoization,
	which do not require exponent amount of multiplications, but require logBase2(exponent) + 1 number of multiplications. 
	So to calculate 5<sup>32</sup> we do not need 32 but 6 number of multiplications. These techniques are already included in language compiler,
	but not their combination with modulus operation in each step, which increases number of mathematical operations, 
	but reduces complexity of multiplication and thus increases speed for larger numbers.
  - More importantly, as we transform each new added number into a modulus of K, if we keep track of those values (n<sup>n</sup> mods), **a repeating** 
	**pattern can be noticed**, for some K values larger, for some small. For instance, pattern for K = 21 is size 43, for K = 47 pattern is size 2163. 
    **- So once we locate the pattern, calculating powers and mods for new numbers is unnecessary, we can just repeatedly cycle numbers in a pattern.** 
    And considering that max N could be huge (10<sup>400</sup>) that's a good and necessary optimization for better results.
- All that considering, number of iterations can still be huge and have to be counted somehow so a BigInteger custom type should be used. I haven't done this yet.
	 
Some comments regarding code:

- **Why 2d array for sum counts?** As values in array of sum counts (sumsCounter) have to be updated from that same array but different index 
in each iteration, if only one array would be used that would invalidate the array and result in incorrect values. 
	- One way to deal with this usualy is to create before each iteration a temporary array that is a copy of original one, so original array 
is updated from temporary array which remains unchanged during iteration. While this results in correct values, if you have to perform 
many iteration cycles it is very suboptimal and time consuming since for each iteration cycle you have to fill temporary
array with values from original array all over again. A lot of copy operations performed (N*numberOfIterationCycles) where N is the size of 
original array and number of iteration cycles can be milions.
	- So this is the other way of resolving this problem in a more optimal way. **You create a 2d array with 2 rows where each row represents
	the same values but in a different state.** Two separate arrays can be also used or one array with other array begining from offset which is the
	size of original array. But 2d array is simplest to implement and I think semantically most correct. So row index 0 in 2d array 
	represents the same sum counts as row 1, only difference is that one row is your current row which is updated, and other contains values you
	update from (the values that were updated in previous iteration cycle). *So in each iteration cycle you toggle the current and previous array,
	the one which was current and contains newest values becomes previous (one you update from) and the other one becomes current. That way there
	is no copying needed before iteration cycles and you always update just one array and the other one you update from is up-to-date.* Boolean 
	variable can store the flag for current array (value can be just 0 and 1 which corresponds to the rows of 2d array) and is the one toggled at 
	the end of each cycle.
	- It should be observed also that all the values used during updating are from previously updated array, current array might as well be empty 
	(though it's not, just containing old values).It doesn't matter because the value is overwritten. *You just avoid nulling and copying the 
	values each time. So for instance:* 
	
	*array[current][2] = array[previouslyUpdated][2] + array[previouslyUpdated][4]* 
	
	,not
	
	*array[current][2] += array[previouslyUpdated][4]*.
	
