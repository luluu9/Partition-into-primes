# Partition-into-primes
A partition of n is a representation of this number as a sum of positive integers: n=n1+n2+...+nr such that n1≥n2≥...≥nr. The numbers n1,n2,...,nr are called parts.

The partition 19=5+3+3+2+2+2+2 can be written as 53224, while 19=5+2+2+2+2+2+2+2 can be written as 527.
Hence the upper indices are 124 and 107, thus 5+3+3+2+2+2+2 is larger than 5+2+2+2+2+2+2+2, 
since 124 is larger than 107.


## Problem

Starting from the smallest partition, write all partitions of the integer n, which parts are prime numbers and the largest part is equal to k. n can be stored in byte.

## Input

In the first line: a number of the rest lines
Then, separated by spaces: n and k

## Output

Partitions separated by +. If there is no partition, then write nothing.

## Example

### Input 
```
3
12 5
5 2
27 17
```

### Output
```
5+3+2+2
5+5+2
17+2+2+2+2+2
17+3+3+2+2
17+5+3+2
17+5+5
17+7+3
```
