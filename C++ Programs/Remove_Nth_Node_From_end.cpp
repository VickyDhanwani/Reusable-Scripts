/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode *a, *b, *prev;
        a = head;
        if(!a->next && n == 1) return NULL;
        int t = n - 1; // 1
        while(t) {
            t--;
            a = a->next;
        }
        b = head;
        prev = b;
        if(!a->next) {
            b = b->next;
            return b;
        }
        while(a->next) {
            prev = b;
            b = b->next;
            a = a->next;
        }
        prev->next = b->next;
        return head;
    }
};
