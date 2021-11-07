#include<iostream>
using namespace std;

int main() {
    int t, x, y;
    cin>>t;
    while(t) {
       cin>>x>>y;
        if((x % 2 == 0) && (y % 2 == 0)) {
            cout<<"YES"<<endl;
        }
        else if((x % 2 == 0) && (y % 2 != 0) && (x > 0)) {
            cout<<"YES"<<endl;
        }
        else {
            cout<<"NO"<<endl;
        }
        t--;
    }
    return 0;
}
