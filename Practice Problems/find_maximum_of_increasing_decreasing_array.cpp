#include<bits/stdc++.h>
using namespace std;

/*
Used Binary Search Algorithm + Recursion
*/

int FindMax(vector<int> arr, int left, int right) {
    if(left == right) {
        return arr[left];
    }
    if(right == left + 1 && arr[right] > arr[left]) {
        return arr[right];
    }
    if(right == left + 1 && arr[left] > arr[right]) {
        return arr[left];
    }
    int mid = (left + right) / 2;
    if(arr[mid] > arr[mid + 1] && arr[mid] > arr[mid - 1]) {
        return arr[mid];
    }
    if(arr[mid] > arr[mid + 1] && arr[mid] < arr[mid - 1]) {
        return FindMax(arr, mid, right);
    }
    if(arr[mid] < arr[mid + 1] && arr[mid] > arr[mid - 1]) {
        return FindMax(arr, left, mid);
    }
}

int main() {
    vector<int> arr;
    int i, n, elem;
    cin>>n;
    for(i = 0; i < n; i++) {
        cin>>elem;
        arr.push_back(elem);
    }
    cout<<FindMax(arr, 0, arr.size() - 1);
    return 0;
}