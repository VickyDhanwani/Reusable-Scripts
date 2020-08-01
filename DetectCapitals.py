#This problem shows whether captials are correctly used in a word or not.
#Returns True for words like : Mumbai, hello, IIT
#Returns False for words like : MuMbai, hEllo, iiT
class Solution:
    def detectCapitalUse(self, word: str) -> bool:
        n = len(word)
        l = []
        for i in range(0, n):
            if ord(word[i]) >= ord('A') and ord(word[i]) <= ord('Z'):
                l.append(1)
            else:
                l.append(0)
            
        if l[0] == 1:
            for i in range(2, n):
                if l[i - 1] != l[i]:
                    return False
            return True
        elif l[0] == 0:
            for i in range(1, n):
                if l[i] == 1:
                    return False
            
            return True
    
        return True
        
