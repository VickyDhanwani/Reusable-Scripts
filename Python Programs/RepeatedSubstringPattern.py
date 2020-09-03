"""
Given a non-empty string check if it can be constructed by taking a substring of it and appending multiple copies of the substring together. 
You may assume the given string consists of lowercase English letters only and its length will not exceed 10000.

Example 1:

Input: "abab"
Output: True
Explanation: It's the substring "ab" twice.
Example 2:

Input: "aba"
Output: False
Example 3:

Input: "abcabcabcabc"
Output: True
Explanation: It's the substring "abc" four times. (And the substring "abcabc" twice.)
"""

class Solution:
    def findIfRepeates(self, s, subs):
        times = int(len(s) / len(subs))
        if times == 1:
            return False
        newStr = subs * times
        if s == newStr:
            return True
        return False
        
    def repeatedSubstringPattern(self, s: str) -> bool:
        if len(s) == 1:
            return False
        mid = int(len(s) / 2) + 1
        while mid > 0:
            x = self.findIfRepeates(s, s[0:mid])
            if x == True:
                return True
            mid -= 1
        return False
            
