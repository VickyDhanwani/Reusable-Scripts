class Node {
    public:
        string val;
        Node *next;
        Node(string k) {
            val = k;
            next = NULL;
        }
};

class Solution {
public:
    int isagreaterthanb(string a, string b) {
        string ab = a + b;
        string ba = b + a;
        if(ab > ba) {
            return 1;
        }
        else {
            return 0;
        }
        int i = 0, j = 0;
        while(i < a.size() && j < b.size()) {
            if(a[i] > b[j]) return 1;
            else if( a[i] < b[j]) return 0;
            else {
                i++;
                j++;
            }

        }
        if(i == a.size()) { // a is smaller in length
            i--;
            while(i >= 0 && j >= 0) {
                if(a[i] > b[j]) return 1;
                else if ( a[i] < b[j] ) return 0;
                else { 
                    i--;
                    j--;
                }
            }
        }
        if(j == b.size()) { // b is smaller in length
            j--;
            while(i < a.size() && j >= 0) {
                if(a[i] > b[j]) return 1;
                else if (a[i] < b[j]) return 0;
                else {
                    i++;
                    j--;
                }
            }
            
            
        }
        return 0;
    }
    string largestNumber(vector<int>& nums) {
        vector<string> strarr;
        for(int i = 0; i < nums.size(); i++) {
            strarr.push_back(to_string(nums[i]));
        }
        vector<Node *> hashmap(10, NULL);
        for(int i = 0; i < strarr.size(); i++) {
            int hashmap_index = strarr[i][0] - '0';
            cout<<hashmap_index;
            Node *temp = new Node(strarr[i]);
            if(hashmap[hashmap_index] == NULL) {
                hashmap[hashmap_index] = temp;
            }
            else {
               Node *curr = hashmap[hashmap_index];
               Node *prev = curr;
               
               while(curr) {
                    string a = curr->val;
                    string b = temp->val;
                    int x = isagreaterthanb(a, b);
                    if(x == 1) { // already sorted, move ahead
                        prev = curr;
                        curr = curr->next;
                    }
                    else { // need to insert in this place
                        break;
                    }
               } 
               if(curr == NULL) {
                    prev->next = temp;
               }
               else if ( curr == hashmap[hashmap_index] ) {
                    hashmap[hashmap_index] = temp;
                    temp->next = curr;
               }
               else {
                    prev->next = temp;
                    temp->next = curr;
               }
            }
        }
        string res;
        for(int i = 9; i >= 0; i--) {
            Node *t = hashmap[i];
            cout<<i<<" -> ";
            while(t) {
                cout<<t->val;
                res = res + (t->val);
                t = t->next;
            }
            cout<<endl;
        }
        return res[0] == '0' ? "0" : res;
    }
};
