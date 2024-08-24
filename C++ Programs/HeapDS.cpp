class Solution {
public:

    void swap( int  *a , int *b) {
        int x;
        x = *a;
        *a = *b;
        *b = x;
    }

    void HeapInsert(vector<int> &hp, int val) {
        hp.push_back(val);
        int child = hp.size() - 1;

        int parent = (child - 1) / 2;
        
        while(child >= 0) {
            if(hp[child] > hp[parent]) {
                swap(&hp[child], &hp[parent]);
            }
            else {
                break;
            }
            child = parent;
            parent = (parent - 1) / 2;
        }
    }
    int getMax(vector<int> &hp) {
        if(hp.size() <= 0) return -1;
        int max_val = hp[0];
        hp[0] = hp[hp.size() - 1];
        hp.pop_back();
        int new_size = hp.size();
        int parent, left_child, right_child;
        parent = 0;
        left_child = (2 * parent) + 1;
        right_child = (2 * parent) + 2;
        while(left_child < new_size || right_child < new_size) {
            if(left_child < new_size && right_child < new_size && hp[left_child] > hp[parent] && hp[right_child] > hp[parent]) {
                if(hp[left_child] > hp[right_child]) {
                    swap(&hp[left_child], &hp[parent]);
                    parent = left_child;
                }
                else {
                    swap(&hp[right_child], &hp[parent]);
                    parent = right_child;
                }
            }
            else if(left_child < new_size && hp[left_child] > hp[parent] ) {
                swap(&hp[left_child], &hp[parent]);
                parent = left_child;
            }   
            else if(right_child < new_size && hp[right_child] > hp[parent]) {
                swap(&hp[right_child], &hp[parent]);
                parent = right_child;
            }
            else {
                break;
            }
            left_child = (2 * parent) + 1;
            right_child = (2 * parent) + 2;
        }
        return max_val;
    }
    vector<string> findRelativeRanks(vector<int>& score) {
        vector<int> hp;
        for(int i = 0; i < score.size(); i++) {
            HeapInsert(hp, score[i]);

        }

        vector<string> result;
        map<int, string> mp;
        int heap_size = hp.size();
        int i = 0;
        while(i < heap_size) {
            int max_val = getMax(hp);
            //cout<<max_val<<" "<<to_string(i + 1)<<endl;
            if(i == 0) {
                mp[max_val] = "Gold Medal";
            }
            else if(i == 1) {
                mp[max_val] = "Silver Medal";
            }
            else if(i == 2) {
                mp[max_val] = "Bronze Medal";
            }
            else {
                mp[max_val] = to_string((i +  1));
                
            }
            i++;
        }
        for(int i = 0; i < score.size(); i++) {
            result.push_back(mp[score[i]]);
        }
        
        
        return result;
        //
        /*
        For first heap remove, insert gold in reult  
        For second heap remove insert silver
        For third heap remove insert bronze
        For fourth heap remove insert the same number
        */
    }
};
