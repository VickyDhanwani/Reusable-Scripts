"""Given a string, determine if it is a palindrome, considering only alphanumeric characters and ignoring cases.

Note: For the purpose of this problem, we define empty string as valid palindrome."""

class Solution:
    def isPalindrome(self, s: str) -> bool:
        st = ""
        for x in s:
            if ord(x) >= ord('a') and ord(x) <= ord('z'):
                st += x
            elif ord(x) >= ord('A') and ord(x) <= ord('Z'):
                st += x.lower()
            elif ord(x) >= ord('0') and ord(x) <= ord('9'):
                st += x
            else:
                pass
        
        t = st[::-1]
        if t == st:
            return True
        return False
