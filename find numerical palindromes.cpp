#include <iostream>
#include <cmath>

bool isNumPal(int n) {
  n = abs(n);
  std::string number = "";
  int stringnum = n;
  int len = 0;
  int digitnum = n;

  while (digitnum != 0) {
    len ++;
    digitnum /= 10;
  }

  //std::cout << len << std::endl;

  std::string flipped = "";
  for (int j = 0; j < len; j++) {
    flipped += '0';
  }

  for (int k = 0; k < len; k++) {
    number += '0';
  }

  //std::cout << number << std::endl;
  //std::cout << flipped << std::endl;


  for (int l = 0; l < len; l++) {
    char add = (stringnum % 10) + 48;
    number[l] = add;
    stringnum /= 10;
  }

  //std::cout << number << std::endl;

  for (int i = 1; i <= len; i++) {
    char add = (n % 10) + 48;
    flipped[len - i] = add;
    n /= 10;
  }

  //std::cout << flipped << std::endl;


  bool ret;

  if (number == flipped){
    ret = true;
  }
  else {
    ret = false;
  }

  if (len == 1) {
    ret = true;
  }

  return ret;
}

int main() {
  int n;
  std::cin >> n;
  if (isNumPal(n) == true) {
    std::cout << n << " is a numeric palindrome." << std::endl;
  }
  else {
    std::cout << "Not a palindrome." << std::endl;
  }
}