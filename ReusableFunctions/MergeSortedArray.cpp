class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        vector<int> finalArray;
        int i = 0, j = 0;
        int k = 0;
        while(i < m && j < n) {
            if(nums1[i] < nums2[j]) {
                finalArray.push_back(nums1[i]);
                k++;
                i++;
            }
            else if(nums2[j] < nums1[i]) {
                finalArray.push_back(nums2[j]);
                j++;
                k++;
            }
            else {
                finalArray.push_back(nums1[i]);
                i++;
                k++;
                finalArray.push_back(nums2[j]);
                j++;
                k++;
            }
        }
        while(i < m) {
            finalArray.push_back(nums1[i]);
            i++;
            k++;
        }
        while( j < n) {
            finalArray.push_back(nums2[j]);
            j++;
            k++;
        }
        for(i = 0; i < finalArray.size(); i++) {
            nums1[i] = finalArray[i];
        }
        
    }
};
