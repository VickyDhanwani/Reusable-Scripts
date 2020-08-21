"""
A sentence S is given, composed of words separated by spaces. Each word consists of lowercase and uppercase letters only.

We would like to convert the sentence to "Goat Latin" (a made-up language similar to Pig Latin.)

The rules of Goat Latin are as follows:

If a word begins with a vowel (a, e, i, o, or u), append "ma" to the end of the word.
For example, the word 'apple' becomes 'applema'.
 
If a word begins with a consonant (i.e. not a vowel), remove the first letter and append it to the end, then add "ma".
For example, the word "goat" becomes "oatgma".
 
Add one letter 'a' to the end of each word per its word index in the sentence, starting with 1.
For example, the first word gets "a" added to the end, the second word gets "aa" added to the end and so on.
Return the final sentence representing the conversion from S to Goat Latin. 

"""

class Solution:
    
    def reverseString(S: str) -> str:
        return S[::-1]
    
    def toGoatLatin(self, S: str) -> str:
        consonantString = "a"
        vowels = ["a", "e", "i", "o", "u", "A", "E", "I", "O", "U" ]
        words = S.split(" ")
        Sentence = ""
        i = 0
        for x in words:
            
            if x[0] in vowels:
                Sentence += x + "ma" + consonantString
            else:
                Sentence += x[1:] + x[0] + "ma" + consonantString
            
            if i != len(words) - 1:
                Sentence += " "
            consonantString += "a"
            i += 1
        return Sentence
