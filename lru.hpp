#ifndef CPP_COURSE_PROJECR_LRUCACHE_H_
#define CPP_COURSE_PROJECR_LRUCACHE_H_

#include <exception>
#include <iostream>
#include <list>
#include <map>
#include <string>

/** @class LRUCacheException - родительский класс ошибок для всего LRUcache,
 * выводит сообщение с ошибкой
 * */
class LRUCacheException : public std::exception {
public:
  explicit LRUCacheException(const std::string &msg) : m_msg(msg) {}
  const char *what() const noexcept override { return m_msg.c_str(); }

private:
  std::string m_msg;
};

/** @class LRUCCapacityEquallNull - класс ошибки, если capacity == 0
 * */
class LRUCCapacityEquallNull : public LRUCacheException {
public:
  LRUCCapacityEquallNull() : LRUCacheException("capacity is null") {}
};

/** @class LRUCKeyNotFind класс ошибки если ключ не найден
 * */
class LRUCKeyNotFind : public LRUCacheException {
public:
  LRUCKeyNotFind() : LRUCacheException("key not find") {}
};

#include "hashtable.hpp"
#include "intrusive_list.hpp"

/** class LRUCache - Least Recently Used (LRU) Cache
 *                   Методы класса я старался по максимому и возможности взять
 * из std::unordered_map
 * */
template <typename K, typename T> class LRUCache {
private:
  size_t _capacity;

  using LRU_pair = std::pair<K, T>;
  using LRU_list = std::list<LRU_pair>;
  using LRU_list_iterator = typename LRU_list::iterator;
  using LRU_table = HashTable<K, LRU_list_iterator>;
  using iterator = LRU_table::iterator;

  LRU_table _hashTable;
  LRU_list _cacheList;

  /** @brief checkCapacityIsZero - Часто надо проверять _capacity, обернул в
   * inline функцию
   * */
  inline void checkCapacityIsZero() {
    if (_capacity == 0)
      throw LRUCCapacityEquallNull();
  }

  /** @brief checkKeyIsFinding - Часто надо проверять ключ, обернул в inline
   * функцию
   * */
  inline void checkKeyIsFinding(const K &key) {
    if (!_hashTable.count(key))
      throw LRUCKeyNotFind();
  }

public:
  //// Member functions
  /** @brief Базовый конструктор
   * */
  LRUCache(const size_t &capacity = 0)
      : _capacity(capacity), _hashTable(capacity) {}

  /** @brief Конструктор копирования
   * */
  // LRUCache( const LRUCache<K, T>& rhs ) : _capacity(rhs._capacity),
  // _cacheList(rhs._cacheList),
  //     _hashTable(rhs._hashTable) {}

  /** @brief Конструктор перемещения
   * */
  // LRUCache( LRUCache<K, T>&& rhs ) : _capacity(rhs._capacity),
  // _cacheList(rhs._cacheList),
  //     _hashTable(rhs._hashTable) {}

  /** @brief Деструктор
   * */
  ~LRUCache() { clear(); }

  //// iterator
  iterator begin() { return iterator(_hashTable); }
  iterator end() { return iterator(_hashTable); }

  //// Capacity
  /** @brief empty - проверка не пуста ли наша таблица
   * */
  bool empty() const noexcept { return _hashTable.empty(); }

  /** @brief size - размер заполненого хеша
   * */
  size_t size() const noexcept { return _hashTable.size(); }

  //// Modifiers
  /** @brief clear - очистить все
   * */
  void clear() noexcept {
    _cacheList.clear();
    _hashTable.clear();
  }

  /** @brief insert - вставка пары
   * */
  void insert(const LRU_pair &pr) { insert(pr.first, pr.second); }

  /** @brief insert - вставка ключ+значение
   * */
  void insert(const K &key, const T &value) {
    checkCapacityIsZero();
    if (_hashTable.count(key)) {
      LRU_list buffer;
      buffer.push_front(LRU_pair(key, value));
      _hashTable.erase(key);
      _hashTable.insert(key, buffer.begin());
      _cacheList.splice(_cacheList.begin(), _cacheList, _hashTable[key]);
    } else {
      if (_cacheList.size() == _capacity) {
        K lastKey = _cacheList.back().first;
        _hashTable.erase(lastKey);
        _cacheList.pop_back();
      }
      _cacheList.push_front(LRU_pair(key, value));
      _hashTable.insert(key, _cacheList.begin());
    }
  }

  /** @brief erase - удаление элемента по ключу
   * */
  void erase(const K &key) {
    checkCapacityIsZero();
    checkKeyIsFinding(key);

    _cacheList.erase(_hashTable[key]);
    _hashTable.erase(key);
  }

  //// Lookup
  /** @brief at - взятие элемета с тотальной проверкой, есть возможность после
   * его редактировать
   * */
  T &at(const K &key) {
    checkCapacityIsZero();
    checkKeyIsFinding(key);

    return this->operator[](key);
  }

  /** @brief at - взятие элемета с тотальной проверкой
   * */
  const T &at(const K &key) const {
    checkCapacityIsZero();
    checkKeyIsFinding(key);

    return this->operator[](key);
  }

  /** @brief operator[] - взятие элемета, есть возможность после его
   * редактировать
   * */
  T &operator[](const K &key) {
    _cacheList.splice(_cacheList.begin(), _cacheList, _hashTable[key]);
    return (T &)(*_hashTable[key]).second;
  }

  //// other
  /** @brief recapacity - изменить размер _capacity, очистить все данные что
   * были до, мало ли
   * */
  void recapacity(const size_t &newCapacity) noexcept {
    clear();
    _capacity = newCapacity;
  }

  /** @brief getCapacity - получить _capacity
   * */
  size_t getCapacity() const noexcept { return _capacity; }

  size_t count(const K &key) { return _hashTable.count(key); }
};

