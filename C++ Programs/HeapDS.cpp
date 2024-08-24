#include <bits/stdc++.h>
using namespace std;

// heap 
// 1. Complete Binary Tree
// 2. Min Heap - Value if Children is greater than its parent
// Array Method
// Assume Array is a Tree
// Root = arr[0]
// L & R Child of root = arr[1] && arr[2]
// L & R child of node n = arr[2n + 1] & arr[ 2n + 2 ]
// Parent of any Node = n - 1 / 2;
// L & R of node 1 = arr[3] & arr[4]
// Arr = []
// Insert 2 => [2]
// Insert 4 => [2, 4]
//Insert 6 => [2, 4, 6]

//Insert 1 => [2 , 4, 6 , 1]
//Evalute => 1 < 4 => swap 1 & 4 => [2 , 1, 6, 4]
//Evalute => 1 < 2 => swap 1 & 2 => [1, 2, 6, 4]

// GetTop => send root => 1
// Replace root with last
// New Root = min(left, right) => [4, 2 , 6]
//Check if heap condition satisfied from root to child => [2, 4, 6]

void PrintHeap(vector<int> &hp) {
    for(int i = 0; i < hp.size(); i++) {
        cout<<hp[i]<<" ";
    }
    cout<<endl;
}

void HeapInsert(vector<int> &hp, int val) {
    hp.push_back(val);
    int child = hp.size() - 1;
    int parent = (child - 1) / 2;
    int x;
    PrintHeap(hp);
    while(child > 0) {
        if(hp[child] < hp[parent]) {
          x = hp[child];
          hp[child] = hp[parent];
          hp[parent] = x;
          PrintHeap(hp);
        }
        child = parent;
        parent = (child - 1) / 2;
        
    }
    
    cout<<"Inserted "<<val<<endl;
    
}

int removeMinimum(vector<int> &hp) {
    if(hp.size() <= 0) return -1;
    int min_val = hp[0];
    int x, n, p, lc, rc;
    n = hp.size() - 1;
    hp[0] = hp[n];
    hp.pop_back();
    p = 0;
    
        lc = (2*p) + 1;
    
    
        rc = (2 * p) + 2;
    
    //PrintHeap(hp);
    // Initiak = p = 0 , lc = 1, rc = 2
    while(lc < n || rc < n) {
        if(lc < n && hp[lc] <= hp[p]) {
            x = hp[p];
            hp[p] = hp[lc];
            hp[lc] = x;
            p = lc;
            //PrintHeap(hp);
        }
        else if(rc < n && hp[rc] < hp[p]){
            x = hp[p];
            hp[p] = hp[rc];
            hp[rc] = x;
            p = rc;
            //PrintHeap(hp);
        }
        else {
            break;
        }
        lc = (2 * p) + 1;
        rc = (2 * p) + 2;  
        //cout<<p<<" "<<lc<<" "<<rc<<endl;
    }
    return min_val;
}

int main() {
    vector<int> hp;
    int p, lc, rc;
    HeapInsert(hp, 2);
    HeapInsert(hp, 4);
    HeapInsert(hp, 6);
    HeapInsert(hp, 1);
    HeapInsert(hp, 3);
    cout<<removeMinimum(hp)<<endl;
    cout<<removeMinimum(hp)<<endl;
    cout<<removeMinimum(hp)<<endl;
    cout<<removeMinimum(hp)<<endl;
    cout<<removeMinimum(hp)<<endl;
    cout<<removeMinimum(hp)<<endl;
    cout<<endl;
	// your code goes here
    return 0;
}
