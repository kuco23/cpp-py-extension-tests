#include <iostream>
#include <cstring>
#include <functional>
#include <algorithm>
#include <vector>
#include "conio.h"

using std::vector;
using std::cout;
using std::endl;

void dispar(int ar[], size_t len) {
  for (int i = 0; i < len; i++) {
    cout << ar[i] << endl;
  }
}

enum en {
  A, B, C
};

int main() {
  int a[4] = {0};
  for (int i = 0; i < 4; i++) {
    cout << a[i] << endl;
  }
  getch();
  return 0;
}
