#include <iostream>
#include <vector>
#include <stack>
#pragma once

#include "Runs.h"
#include <assert.h>

using namespace std;

void testRunManager() {
    int a[] = { 1, 2, 3, 4, 5, 1, 3, 3, 3, 9, 8, 7, 6 };
    RunManager rM(ITimSortParamsDefault(), 13);
    stack <pair <int*, int*> > stackRes = rM.splitIntoRuns(a, a + 13);
    stack <pair <int*, int*> > stackAns;
    stackAns.push(make_pair(a, a + 5));
    stackAns.push(make_pair(a + 5, a + 10));
    stackAns.push(make_pair(a + 10, a + 12));
    stackAns.push(make_pair(a + 12, a + 13));
    int aRes[] = { 1, 2, 3, 4, 5, 1, 3, 3, 3, 9, 7, 8, 6 };
    for (int i = 0; i < 13; ++i){
        assert(a[i] == aRes[i]);// "RunsManage failed bad sort");
    }
    assert(stackRes == stackAns);// "RunsManage failed wrong returning stack");
}

/*
int aaa[1000000];
int b[1000000];
int c[1000000];
int x[] = { 19, 16, 15, 15, 10, 17, 5, 1, 8, 13, 7, 14, 12, 14, 10, 16, 11, 13 };

int main() {
//timSort(x, x + 18);
//for (;;);
//testRunManager();
for (int i = 0; i < 7000; ++i) {
int n = (rand() * rand()) % 150000;
for (int j = 0; j < n; j++) {
aaa[j] = (rand() *rand() % 155000);
b[j] = aaa[j];
c[j] = aaa[j];
}
std::sort(b, b + n);
int time0 = clock();
timSort(aaa, aaa + n);
cout << n << " " << clock() - time0 << endl;
for (int j = 0; j < n; ++j) {
if (aaa[j] != b[j]) {
cout << "Wrong" << endl;
cout << n << endl;
for (int k = 0; k < n; k++) {
cout << c[k] << " ";
}
cout << endl;
for (int k = 0; k < n; k++) {
cout << aaa[k] << " ";
}
cout << endl;
for (int k = 0; k < n; k++) {
cout << b[k] << " ";
}
cout << endl;
for (;;);
}
}
}
int a[] = { 1, 2, 3, 4, 0, -1, -2, -3, -4, 2, 3, 3, 2, 10, 1, 10, -100 };
print(a, a + 14);
timSort(a, a + 14);
print(a, a + 14);
for (;;);
}

*/