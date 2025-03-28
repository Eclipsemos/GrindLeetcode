// Eclipsemos @UIUC
// Created on 2024/09/20 21:28:22
#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define vc vector<int>

// Paste Class Solution here
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int n = nums.size();
        if(n<=1)
            return true;
        int cur_max = 0;
        for(int i=0;i<n;i++)
        {
            if(cur_max<i)
                break;
            cur_max = max(cur_max,nums[i]+i);
        }
        return cur_max>=n-1;
    }
};

int main()
{
    Solution S;
    // vc v={};

    return 0;
}