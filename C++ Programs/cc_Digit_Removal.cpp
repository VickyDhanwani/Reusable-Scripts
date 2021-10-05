#include<iostream>
using namespace std;

int main() {
    int t;
    int n, c, d, r, i, max_index, count, j;
    int arr[10], brr[10];
    cin>>t;
    while(t) {
        cin>>n;
        cin>>d;
        for(i = 0; i < 10; i++) {
            arr[i] = 0;
            brr[i] = 0;
        }
        c = n;
        j = 9;
        count = 0;
        max_index = -1;
        while(c) {
            r = c % 10;
            arr[j] = r;
            if(r == d) {
                max_index = j;
            }
            j--;
            count++;
            c = c / 10;
        }
        if(max_index == -1) {
            cout<<"0"<<endl;
        }
        else if(n == 0 && d == 0) {
            cout<<"1"<<endl;
        }
        else if(n == 0 && d != 0) {
            cout<<"0"<<endl; 
        }
        else
        {
            if (d == 9)
            {
                brr[max_index] = 0;
                brr[max_index - 1] = arr[max_index - 1] + 1;
                for(j = max_index - 2; j >= 0; j--) {
                    brr[j] = arr[j];
                }
                for(j = max_index + 1; j < 10; j++) {
                    brr[j] = 0;
                }
                
            }
            else if(d == 0) {
                brr[max_index] = 1;
                for(j = max_index + 1; j < 10; j++) {
                    brr[j] = 1;
                }
                for(j = max_index - 1; j >= 0; j--) {
                    brr[j] = arr[j];
                }
            }
            else
            {
                brr[max_index] = arr[max_index] + 1;
                for(j = max_index - 1; j >= 0; j--) {
                    brr[j] = arr[j];
                }
                for(j = max_index + 1; j < 10; j++) {
                    brr[j] = 0;
                }
            }
            //cout<<max_index<<endl;
            r = 0;
            for(j = 0; j < 10; j++) {
                r = (r * 10) + brr[j];
            }
            cout<<(r - n)<<endl;
        }
        t--;
    }
    return 0;
}