#include <iostream>
#include <string>
#include <typeinfo>
using namespace std;

namespace lz {

template<typename T>
string ClassString() {
  int index = 0;
  string s = typeid(T).name();
  for(index = 0; index < s.length(); index++) {
    if (s[index] > '9' || s[index] < '0')
      break;
  }
  return s.substr(index);
};

template<typename T>
class Object {
public:
  string ToString() {
    return ClassString<T>();
  };
};

}

