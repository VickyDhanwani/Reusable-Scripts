"""
Problem: Move Zeroes
Given an integer array nums, move all 0's to the end while maintaining the relative order of non-zero elements. Do this in-place without making a copy of the array.
Example:

Input: [0,1,0,3,12]
Output: [1,3,12,0,0]

Constraints:

Time Complexity: O(n)
Space Complexity: O(1) - in-place
You can modify the array

"""

def move_zeros(arr):
    i = j = 0
    while j < len(arr):
        if(arr[j] != 0):
            arr[i] = arr[j];
            i += 1
        j += 1
    while i < len(arr):
        arr[i] = 0
        i += 1
    return arr

if __name__ == "__main__":
    arr = [0, 1, 0, 3,12]
    print(move_zeros(arr))
  