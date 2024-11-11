#include "lru.hpp"

#include <cassert>
#include <sstream>

class TestException : std::exception {
private:
  std::string emsg;

public:
  explicit TestException(const std::string &msg) : emsg(msg) {}
  const char *what() const noexcept override { return emsg.c_str(); }
};

namespace LRUTestSpace {
struct TestObj {
  std::string _key;
  int _value;
  bool _put;
};

std::vector<TestObj> BaseFromLeetCode = {
    {"1", 1, true},   {"2", 2, true},   {"1", 1, false},
    {"3", 3, true},   {"2", -1, false}, {"4", 4, true},
    {"1", -1, false}, {"3", 3, false},  {"4", 4, false}};

std::vector<TestObj> DoubleKey = {{"A", 1, true},  {"A", 1, false},
                                  {"A", 1, false}, {"B", 2, true},
                                  {"A", 3, true},  {"A", 3, false}};

std::vector<TestObj> EmptyCache = {{"A", -1, false}};

std::vector<TestObj> ManyDoubleElement = {
    {"A", 1, true}, {"A", 1, true}, {"A", 1, true}, {"A", 1, true},
    {"A", 1, true}, {"A", 1, true}, {"A", 1, false}};

bool TEST_CACHE_PUTaGET(const int &capacity, std::vector<TestObj> &testScript) {
  LRUCache<std::string, int> cache(capacity);
  int value = 0;
  for (const TestObj &testObj : testScript) {
    if (testObj._put) {
      try {
        cache.insert(testObj._key, testObj._value);
      } catch (const LRUCCapacityEquallNull &capNull) {
        if (capacity == 0)
          continue;
        return false;
      } catch (const std::exception &emsg) {
        return false;
      };
    } else {
      try {
        value = cache.at(testObj._key);
      } catch (const LRUCCapacityEquallNull
                   &capNull) // Если размер изначальный равен 0
      {
        if (capacity == 0)
          continue;
        return false;
      } catch (const std::exception &emsg) {
        if (testObj._value != -1) {
          return false;
        }
        continue;
      };

      if (value != testObj._value)
        return false;
    }
  }
  return true;
}

void test() {
  assert(TEST_CACHE_PUTaGET(2, BaseFromLeetCode));
  assert(TEST_CACHE_PUTaGET(3, DoubleKey));
  assert(TEST_CACHE_PUTaGET(2, EmptyCache));
  assert(TEST_CACHE_PUTaGET(0, BaseFromLeetCode));
  assert(TEST_CACHE_PUTaGET(2, ManyDoubleElement));
}
}; // namespace LRUTestSpace

namespace LRUHWTestSpace {

namespace test_xx {
void test_01() {
  std::stringstream input, output;
  input << "2 4" << std::endl;
  input << "v1\t1e-3 1e-2 1e-1" << std::endl;
  input << "v2" << std::endl;
  input << "v1" << std::endl;
  loop(input, output);
  assert(output.str() == "!STORED!\n!NOEMBED!\n0.001 0.01 0.1\n");
}

void test_02() {
  std::stringstream input, output;
  input << "2 2" << std::endl;
  input << "v1\t" << std::endl;
  loop(input, output);
  assert(output.str() == "!STORERR!\n");
}

void test_03() {
  std::stringstream input, output;
  input << "2 2" << std::endl;
  input << "v1\t1e-2 1e-1" << std::endl;
  input << "v2\t1e-1 1e-2" << std::endl;
  input << "v1" << std::endl;
  input << "v2" << std::endl;
  input << "v3\t1e1 1e2" << std::endl;
  input << "v3" << std::endl;
  input << "v1" << std::endl;
  loop(input, output);
  assert(
      output.str() ==
      "!STORED!\n!STORED!\n0.01 0.1\n0.1 0.01\n!STORED!\n10 100\n!NOEMBED!\n");
}

void test_04() {
  std::stringstream input, output;
  input << "2 1" << std::endl;
  input << "v1\t1e-2 1e-1" << std::endl;
  input << "v2\t1e-1 1e-2" << std::endl;
  input << "v1" << std::endl;
  input << "v2" << std::endl;
  input << "v3\t1e1 1e2" << std::endl;
  input << "v3" << std::endl;
  input << "v1" << std::endl;
  loop(input, output);
  assert(output.str() ==
         "!STORED!\n!STORED!\n0.01\n0.1\n!STORED!\n10\n!NOEMBED!\n");
}

void test_05() {
  std::stringstream input, output;
  input << "2 1" << std::endl;
  input << "v1\t" << std::endl;
  loop(input, output);
  assert(output.str() == "!STORERR!\n");
}

void test_06() {
  std::stringstream input, output;
  input << "2 3" << std::endl;
  input << "v1\t1e3" << std::endl;
  input << "v1" << std::endl;
  input << "v1\t1e2" << std::endl;
  input << "v1" << std::endl;
  loop(input, output);
  assert(output.str() ==
         "!STORED!\n1000\n!STORED!\n1000 100\n");
}

}; // namespace test_xx

void test() {
  test_xx::test_01();
  test_xx::test_02();
  test_xx::test_03();
  test_xx::test_04();
  test_xx::test_05();
  test_xx::test_06();
}

}; // namespace LRUHWTestSpace

int main() {
  LRUTestSpace::test();
  LRUHWTestSpace::test();
  return 0;
}
