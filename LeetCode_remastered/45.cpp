//Eclipsemos @UIUC 
//Created on 2024/09/20 21:56:18
#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define vc vector<int>

//Paste Class Solution here

class Solution 
{
public:
    int jump(vector<int>& nums) 
    {
        //jump v2
        int ans = 0;
        int dp[100001];
        memset(dp,0,sizeof(dp));
        int len = nums.size();
        dp[0] = 1;
        int current_jump = 0;
        int far = 0;
        for(int i = 0;i<len-1;i++)
        {
            far = max(far,i+nums[i]);
            if(i==current_jump)
            {
                ans++;
                current_jump = far;
            }
        }
        return ans;
    }
};


int main()
{
    Solution S;
    // vc v={};

    return 0;
}