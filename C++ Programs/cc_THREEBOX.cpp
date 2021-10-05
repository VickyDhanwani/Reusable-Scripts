#include<iostream>
using namespace std;

int main() {
    int t, a, b, c, d, count;
    cin>>t;
    while(t) {
        cin>>a>>b>>c>>d;
        count = 0;
        if((a + b + c) <= d) {
            count = 1;
        }
        else if(a == d && b == d && c == d) {
            count = 3;
        }
        else {
            if((a + b) <= d || (b + c) <= d || (a + c) <= d) {
                count = 2;
            }
            else {
                count = 3;
            }
        }
        cout<<count<<endl;
        t--;
    } 
    return 0;
}