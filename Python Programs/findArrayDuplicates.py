"""
Given an array of integers, 1 ≤ a[i] ≤ n (n = size of array), some elements appear twice and others appear once.

Find all the elements that appear twice in this array.
"""

class Solution:
    def findDuplicates(self, nums: List[int]) -> List[int]:
        d = {}
        r = []
        for x in nums:
            if d.get(x):
                d[x] += 1
            else :
                d[x] = 1
            
        for x in d.keys():
            if d[x] == 2:
                r.append(x)
            
        return r
