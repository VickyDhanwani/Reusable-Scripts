#include<bits/stdc++.h>
using namespace std;

void merge(int *arr, int left, int mid, int right) {
    int left_size = mid - left + 1;
    int right_size = right - mid;
    int *brr, *crr;
    brr = new int[left_size];
    crr = new int[right_size];
    int i, j, k;
    for(i = 0; i <= mid; i++) {
        brr[i] = arr[i + left];
    }
    for(i = 0; i < (right - mid); i++) {
        crr[i] = arr[i + mid + 1];
    }
    k = left;
    i = 0;
    j = 0;
    while(i < left_size && j < right_size) {
        if(brr[i] > crr[j]) {
            arr[k] = crr[j];
            k++;
            j++;
        }
        else if(brr[i] < crr[j]) {
            arr[k] = brr[i];
            i++;
            k++;
        }
        else {
            arr[k] = brr[i];
            k++;
            arr[k] = brr[i];
            k++;
            i++;
            j++;
        }
    }
    while(i < left_size) {
        arr[k] = brr[i];
        i++;
        k++;
    }
    while(j < right_size) {
        arr[k] = crr[j];
        k++;
        j++;
    }
}

void mergesort(int *arr, int left, int right) {
    if(left >= right) {
        return;
    }
    int mid = (left + right) / 2;
    mergesort(arr, left, mid);
    mergesort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int main() {
    int *arr, n, i, j;
    cin>>n;
    arr = new int[n];
    for(i = 0; i < n; i++) {
        cin>>arr[i];
    }
    mergesort(arr, 0, n - 1);
    for(i = 0; i < n; i++) {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    return 0;
}
