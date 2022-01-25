/*
Given a string 's'. The task is to find the smallest window length that contains all the characters of the given string at least one time.
For eg. A = aabcbcdbca, then the result would be 4 as of the smallest window will be dbca.
*/
 
#include<bits/stdc++.h>
using namespace std;

int main() {
    string str;
    int len, i, j, k, count = 0, temp_count = 0, non_distinct = 0;
    int arr[26] = {0};
    cin>>str;
    len = str.size();
    for(i = 0; i < len; i++) {
        arr[str[i] - 'a']++;
    }
    for(i = 0; i < 26; i++) {
        if(arr[i] > 0) {
            count++;
        }
    }
    for(i = 0; i <= (len - count); i++) {
        for(k = 0; k < 26; k++) {
            arr[k] = 0;
        }
        for(k = i; k < (i + count); k++) {
            arr[str[k] - 'a']++;
        }
        non_distinct = 0;
        for(k = 0; k < 26; k++) {
            if(arr[k] > 1) {
                non_distinct = 1;
                break;
            }
        }
        if(non_distinct == 0) {
            for(k = i; k < (i + count); k++) {
                cout<<str[k];
            }
            cout<<endl;
        }
    }
    return 0;
}
