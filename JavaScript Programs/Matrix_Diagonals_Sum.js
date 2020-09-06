/*

Input: mat = [[1,2,3],
              [4,5,6],
              [7,8,9]]
Output: 25
Explanation: Diagonals sum: 1 + 5 + 9 + 3 + 7 = 25
Notice that element mat[1][1] = 5 is counted only once.
Example 2:

Input: mat = [[1,1,1,1],
              [1,1,1,1],
              [1,1,1,1],
              [1,1,1,1]]
Output: 8
Example 3:

Input: mat = [[5]]
Output: 5

*/

/**
 * @param {number[][]} mat
 * @return {number}
 */
var diagonalSum = function(mat) {
    var i = 0;
    var  n = mat.length;
    var j = 0;
    var leftDiagonalSum = 0;
    var rightDiagonalSum = 0;
    for(i = 0; i < n; i++) {
        leftDiagonalSum += mat[i][j];
        
        j++;
    }
    j = n - 1;
    for(i = 0; i < n; i++) {
        rightDiagonalSum += mat[i][j];
        j--;
    }
    leftDiagonalSum += rightDiagonalSum;
    if(n % 2 == 1) {
        i = parseInt(n/2);
        leftDiagonalSum -= mat[i][i];
    }
    return leftDiagonalSum;
};
