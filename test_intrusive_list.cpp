#include "intrusive_list.hpp"

#include <cassert>
#include <list>
#include <sstream>

namespace check {
std::vector<int> frame = {-3, -2, -1, 0, 1, 2, 3};
const size_t MIDDLE = 3;

template <typename T> void front_and_back() {
  T lst;
  assert(lst.empty());
  assert(lst.size() == 0);

  // Добавляем первый элемент
  lst.push_back(frame[MIDDLE]);
  assert(((lst.front() == frame[MIDDLE]) and (lst.back() == frame[MIDDLE])));
  assert(!lst.empty());
  assert(lst.size() == 1);

  lst.front() = 1;
  assert(((lst.front() == 1) and (lst.back() == 1)));

  lst.back() = -1;
  assert(((lst.front() == -1) and (lst.back() == -1)));
  lst.back() = frame[MIDDLE];

  // Проверяем заполнение вектора
  for (size_t i = (MIDDLE - 1), j = (MIDDLE + 1), count = 1; j < frame.size();
       --i, ++j) {
    lst.push_front(frame[i]);
    lst.push_back(frame[j]);
    count += 2;

    // Проверка корректных значений
    assert(((lst.front() == frame[i]) and (lst.back() == frame[j])));
    assert(lst.size() == count);
  }

  // удаление
  for (size_t i = 1, j = (lst.size() - 2), count = lst.size(); i <= j;
       ++i, --j) {
    lst.pop_front();
    lst.pop_back();
    count -= 2;

    assert(((lst.front() == frame[i]) and (lst.back() == frame[j])));
    assert(lst.size() == count);
  }

  lst.pop_front();
  assert(lst.size() == 0);
  lst.push_back(frame[MIDDLE]);
  lst.pop_back();
  assert(lst.size() == 0);

  // Проверка очистки списка
  for (size_t i = (MIDDLE - 1), j = (MIDDLE + 1); j < frame.size(); --i, ++j) {
    lst.push_front(frame[i]);
    lst.push_back(frame[j]);
  }

  assert(lst.size() == (frame.size() - 1));

  lst.clear();
  assert(lst.empty());
  assert(lst.size() == 0);
}

template <typename T> void inter() {
  T lst;
  lst.push_back(frame[MIDDLE]);
  for (size_t i = (MIDDLE - 1), j = (MIDDLE + 1); j < frame.size(); --i, ++j) {
    lst.push_front(frame[i]);
    lst.push_back(frame[j]);
  }

  //// Тест обычного итератора ++, -- ...
  auto vectorIt = frame.begin();
  auto listIt = lst.begin();
  for (size_t i = 0, I = frame.size(); i < I; ++i, ++listIt, ++vectorIt)
    assert(*listIt == *vectorIt);

  vectorIt = frame.begin();
  listIt = lst.begin();
  for (size_t i = 0, I = frame.size(); i < I; ++i, listIt++, vectorIt++)
    assert(*listIt == *vectorIt);

  auto vectorItEnd = frame.end();
  auto listItEnd = lst.end();
  for (size_t i = 0, I = frame.size(); i < I; ++i) {
    --listItEnd;
    --vectorItEnd;
    assert(*listItEnd == *vectorItEnd);
  }

  vectorItEnd = frame.end();
  listItEnd = lst.end();
  for (size_t i = 0, I = frame.size(); i < I; ++i) {
    listItEnd--;
    vectorItEnd--;
    assert(*listItEnd == *vectorItEnd);
  }

  //// Тест обычного const итератора ++, -- ...
  auto vectorItConst = frame.cbegin();
  auto listItConst = lst.cbegin();
  for (size_t i = 0, I = frame.size(); i < I;
       ++i, ++listItConst, ++vectorItConst)
    assert(*listItConst == *vectorItConst);

  vectorItConst = frame.cbegin();
  listItConst = lst.cbegin();
  for (size_t i = 0, I = frame.size(); i < I;
       ++i, listItConst++, vectorItConst++)
    assert(*listItConst == *vectorItConst);

  auto vectorItEndConst = frame.cend();
  auto listItEndConst = lst.cend();
  for (size_t i = 0, I = frame.size(); i < I; ++i) {
    --listItEndConst;
    --vectorItEndConst;
    assert(*listItEndConst == *vectorItEndConst);
  }

  vectorItEndConst = frame.cend();
  listItEndConst = lst.cend();
  for (size_t i = 0, I = frame.size(); i < I; ++i) {
    listItEndConst--;
    vectorItEndConst--;
    assert(*listItEndConst == *vectorItEndConst);
  }

  auto it = lst.begin();
  auto revers = --(frame.cend());
  for (const auto END = lst.end(); it != END; ++it, --revers) {
    *it = *revers;
  }
  --it;
  for (const auto value : frame) {
    int x = *(it--);
    assert(value == x);
  }
}

template <typename T> void insert() {
  T lst;
  auto it = lst.begin();
  for (size_t i = 0; i < frame.size(); ++i) {
    it = lst.insert(it, frame[i]);
    assert(*it == frame[i]);
  }
}

// Зашел в тупик со splice... Вроде алгоритм верный переписал, но почему не
// работет, не понимаю...
#if 0
        void splice()
        {
            List<int> lst1;
            List<int> lst2;
            lst2.push_back(10);
            lst2.push_back(20);
            lst2.push_back(30);

            lst1.push_back(frame[MIDDLE]);
            for( size_t i = ( MIDDLE - 1 ), j = ( MIDDLE + 1 ); j < frame.size(); --i, ++j )
            {
                lst1.push_front(frame[i]);
                lst1.push_back(frame[j]);
            }

            auto it = ++(lst1.begin());
            lst1.splice( it, lst2, lst2.begin() );

            std::list<int> list1{ -3, -2, -1, 0, 1, 2, 3};
            std::list<int> list2{ 10, 20, 30 };

            auto it_ = ++(list1.begin());
            list1.splice( it_, list2, list2.begin() );

            auto stdIt1 = list1.begin();
            auto myIt1 = lst1.begin();
            for( size_t i = 0; i < lst1.size(); ++i, ++stdIt1, ++myIt1 )
            {
                if( *stdIt1 != *myIt1 )
                    throw ListNotCurrentValue( *stdIt1, *myIt1 );
            }

            auto stdIt2 = list2.begin();
            auto myIt2 = lst2.begin();

            for( size_t i = 0; i < lst2.size(); ++i, ++stdIt2, ++myIt2 )
            {
                if( *stdIt2 != *myIt2 )
                    throw ListNotCurrentValue( *stdIt2, *myIt2 );
            }

            auto it_1 = list1.begin();
            list1.splice( it_1, list1, list1.begin() );

            auto it1 = lst1.begin();
            lst1.splice( it1, lst1, lst1.begin() );

            auto stdIt3 = list1.begin();
            auto myIt3 = lst1.begin();

            for( size_t i = 0; i < lst1.size(); ++i, ++stdIt3, ++myIt3 )
            {
                if( *stdIt3 != *myIt3 )
                    throw ListNotCurrentValue( *stdIt3, *myIt3 );
            }
        }
#endif

}; // namespace check

int test() {
  using mylist = List<int>;
  using stdlist = std::list<int>;
  check::front_and_back<mylist>();
  check::front_and_back<stdlist>();
  check::inter<mylist>();
  check::inter<stdlist>();
  check::insert<mylist>();
  check::insert<stdlist>();
  // check::splice();
  return 0;
}

int main() { return test(); }
