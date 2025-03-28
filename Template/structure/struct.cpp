// Eclipsemos @UIUC
// Created on 2025/03/28 07:37:24
//
#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define vc vector<int>

// Paste Class Solution here
struct node
{
    int x;
    int y;
    int z;
    node() : x(0), y(0), z(0) {}
    node(int x, int y, int z) : x(x), y(y), z(z) {}
} a;

template <typename T>
T add(T a, T b)
{
    return a + b;
}

template <typename T>
struct Box {
    T value;
    Box(T v) : value(v) {}

    void show() {
        std::cout << "Value: " << value << "\n";
    }
};


int main()
{
    int x = add(3, 4);                              // int
    double y = add(2.5, 4.3);                       // double
    string z = add(string("hi "), string("there")); // string

    // vc v={};

    return 0;
}