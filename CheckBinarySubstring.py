"""
Given a binary string S (a string consisting only of '0' and '1's) and a positive integer N, return true 
if and only if for every integer X from 1 to N, the binary representation of X is a substring of S.
"""



class Solution:
   
    def queryString(self, S: str, N: int) -> bool:
        for x in range(1, N + 1):
            s = ""
            n = x
            while n > 0:
                t = n % 2
                s = s + str(t)
                n = int(n / 2)
            r = s[::-1]
            if S.find(r) == -1:
                return False
            
        return True
