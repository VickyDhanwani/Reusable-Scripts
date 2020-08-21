# Given an integer (signed 32 bits), write a function to check whether it is a power of 4.

class Solution:
    def isPowerOfFour(self, num: int) -> bool:
        t = 0
        prod = num
        while num > 1:
            num =int( num / 4 )
            t += 1
        
        if 4 ** t == prod:
            return True
        return False
