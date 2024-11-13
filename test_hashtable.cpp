#include "hashtable.hpp"

#include <cassert>
#include <sstream>
#include <string>

namespace check {
std::vector<std::pair<std::string, int>> frame = {
    {"a", 10}, {"b", 20}, {"c", 30}};

std::ostream &operator<<(std::ostream &os,
                         const std::pair<std::string, int> &pr) {
  os << pr.first << ' ' << pr.second << std::endl;
  return os;
}

template <typename T> void insert() {
  // Корректно вставляем
  T table;
  assert(table.empty());

  for (const auto &[key, value] : frame)
    table.insert(std::pair<std::string, int>{key, value});

  assert(!table.empty());
  assert(table.size() == frame.size());

  // Проверка итератора
  for (auto [key, value] : frame) {
    bool noElement = true;

    // Проверка итератора
    for (const auto [table_key, table_value] : table) {
      if (((table_key == key) and (table_value == value)))
        noElement = false;
    }

    assert(!noElement);
    assert(table.count(key) == 1);

    auto it_finded = table.find(key);
    assert((((*it_finded).first == key) and ((*it_finded).second == value)));

    assert(table[key] == value);
  }

  for (auto it = table.begin(); it != table.end(); it++) {
    assert(it == table.find((*it).first));
  }

  table.clear();
}

template <typename T> void erase() {
  // Корректно вставляем
  T table;
  for (const auto &[key, value] : frame)
    table.insert(std::pair<std::string, int>{key, value});

  for (const auto &[key, value] : frame)
    assert(table.erase(key));
  assert(table.erase(frame[0].first) == 0);
  table.clear();
}

template <typename T> void clear() {
  T table(10);
  float lf = 0.0f;
  float mlf = 0.0f;
  table.max_load_factor(1.0f);

  for (const auto &[key, value] : frame)
    table.insert(std::pair<std::string, int>{key, value});

  // Тестирование load_factor
  lf = table.load_factor();
  assert(lf <= 0.3f);

  mlf = table.max_load_factor();
  assert(mlf == 1.0f);

  table.max_load_factor(0.01f);
  mlf = table.max_load_factor();
  assert(mlf == lf);

  // Тестирование clear
  table.clear();

  assert(table.empty());

  // Тестирование load_factor
  lf = table.load_factor();
  assert(lf == 0.0f);
  table.clear();
}

template <typename T> void reserve() {
  T table(10);
  assert(table.load_factor() == 0.0f);

  for (const auto &[key, value] : frame)
    table.insert(std::pair<std::string, int>{key, value});

  table.reserve(0);
  assert(table.load_factor() == 1.0f);

  table.reserve(100);
  assert(table.load_factor() <= 0.03f);
  table.clear();
}

template <typename T> void iter() {
  T table(10);
  auto it = table.find("A");
  assert(it == table.end());
  assert(table.erase("A") == 0);
  table.clear();
}
}; // namespace check

int test() {
  using HT = HashTable<std::string, int>;
  using UM = std::unordered_map<std::string, int>;
  check::insert<HT>();
  check::erase<HT>();
  check::clear<HT>();
  check::reserve<HT>();
  check::iter<HT>();
  check::insert<UM>();
  check::erase<UM>();
  check::clear<UM>();
  check::reserve<UM>();
  check::iter<UM>();
  check::frame.clear();
  return 0;
}

int main() { return test(); }
