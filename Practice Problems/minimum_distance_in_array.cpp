#include<bits/stdc++.h>
using namespace std;


int  minimum_difference(vector<int>nums){
    sort(nums.begin(), nums.end());
    int diff = nums[1] - nums[0];
    for(int i = 2; i < nums.size(); i++) {
        if(diff > (nums[i] - nums[i - 1])) {
            diff = nums[i] - nums[i - 1];
        }
    }
    return diff;
}

int main() {
    vector<int> arr;
    int n, i, elem;
    cin>>n;
    for(i = 0; i < n; i++) {
        cin>>elem;
        arr.push_back(elem);
    }
    cout<<minimum_difference(arr);
    return 0;
}