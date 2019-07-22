#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

vector<int> getprimesimpl(int n) {
  if (n <= 1) return {};
  if (n == 2) return {2};
  vector<int> primes = {2}; bool mark;
  for (int i = 3; i < n; i+=2) {
    mark = true;
    for (int j = 3; j <= ceil(pow(i, 0.5)); j+=2) {
      if (i % j == 0) {
        mark = false;
        break;
      }
    }
    if (mark) primes.push_back(i);
  }
  return primes;
}

int main() {
  vector<int> sez = getprimesimpl(10);
  for (auto &i : sez) {
    cout << i << endl;
  }
}
