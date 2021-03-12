"""
Given a pattern and a string str, find if str follows the same pattern.

Here follow means a full match, such that there is a bijection between a letter in pattern and a non-empty word in str.

Example 1:

Input: pattern = "abba", str = "dog cat cat dog"
Output: true
Example 2:

Input:pattern = "abba", str = "dog cat cat fish"
Output: false
Example 3:

Input: pattern = "aaaa", str = "dog cat cat dog"
Output: false
Example 4:

Input: pattern = "abba", str = "dog dog dog dog"
Output: false
Notes:
You may assume pattern contains only lowercase letters, and str contains lowercase letters that may be separated by a single space.
"""

class Solution:
    def wordPattern(self, pattern: str, str: str) -> bool:
        words = str.split(" ")
        if len(words) != len(pattern) :
            return False
        d = {}
        n = len(words)
        for i in range(0, n):
            if d.get(pattern[i]):
               
                if d[pattern[i]] != words[i]:
                    return False
            else:
                if words[i] in d.values():
                    return False
                d[pattern[i]] = words[i]
            
        return True