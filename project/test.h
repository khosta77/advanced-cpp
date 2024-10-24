#ifndef CPP_COURSE_TEST_H_
#define CPP_COURSE_TEST_H_

#include "lru.h"

#include <cassert>

struct TestObj
{
    std::string _key;
    int _value;
    bool _put;
};

extern std::vector<TestObj> BaseFromLeetCode;
extern std::vector<TestObj> DoubleKey;
extern std::vector<TestObj> EmptyCache;
extern std::vector<TestObj> ManyDoubleElement;

bool TEST_CACHE_PUTaGET( const int& capacity, std::vector<TestObj>& testScript );
void test();



#endif  // CPP_COURSE_TEST_H_
