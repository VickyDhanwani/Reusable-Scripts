#include<bits/stdc++.h>
using namespace std;

class Heap {
    private:
        int *arr;
        int len, top;
    public: 
        Heap(int len) {
            this->arr = new int[len];
            this->len = len;
            this->top = 0;
        }

        void HeapInsert(int elem) {
            this->arr[this->top] = elem;
            top++;
            printHeap();
        }

        void printHeap() {
            int i;
            for(i = 0; i < top; i++) {
                cout<<arr[i]<<" ";
            }  
            cout<<endl;
        }
};

int main() {
    int n, k;
    Heap hp(5);
    hp.HeapInsert(4);
    hp.HeapInsert(9);
    hp.HeapInsert(8);
    hp.HeapInsert(7);
    hp.HeapInsert(6);
    return 0;
}