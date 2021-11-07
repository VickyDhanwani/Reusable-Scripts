#include<bits/stdc++.h>
using namespace std;

int main() {
    int t, n, i, j, tag, hilltop;
    vector<int> arr;
    cin>>t;
    while(t) {
        cin>>n;
        tag = 1;
        hilltop = 0;
        for(i = 0; i < n; i++) {
            cin>>j;
            arr.push_back(j);
            if(i > 0 && tag != 4) {
                if(tag == 1) {
                    if(arr[i] > arr[i - 1]) {
                        tag = 2;
                    }
                    else if(arr[i] < arr[i - 1]) {
                        tag = 3;
                    }
                    else {
                        tag = 4;
                    }
                }
                else if(tag == 2) {
                    if(arr[i] < arr[i - 1]) {
                        tag = 3;
                        hilltop = 1;
                    }
                    else if(arr[i] == arr[i - 1]) {
                        tag = 4;
                    }
                }
                else if(tag == 3) {
                    if(arr[i] >= arr[i - 1]) {
                        tag = 4;
                    }
                }
            }

        }
        cout<<tag<<endl;
        arr.clear();
        t--;
    }
    return 0;
}