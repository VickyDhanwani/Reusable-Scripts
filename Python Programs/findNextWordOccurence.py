"""
Given words first and second, consider occurrences in some text of the form "first second third", where second comes immediately after first, and third comes immediately after second.

For each such occurrence, add "third" to the answer, and return the answer.
"""

class Solution:
    def findOcurrences(self, text: str, first: str, second: str) -> List[str]:
        textArr = text.split(" ")
        if len(textArr) == 0:
            return []
        if len(textArr) == 1 or len(textArr) == 2:
            return []
        i = 0
        j = i + 1
        k = j + 1
        p = []
        while k < len(textArr):
            if textArr[i] == first and textArr[j] == second:
                p.append(textArr[k])
            i += 1
            j += 1
            k += 1
        
        return p
