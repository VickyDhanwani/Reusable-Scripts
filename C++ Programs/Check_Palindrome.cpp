/*
Given a string, determine if it is a palindrome, considering only alphanumeric characters and ignoring cases.

Note: For the purpose of this problem, we define empty string as valid palindrome.
*/

class Solution {
public:
    bool checkPalindrome(vector<char> newStr) {
        int n = newStr.size();
        int i = 0, j = n - 1;
        while(j >= i) {
            if(newStr[i] != newStr[j]) {
                return false;
            }
            i++;
            j--;
        }
        return true;
    }
    
    bool isPalindrome(string s) {
        vector<char> newStr;
        int n = s.size();
        int i = 0;
        for(i = 0; i < n; i++) {
            if(s[i] >= 'a' && s[i] <= 'z') {
                newStr.push_back(s[i]);
            }
            else if(s[i] >= 'A' && s[i] <= 'Z') {
                newStr.push_back(s[i] - 'A' + 'a');
            }
            else if(s[i] >= '0' && s[i] <= '9'){
                newStr.push_back(s[i]);
            }
            
        }
        return checkPalindrome(newStr);
    }
};
