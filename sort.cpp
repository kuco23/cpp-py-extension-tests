#include <iostream>
#include <cstring>
#include <functional>
#include <algorithm>
#include "conio.h"

using std::cout;
using std::endl;

void dispar(int ar[][2], size_t len) {
  for (int i = 0; i < len; i++) {
    cout << ar[i][0] << " " << ar[i][1] << endl;
  }
}


inline void simplesort(int ar[][2], size_t len) {
  std::qsort(ar, len, sizeof(*ar),
        [](const void *arg1, const void *arg2)->int  {
            int const *lhs = static_cast<int const*>(arg1);
            int const *rhs = static_cast<int const*>(arg2);
            return (lhs[0] < rhs[0]) ? -1 : 1;
        });
}


int main() {
  int ar[][2] = {{2,1}, {1,2}, {0,2}, {6,0}};
  size_t len = sizeof(ar) / sizeof(*ar);
  dispar(ar, len);
  simplesort(ar, len);
  cout << endl;
  dispar(ar, len);
  getch();
  return 0;
}
