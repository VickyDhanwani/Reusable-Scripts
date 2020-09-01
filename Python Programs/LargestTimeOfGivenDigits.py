"""

Given an array of 4 digits, return the largest 24 hour time that can be made.

The smallest 24 hour time is 00:00, and the largest is 23:59.  Starting from 00:00, a time is larger if more time has elapsed since midnight.

Return the answer as a string of length 5.  If no valid time can be made, return an empty string.

Example 1:

Input: [1,2,3,4]
Output: "23:41"
Example 2:

Input: [5,5,5,5]
Output: ""

"""

class Solution:
    def largestTimeFromDigits(self, A: List[int]) -> str:
        times = [
            [A[0], A[1], A[2], A[3]],
            [A[0], A[1], A[3], A[2]],
            [A[0], A[2], A[1], A[3]],
            [A[0], A[2], A[3], A[1]],
            [A[0], A[3], A[1], A[2]],
            [A[0], A[3], A[2], A[1]],
            
            [A[1], A[2], A[3], A[0]],
            [A[1], A[2], A[0], A[3]],
            [A[1], A[0], A[2], A[3]],
            [A[1], A[0], A[3], A[2]],
            [A[1], A[3], A[0], A[2]],
            [A[1], A[3], A[2], A[0]],
            
            [A[2], A[0], A[1], A[3]],
            [A[2], A[0], A[3], A[1]],
            [A[2], A[1], A[0], A[3]],
            [A[2], A[1], A[3], A[0]],
            [A[2], A[3], A[0], A[1]],
            [A[2], A[3], A[1], A[0]],
            
            [A[3], A[0], A[1], A[2]],
            [A[3], A[0], A[2], A[1]],
            [A[3], A[1], A[0], A[2]],
            [A[3], A[1], A[2], A[0]],
            [A[3], A[2], A[0], A[1]],
            [A[3], A[2], A[1], A[0]],
            
        ]
        min_time = 0
        max_time = (23 * 60) + 59;
        curr_max = 0;
        res = ""
        for y in times:
            print(y)
            
            if y[0] > 2:
                continue
            if y[0] == 2 and y[1] > 3:
                continue
            if y[2] > 5:
                continue
            
            curr = ((y[0] * 10 + y[1]) * (60)) + (y[2] * 10 + y[3])
            
            if curr >= min_time and curr <= max_time:
                if curr >= curr_max:
                    
                    hrs = y[0] * 10 + y[1]
                    mins = y[2] * 10 + y[3]
                    
                    if hrs > 23 or mins > 59:
                        continue
                    
                    curr_max = curr
                    
                    if hrs == 0 and mins == 0:
                        res = "00:00"
                    elif hrs < 10 and mins >= 10:
                        res = "0" + str(hrs) + ":" + str(mins)
                        
                    elif hrs < 10 and mins < 10:
                        res = "0" + str(hrs) + ":" + "0" + str(mins)
                    elif hrs >= 10 and mins < 10:
                        res = str(hrs) + ":" + "0" + str(mins)
                    else:
                        res = (str(hrs) + ":"+ str(mins))
        
        return res
