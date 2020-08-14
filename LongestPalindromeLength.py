"""

Given a string which consists of lowercase or uppercase letters, 
find the length of the longest palindromes that can be built with those letters.

This is case sensitive, for example "Aa" is not considered a palindrome here.

Note:
Assume the length of given string will not exceed 1,010.

"""

class Solution:
    def longestPalindrome(self, s: str) -> int:
        d = {}
        res = 0
        odd = 0
        for x in s:
            if d.get(x):
                d[x] += 1
            else:
                d[x] = 1
        for x in d.keys():
            if d[x] % 2 == 0:
                res += d[x]
            else:
                odd = 1
                res += (d[x] - 1)
        return res + odd
