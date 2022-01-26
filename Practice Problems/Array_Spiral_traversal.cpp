#include<bits/stdc++.h>
using namespace std;

int main() {
    int rows, x, i, j;
    cin>>rows;
    int arr[rows][rows];
    for(i = 0; i < rows; i++) {
        for(j = 0; j < rows; j++) {
            cin>>arr[i][j];
        }
    }
    x = 0;
    while(x <= (rows / 2)) {
        i = x;
        j = x;
        while(j < (rows - x)) {
            //cout<<"Printing "<<i<<","<<j<<endl;
            cout<<arr[i][j]<<" ";
            j++;
        }
        j = rows - x - 1;
        i = x + 1;
        while(i < (rows - x)) {
            //cout<<"Printing "<<i<<","<<j<<endl;
            cout<<arr[i][j]<<" ";
            i++;
        }
        i = rows - x - 1;
        j = rows - x - 2;
        while(j >= x) {
            //cout<<"Printing "<<i<<","<<j<<endl;
            cout<<arr[i][j]<<" ";
            j--;
        }
        i = rows - x - 2;
        j = x;
        while(i >= (x + 1)) {
            //cout<<"Printing "<<i<<","<<j<<endl;
            cout<<arr[i][j]<<" ";
            i--;
        }
        x++;
    }
    return 0;
}