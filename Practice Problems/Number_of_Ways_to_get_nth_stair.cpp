#include<bits/stdc++.h>
using namespace std;

int count_ways(int n){
    if(n == 0) {
        return 0;
    }
    if(n == 1) {
        return 1;
    }
    if(n == 2) {
        return 2;
    }
    return count_ways(n - 1) + count_ways(n - 2);

}

int main() {
    int n;
    cin>>n;
    cout<<count_ways(n);
    return 0;
}