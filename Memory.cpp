#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define vc vector<int>
//
class Solution
{
private:
    int memory_value = 1;

public:
    void wpm(int value)
    {
        cout << "Memory write" << endl;
        memory_value = value;
        return;
    }

    void rpm()
    {
        cout << "Memory read: " << memory_value << endl;
        return;
    }
};
//
int main()
{
    Solution S;
    // vc v={};
    S.wpm(4);
    S.rpm();
    return 0;
}