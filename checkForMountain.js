/*
Given an array A of integers, return true if and only if it is a valid mountain array.

Recall that A is a mountain array if and only if:

A.length >= 3
There exists some i with 0 < i < A.length - 1 such that:
A[0] < A[1] < ... A[i-1] < A[i]
A[i] > A[i+1] > ... > A[A.length - 1]

Mountains :
[0,3,2,1]
[0, 1, 3, 6, 7, 8, 7, 3, 2, 1]

Non Mountains :

[0, 3, 3, 1]
[0, 3, 5, 7, 8, 9, 8, 10]


*/



/**
 * @param {number[]} A
 * @return {boolean}
 */
var validMountainArray = function(A) {
    let n = A.length;
    let i = 0, maxim = 0;
    if(n < 3) {
        return false;
    }
    
    for(i = 1; i < n; i++) {
        if(A[i] > A[maxim]) {
            maxim = i;
        } 
    }
    if(maxim == 0 || maxim == n - 1) {
        return false;
    }
    //console.log(A[maxim]);
    for(i = maxim - 1; i >= 0; i--) {
        if(A[i] >= A[i + 1]) {
            return false;
        }
    }
    for(i = maxim + 1; i < n; i++) {
        if(A[i] >= A[i - 1]) {
            return false;
        }
    }
    return true;
    
};
