/*
Branko Milosevic
Coursera C++ For C Programmers part A

=== wk01 assignment 01 ===

Convert this program to C++
* change to C++ io
* change to one line comments
* change defines of constants to const
* change array to vector<>
* inline any short function
*/

#include <iostream>     // change to cpp standard io library
#include <vector>

const int N = 40;       // change to const int declaration of global constant
using namespace std;

// function [sum] adds up all members of the vector [d] of size [n] to the input variable [p]
inline void sum(int*p, int n, vector<int>& d)
{
    int i;
    *p = 0;

    for(i = 0; i < n; ++i)
        *p = *p + d[i];
}

int main()
{
    int i;
    int accum = 0;
    vector <int> data(N);

    for(i = 0; i < N; ++i)
        data[i] = i;

    sum(&accum, N, data);

    cout << "sum is: " << accum << endl;

    return 0;
}