class LRUCacheForHW1 {
private:
  using Q = std::string;
  using F = std::vector<float>;

  LRUCache<Q, F> _cache;

  size_t _bytes;

public:
  LRUCacheForHW1(const size_t &N, const size_t &bytes)
      : _cache(N), _bytes(bytes) {}
  ~LRUCacheForHW1() { clear(); }

  void insert(const Q &key, const F &values) {
    if (_cache.count(key) == 1) {
      std::vector<float> old_values = _cache.at(key);
      if (old_values.size() == _bytes) {
        old_values.clear();
        return;
      }
      size_t free_size = (_bytes - old_values.size());
      for (size_t i = 0, I = values.size(); ((i < I) and (i < free_size)); ++i)
        old_values.push_back(values[i]);
      _cache.insert(key, old_values);
      old_values.clear();
      return;
    }

    if (values.size() <= _bytes) {
      _cache.insert(key, values);
      return;
    }

    F buffer_values = values;
    buffer_values.resize(_bytes);
    _cache.insert(key, buffer_values);
    buffer_values.clear();
  }

  F at(const Q &key) { return _cache.at(key); }
  size_t size() { return _cache.size(); }
  size_t size_bytes() const { return _bytes; }
  size_t count(const Q &key) { return _cache.count(key); }
  void clear() { _cache.clear(); }
};

std::vector<std::string> split(const std::string &content, const char del) {
  std::vector<std::string> arrayString;
  std::string buffer = "";
  for (size_t i = 0, I = content.size(); i < I; ++i) {
    if (content[i] != del)
      buffer += content[i];
    else {
      arrayString.push_back(buffer);
      buffer = "";
    }
  }
  arrayString.push_back(buffer);
  buffer.clear();
  return arrayString;
}

std::ostream &operator<<(std::ostream &os, const std::vector<float> &values) {
  for (size_t i = 0, I = values.size(), I_ = (I - 1); i < I; ++i)
    os << values[i] << ((i != I_) ? " " : "");
  return os;
}

void loop_get(LRUCacheForHW1 &cache, const std::string &line,
              std::ostream &out) {
  if (cache.count(line)) {
    out << cache.at(line) << std::endl;
  } else {
    out << "!NOEMBED!" << std::endl;
  }
}

std::vector<float> strsToFloats(std::vector<std::string> &values_str) {
  std::vector<float> values(values_str.size());
  try {
    for (size_t i = 0, I = values_str.size(); i < I; ++i)
      values[i] = std::stof(values_str[i]);
  } catch (...) {
    values.clear();
  };

  values_str.clear();
  return values;
}

void loop_insert(LRUCacheForHW1 &cache, const std::string &line,
                 std::ostream &out) {
  std::vector<std::string> buffer = split(line, '\t');
  if ((buffer.empty() or (buffer.size() == 1))) {
    out << "!STORERR!" << std::endl;
    return;
  }

  std::string key = buffer[0];
  std::vector<std::string> values_str = split(buffer[1], ' ');
  buffer.clear();

  auto values = strsToFloats(values_str);
  if (values.empty()) {
    out << "!STORERR!" << std::endl;
    key.clear();
    return;
  };

  try {
    cache.insert(key, values);
    out << "!STORED!" << std::endl;
  } catch (...) {
    out << "!STORERR!" << std::endl;
  };
  values.clear();
  key.clear();
}

void loop(std::istream &in, std::ostream &out) {
  size_t N = 0, size = 0;
  in >> N >> size;
  LRUCacheForHW1 cache(N, size);

  std::string line;
  in.ignore();
  while (std::getline(in, line)) {
    if (line.empty())
      continue;

    if (line.find('\t') == std::string::npos)
      loop_get(cache, line, out);
    else
      loop_insert(cache, line, out);
  }
  line.clear();
  cache.clear();
}

#endif // CPP_COURSE_PROJECR_LRUCACHE_H_
