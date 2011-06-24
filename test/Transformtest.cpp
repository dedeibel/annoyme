
#include <string>
#include <sstream>
#include <set>
#include <list>
#include <algorithm>
#include <vector>
#include <functional>
#include <iterator>

#include <ostream> // debug
#include <iostream> // debug

class A {
  private:
  int mult;

  public:
  A() : mult(2) {
  }
  static int trans(int d) {
    return 2*d;
  }

};

int main(int argc, char **argv) {

  
	std::set<int> d;
  d.insert(2);
  d.insert(7);
  d.insert(8);
  d.insert(12);
	std::list<int> s;
  A a;
  std::transform(d.begin(), d.end(), s.begin(), A::trans);

  std::copy(s.begin(), s.end(), std::ostream_iterator<int>(std::cout, ", "));
  std::cout << std::endl;

  return 0;
}
