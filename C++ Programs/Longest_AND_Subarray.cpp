#include<iostream>
using namespace std;

int main() {
    int t, n, i, max_count, mmax_cnt;
    cin>>t;
    while(t) {
        mmax_cnt = -1;
        cin>>n;
        max_count = 1;
        if(n == 1) {
            cout<<"1"<<endl;
            t--;
            continue;
        }
        for(i = 1; i < n; i++) {
            if(i & (i + 1) > 0) {
                max_count++;
                if(max_count > mmax_cnt) {
                    mmax_cnt = max_count;
                }
            }
            else {
                max_count = 0;
                
            }
        }
        cout<<mmax_cnt<<endl;
        t--;
    }

    return 0;
}