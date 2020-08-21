/* Given a column title as appear in an Excel sheet, return its corresponding column number. */
/*
 A -> 1
    B -> 2
    C -> 3
    ...
    Z -> 26
    AA -> 27
    AB -> 28 
    ...
*/
class Solution {
public:
    int titleToNumber(string s) {
        int result = 0;
        int l = s.length();
        int i = 0;
        for(i = 0; i < l; i++) {
            result *= 26;
            result += s[i] - 'A' + 1; 
        }
        return result;
    }
};
