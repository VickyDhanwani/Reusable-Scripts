#include<bits/stdc++.h>
using namespace std;

int LongestPalindromicSubstring(string str) {
    int size = str.size();
    int DP[size][size];
    int i, j;
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            if(i == j) {
                DP[i][j] = 1;
            }
            else if(i > j) {
                DP[i][j] = -1;
            }
            else if(j == i + 1) {
                if(str[j] == str[j - 1]) {
                    DP[i][j] = 1;
                }
                else {
                    DP[i][j] = 0;
                }
            }
            else {
                DP[i][j] = 0;
            }
        }
    }
    for(j = 2; j < size; j++) {
        for(i = 0; i < size - 2; i++) {
            if(str[i] == str[i + j]) {
                DP[i][i + j] = DP[i + 1][j - 1];
            }
            else {
                DP[i][i + j] = 0;
            }
        }
    }
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            cout<<DP[i][j]<<" ";
        }
        cout<<endl;
    }

    return 0;
    
}

int main() {
    string str;
    cin>>str;
    //cout<<str<<"--"<<str.size()<<"--"<<str[4]<<int(str[0] > str[9]);
    cout<<LongestPalindromicSubstring(str);
    return 0;
}