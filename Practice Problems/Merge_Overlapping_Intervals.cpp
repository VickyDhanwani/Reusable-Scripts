#include<bits/stdc++.h>
using namespace std;

vector<vector<int>> merge_intervals(vector<vector<int>> arr) {
    vector<vector<int>> brr;
    vector<int> interval(2);
    int i, j, n, start, end;
    n = arr.size();
    start = arr[0][0];
    end = arr[0][1];
    for(i = 1; i < n; i++) {
        if(arr[i][0] <= end) {
            end = arr[i][1];
        }
        else {
            interval[0] = start;
            interval[1] = end;
            brr.push_back(interval);
            start = arr[i][0];
            end = arr[i][1];
        }
    }
    interval[0] = start;
    interval[1] = end;
    brr.push_back(interval);
    return brr;        
}

int main() {
    vector<vector<int>> arr;
    vector<int> row(2);
    int i, start, end, n;
    cin>>n;
    for(i = 0; i < n ; i++) {
        cin>>start>>end;
        row[0] = start;
        row[1] = end;
        arr.push_back(row);
    }
    arr = merge_intervals(arr);
    for(i = 0; i < arr.size(); i++) {
        cout<<arr[i][0]<<" "<<arr[i][1]<<endl;
    }
    
    return 0;
}