"""
You have an array of logs.  Each log is a space delimited string of words.

For each log, the first word in each log is an alphanumeric identifier.  Then, either:

Each word after the identifier will consist only of lowercase letters, or;
Each word after the identifier will consist only of digits.
We will call these two varieties of logs letter-logs and digit-logs.  It is guaranteed that each log has at least one word after its identifier.

Reorder the logs so that all of the letter-logs come before any digit-log.  The letter-logs are ordered lexicographically ignoring identifier, with the identifier used in case of ties.  The digit-logs should be put in their original order.

Return the final order of the logs.
"""

class Solution:
    def reorderLogFiles(self, logs: List[str]) -> List[str]:
        dig = []
        let = {}
        for x in logs:
            t = x.find(" ")
            key = x[0:t+1]
            value = x[t+1:]
            if ord(x[t+1]) >= ord('0') and ord(x[t+1]) <= ord('9'):
                dig.append(x)
                
            else:
                let[key] = value
        res = []
        let2 = {}
        for x in sorted(let.keys()):
            let2[x] = let[x]
        
        let = sorted(let2.items(), key = lambda x : x[1])
        for x in let:
            t = x[0] + x[1]
            res.append(t)
        
        
        for x in dig:
            
            res.append(x)
            
           
        return res
