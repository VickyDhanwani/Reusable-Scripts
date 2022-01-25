#include<bits/stdc++.h>
using namespace std;

int Find_Minimum_Platforms(vector<int> arrivals, vector<int> departures, int trains) {
    sort(arrivals, arrivals + trains);
    sort(departures, departures + trains);
    int min_platforms = 0, i = 0, curr_platforms = 0, j = 0;
    if(arrivals.size() == 1) {
        return 1;
    }
    while(i < arrivals.size()) {
        if(arrivals[i] <= departures[j]) {
            i++;
            curr_platforms++;
        }
        else if(arrivals[i] > departures[j]) {
            j++;
            curr_platforms--;
        }
        if(curr_platforms > min_platforms) {
            min_platforms = curr_platforms;
        }
    }
    return min_platforms;
}

int main() {
    vector<int> arrivals;
    vector<int> departures;
    int trains, i, arr, dep;
    cin>>trains;
    for(i = 0; i < trains; i++) {
        cin>>arr>>dep;
        arrivals.push_back(arr);
        departures.push_back(dep);
    }
    cout<<Find_Minimum_Platforms(arrivals, departures, trains);
    return 0;
}