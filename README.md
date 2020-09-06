# Project-Euler
## Project Euler #250

This is my solution to a Project Euler #250 problem  posted on HackerRank.
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
	get the modulo you have to calculate the number first. There is an optimized algorithm for calculating power using memoization,
	which do not require exponent amount of multiplications, but require log<sub>2</sub>(exponent) + 1 number of multiplications. It's called "**exponentiation by squaring**" algorithm. So to calculate 5<sup>32</sup> with it we do not need 32 but 6 multiplications. This algorithm is already included in language compiler by default for
	pow function, but not its combination with modulus operation in each step, which increases number of mathematical operations, 
	but reduces complexity of multiplication and thus increases speed for larger numbers. So called "**modular exponentiation**".
  - More importantly, as we transform each new added number into a modulus of K, if we keep track of those values (a<sup>a</sup> mods), **repeating** 
	**pattern can be noticed**, for some K values larger, for some small. For instance, pattern for K = 21 is size 43, for K = 47 pattern is size 2163. **So once we locate the pattern, calculating powers and mods for new numbers is unnecessary, we can just repeatedly cycle numbers in a pattern.** 
    And considering that max N could be huge (10<sup>400</sup>) that's a good optimization for better results.
- All that considering, number of iterations can still be huge and have to be counted somehow so a BigInteger implementation should be used in some places for larger tests to end up correct. I haven't done this yet as I still haven't reached the required timing performance for some smaller tests that don't require BigIntegers.
	 
Some comments regarding code:

- **I also included shorter version without pattern-searching because it is more concise and easier to understand though less performant.**

- **Why 2d array for sum counts?** As values in array of sum counts (sumsCounter) have to be updated from that same array but different index 
in each iteration, if only one array would be used that would invalidate the array and result in incorrect values. 
	- So this is one of the ways of resolving this problem in a relatively optimal way. You create a 2d array with 2 rows where each row represents
	the same values but in a different state. Instead of this two separate arrays might also be used with pointers changing between one and the other array.
	But I think 2d array is simpler and more concise to implement. So row index 0 in 2d array 
	represents the same sum counts as row 1, only difference is that one row is your current row which is updated, and other contains values you
	update from (the values that were updated in previous iteration cycle). *So in each iteration cycle you toggle the current and previous array,
	the one which was current and contains newest values becomes previous (one you update from) and the other one becomes current. That way there
	is no copying or initialization needed before each iteration cycle and you always update just one array and the other one you update from is up-to-date.* **Boolean 
	variable stores the flag for current array** (value can be just 0 and 1 which corresponds to the rows of 2d array) and is the one toggled at 
	the end of each cycle.
	- All the values used during updating are from previously updated array, current array might as well be empty 
	(though it's not, just containing old values).It doesn't matter because the value is overwritten. So:
	
	*array[current][2] = array[previouslyUpdated][2] + array[previouslyUpdated][4]* 
	
	,not
	
	*array[current][2] += array[previouslyUpdated][4]*.
	
