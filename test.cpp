#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

int main() {
  unordered_map<pair<int, int>, int> lmao;
  lmao.insert({1, 2}, 3);
  cout << lmao[{1, 2}] << '\n';
}
