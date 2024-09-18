class Solution {
public:
    int romanToInt(string s) {
        int val = 0;
        for(int i = 0; i < s.size() ; i++) {
            if(s[i] == 'I') {
                if(i != s.size() - 1 && s[i + 1] == 'V') {
                    i++;
                    val = val + 4;
                }
                else if(i != s.size() - 1 && s[i + 1] == 'X') {
                    i++;
                    val = val + 9;
                }
                else {
                    val = val + 1;
                }
            }
            else if(s[i] == 'X') {
                if(i != s.size() && s[i + 1] == 'L') {
                    i++;
                    val = val + 40;
                }
                else if(i != s.size() && s[i + 1] == 'C') {
                    i++;
                    val = val + 90;
                }
                else {
                    val = val + 10;
                }
            }
            else if(s[i] == 'C') {
                if(i != s.size() && s[i + 1] == 'D') {
                    i++;
                    val = val + 400;
                }
                else if(i != s.size() && s[i + 1] == 'M') {
                    i++;
                    val = val + 900;
                }
                else {
                    val = val + 100;
                }
            }
            else if(s[i] == 'V') {
                val = val + 5;
            }
            else if(s[i] == 'L') {
                val = val + 50;
            }
            else if(s[i] == 'D') {
                val = val + 500;
            }
            else if(s[i] == 'M') {
                val = val + 1000;
            }
            
        }
        return val;
    }
};
