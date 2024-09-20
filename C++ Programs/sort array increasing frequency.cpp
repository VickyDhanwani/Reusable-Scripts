class Node {
    public:
    int val, freq;
    Node * next;
    Node(int val, int freq) {
        this->val = val;
        this->freq = freq;
        this->next = NULL;
    }
};

class Solution {
public:
    vector<int> frequencySort(vector<int>& nums) {
        map<int, int> m;
        for(int i = 0; i < nums.size(); i++) {
            m[nums[i]]++;
        }
        vector<Node *> n(101, NULL);
        for(auto x : m) {
            
            Node *temp = new Node(x.first, x.second);
            if(!n[x.second]) {n[x.second] = temp;
            }
            else if(n[x.second]) {
                Node *t = n[x.second];
                if(temp->val > t->val) {
                    n[x.second] = temp;
                    temp->next = t;
                }
                else {
                    Node *p = t;
                    while(t && t->val < temp->val) {
                        p = t;
                        t = t->next;
                    }
                    if(!t) {
                        p->next = temp;
                    }
                    else {
                        p->next = temp;
                        temp->next = t;
                    }
                }
                
            }
        } 
        vector<int> res;
        for(int i = 1; i < 101; i++) {
            Node *t = n[i];
            
               // cout<<i<<" -> ";
            
            while(t) {
                //cout<<t->val<<" ";
                
                for(int j = 0; j < i; j++) {
                    res.push_back(t->val);
                }
                
                t = t->next;
            }
            cout<<endl;
        }     
        return res;
    }
};
