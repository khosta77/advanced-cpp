#ifndef CPP_COURSE_PROJECT_LIST_H_
#define CPP_COURSE_PROJECT_LIST_H_

#include <iostream>

/*
 * Из ТЗ требование к list:
 * >> список, очевидно, будет шаблонным. Можно вдохновляться
 *    https://www.boost.org/doc/libs/1_67_0/doc/html/intrusive/list.html ,
 *    который "using the public member hook...".
 * >> внутри списка должны быть:
 *    - реализованы итераторы (обычный и const), +
 *    - empty(),                                 +
 *    - size(),                                  +
 *    - clear(),                                 +
 *    - front(),                                 +
 *    - back(),                                  +
 *    - push/pop_front(),                        +
 *    - push/pop_back(),                         +
 *    - insert(),                                +
 *    - splice() с семантикой, аналогичной STL.  -
 * */
template <typename T> class List {
public:
  class iterator;
  class const_iterator;

private:
  size_t _size;

  struct Node {
    T _value;
    Node *_next;
    Node *_prev;

    Node(const T &value, Node *next, Node *prev)
        : _value(value), _next(next), _prev(prev) {}
  };

  Node *_head;
  Node *_tail;

  friend class iterator;
  friend class const_iterator;

public:
  //// Member functions
  List() : _size(0), _head(nullptr), _tail(nullptr) {}

  ~List() { clear(); }

  //// Element access
  T &front() { return (T &)_head->_value; }

  T &back() { return (T &)_tail->_value; }

  //// Iterators
  class iterator {
  private:
    using It = Node *;

    It _it;
    bool _isEnd;

    friend class List;

  public:
    iterator(Node *node = nullptr, const bool &isEnd = false)
        : _it(node), _isEnd(isEnd) {}

    bool operator!=(const iterator &rhs) const {
      if ((_isEnd and (rhs._isEnd == _isEnd)))
        return false;

      int x = (int(_it->_value != rhs._it->_value) +
               int(_it->_prev != rhs._it->_prev) +
               int(_it->_next != rhs._it->_next));

      if (((x == 0) or ((x == 3) and (_isEnd != rhs._isEnd))))
        return true;
      return false;
    }

    bool operator==(const iterator &rhs) const {
      if ((_isEnd and (rhs._isEnd == _isEnd)))
        return false;

      int x = (int(_it->_value == rhs._it->_value) +
               int(_it->_prev == rhs._it->_prev) +
               int(_it->_next == rhs._it->_next));

      if (((x == 0) or ((x == 3) and (_isEnd == rhs._isEnd))))
        return true;
      return false;
    }

    iterator &operator++() {
      if ((_isEnd and (_it->_next != nullptr))) {
        _isEnd = false;
        return *this;
      }

      if (_it->_next == nullptr)
        _isEnd = true;
      else
        _it = _it->_next;

      return *this;
    }

    iterator operator++(int) {
      iterator buffer(_it, _isEnd);

      if ((_isEnd and (_it->_next != nullptr))) {
        _isEnd = false;
        return *this;
      }

      if (_it->_next == nullptr)
        _isEnd = true;
      else
        _it = _it->_next;

      return buffer;
    }

    iterator &operator--() {
      if ((_isEnd and (_it->_prev != nullptr))) {
        _isEnd = false;
        return *this;
      }

      if (_it->_prev == nullptr)
        _isEnd = true;
      else
        _it = _it->_prev;

      return *this;
    }

    iterator operator--(int) {
      iterator buffer(_it, _isEnd);

      if ((_isEnd and (_it->_prev != nullptr))) {
        _isEnd = false;
        return *this;
      }

      if (_it->_prev == nullptr)
        _isEnd = true;
      else
        _it = _it->_prev;

      return buffer;
    }

    T &operator*() { return (T &)_it->_value; }
  };

  class const_iterator {
  private:
    using It = Node *;

    It _it;
    bool _isEnd;

    friend class List;

  public:
    const_iterator(Node *node = nullptr, const bool &isEnd = false)
        : _it(node), _isEnd(isEnd) {}

    bool operator!=(const const_iterator &rhs) const {
      if ((_isEnd and (rhs._isEnd == _isEnd)))
        return false;

      int x = (int(_it->_value != rhs._it->_value) +
               int(_it->_prev != rhs._it->_prev) +
               int(_it->_next != rhs._it->_next));

      if (((x == 0) or ((x == 3) and (_isEnd != rhs._isEnd))))
        return true;
      return false;
    }

    bool operator==(const const_iterator &rhs) const {
      if ((_isEnd and (rhs._isEnd == _isEnd)))
        return false;

      int x = (int(_it->_value == rhs._it->_value) +
               int(_it->_prev == rhs._it->_prev) +
               int(_it->_next == rhs._it->_next));

      if (((x == 0) or ((x == 3) and (_isEnd == rhs._isEnd))))
        return true;
      return false;
    }

    const_iterator &operator++() {
      if ((_isEnd and (_it->_next != nullptr))) {
        _isEnd = false;
        return *this;
      }

      if (_it->_next == nullptr)
        _isEnd = true;
      else
        _it = _it->_next;

      return *this;
    }

    const_iterator operator++(int) {
      const_iterator buffer(_it, _isEnd);

      if ((_isEnd and (_it->_next != nullptr))) {
        _isEnd = false;
        return *this;
      }

      if (_it->_next == nullptr)
        _isEnd = true;
      else
        _it = _it->_next;

      return buffer;
    }

    const_iterator &operator--() {
      if ((_isEnd and (_it->_prev != nullptr))) {
        _isEnd = false;
        return *this;
      }

      if (_it->_prev == nullptr)
        _isEnd = true;
      else
        _it = _it->_prev;

      return *this;
    }

    const_iterator operator--(int) {
      const_iterator buffer(_it, _isEnd);

      if ((_isEnd and (_it->_prev != nullptr))) {
        _isEnd = false;
        return *this;
      }

      if (_it->_prev == nullptr)
        _isEnd = true;
      else
        _it = _it->_prev;

      return buffer;
    }

    const T operator*() const { return _it->_value; }
  };

  /** @brief begin - метод получения начала итератора
   * */
  iterator begin() { return iterator(_head); }
  const_iterator begin() const { return const_iterator(_head); }
  const_iterator cbegin() const noexcept { return const_iterator(_head); }

  /** @brief end - метод получения конца итератора
   * */
  iterator end() { return iterator(_tail, true); }
  const_iterator end() const { return const_iterator(_tail, true); }
  const_iterator cend() const noexcept { return const_iterator(_tail, true); }

  //// Capacity
  bool empty() const { return ((_head == nullptr) and (_tail == nullptr)); }

  size_t size() const { return _size; }

  //// Modifiers
  void clear() {
    if ((_head == nullptr) and (_tail == nullptr))
      return;

    Node *node = _head->_next;
    while (node) {
      delete _head;
      _head = node;
      node = node->_next;
    }
    delete _head;
    _head = nullptr;
    _tail = nullptr;
    _size = 0;
  }

  iterator insert(iterator pos, const T &value) {
    if (pos._it == nullptr) {
      this->push_front(value);
      return this->begin();
    }

    Node *node = new Node(value, nullptr, nullptr);

    if (pos._it == _head) {
      node->_next = _head;
      _head->_prev = node;
      _head = node;
    } else {
      node->_prev = pos._it->_prev;
      node->_next = pos._it;

      if (pos._it->_prev)
        pos._it->_prev->_next = node;

      pos._it->_prev = node;
    }

    return iterator(node);
  }

private:
  void push_first_element(const T &value) {
    Node *node = new Node(value, nullptr, nullptr);
    _head = _tail = node;
    ++_size;
  }

public:
  void push_front(const T &value) {
    if (_size == 0)
      return push_first_element(value);

    Node *node = new Node(value, _head, nullptr);
    _head->_prev = node;
    _head = node;
    ++_size;
  }

  void push_back(const T &value) {
    if (_size == 0)
      return push_first_element(value);

    Node *node = new Node(value, nullptr, _tail);
    _tail->_next = node;
    _tail = node;
    ++_size;
  }

private:
  void pop_last_element() {
    Node *node = _head;
    _head = nullptr;
    _tail = nullptr;
    delete node;
    --_size;
  }

public:
  void pop_front() {
    if (_size == 1)
      return pop_last_element();

    Node *node = _head;
    _head = _head->_next;
    _head->_prev = nullptr;
    --_size;
    delete node;
  }

  void pop_back() {
    if (_size == 1)
      return pop_last_element();

    Node *node = _tail;
    _tail = _tail->_prev;
    _tail->_next = nullptr;
    --_size;
    delete node;
  }

  //// Operations
  void splice([[maybe_unused]] iterator pos, [[maybe_unused]] List &other,
              [[maybe_unused]] iterator it) {}
};

#endif // CPP_COURSE_PROJECT_LIST_H_
