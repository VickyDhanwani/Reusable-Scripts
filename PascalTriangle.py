"""
Given a non-negative index k where k ≤ 33, return the kth index row of the Pascal's triangle.

Note that the row index starts from 0.


Pascal triangle :

        1
      1   1
    1   2   1
  1   3   3   1
1   4   6   4   1
"""

class Solution:
    def getRow(self, rowIndex: int) -> List[int]:
        if rowIndex == 0:
            return [1]
        if rowIndex == 1:
            return [1, 1]
        if rowIndex == 2:
            return [1, 2, 1]
        t = [1, 2, 1]
        i = 2
        res = []
        while i < rowIndex:
            res = [1]
            j = len(t)
            for k in range(0, j-1):
                res.append(t[k] + t[k + 1])
            res.append(1)
            t = res
            i += 1
            
        return res
