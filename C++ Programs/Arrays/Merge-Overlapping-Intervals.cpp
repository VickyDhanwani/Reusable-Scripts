/*Given a set of time intervals in any order, merge all overlapping intervals into one and output the result which should have only mutually exclusive intervals. Let the intervals be represented as pairs of integers for simplicity. 
For example, let the given set of intervals be {{1,3}, {2,4}, {5,7}, {6,8}}. The intervals {1,3} and {2,4} overlap with each other, so they should be merged and become {1, 4}. Similarly, {5, 7} and {6, 8} should be merged and become {5, 8}
*/

#include<bits/stdc++.h>
using namespace std;
class node {
    public:
    int start, end;
};

int main() {
    node temp;
    vector<node> arr, output;
    int i, j, k, n, starttime, endtime;
    cin>>n;
    for(i = 0; i < n; i++) {
        cin>>k;
        cin>>j;
        temp.start = k;
        temp.end = j;
        arr.push_back(temp);
    }
    //sort the vector - Insertion Sort
    for(i = 1; i < n; i++) {
        temp = arr[i];
        j = i - 1;
        while(j >= 0 && arr[j].start > temp.start) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
    for(i = 0; i < arr.size(); i++) {
        cout<<"( "<<arr[i].start<<" , "<<arr[i].end<<" )"<<endl;
    }
    //Logic Start
    starttime = arr[0].start;
    endtime = arr[0].end;
    for(i = 1; i < n; i++) {
        if(arr[i].start <= endtime && arr[i].end <= endtime) {

        }
        else if(arr[i].start <= endtime && arr[i].end > endtime) {
            endtime = arr[i].end;
        }
        else if(arr[i].start > endtime) {
            temp.start = starttime;
            temp.end = endtime;
            output.push_back(temp);
            starttime = arr[i].start;
            endtime = arr[i].end;
        }
    }
    if(output.size() == 0 && n >= 1) {
        temp.start = starttime;
        temp.end = endtime;
        output.push_back(temp);
    }
    for(i = 0; i < output.size(); i++) {
        cout<<"( "<<output[i].start<<" , "<<output[i].end<<" )"<<endl;
    }
    return 0;
}