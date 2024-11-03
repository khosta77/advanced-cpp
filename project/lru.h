#ifndef CPP_COURSE_PROJECR_LRUCACHE_H_
#define CPP_COURSE_PROJECR_LRUCACHE_H_

#include <iostream>
#include <list>
#include <map>

#include "exception.h"
#include "HashTable.h"

/** class LRUCache - Least Recently Used (LRU) Cache
 *                   Методы класса я старался по максимому и возможности взять из std::unordered_map
 * */
template<typename K, typename T>
class LRUCache
{
private:

    size_t _capacity;

    HashTable<K, typename std::list<std::pair<K, T>>::iterator> _hashTable;
    std::list<std::pair<K, T>> _cacheList;

    /** @brief checkCapacityIsZero - Часто надо проверять _capacity, обернул в inline функцию
     * */
    inline void checkCapacityIsZero()
    {
        if( _capacity == 0 )
            throw LRUCCapacityEquallNull();
    }

    /** @brief checkKeyIsFinding - Часто надо проверять ключ, обернул в inline функцию
     * */
    inline void checkKeyIsFinding( const K& key )
    {
        if( !_hashTable.count(key) )
            throw LRUCKeyNotFind();
    }
public:

    //// Member functions
    /** @brief Базовый конструктор
     * */
    LRUCache( const size_t& capacity = 0 ) : _capacity(capacity), _hashTable(capacity) {}
    
    /** @brief Конструктор копирования
     * */
    //LRUCache( const LRUCache<K, T>& rhs ) : _capacity(rhs._capacity), _cacheList(rhs._cacheList),
    //    _hashTable(rhs._hashTable) {}

    /** @brief Конструктор перемещения
     * */
    //LRUCache( LRUCache<K, T>&& rhs ) : _capacity(rhs._capacity), _cacheList(rhs._cacheList),
    //    _hashTable(rhs._hashTable) {}

    /** @brief Деструктор
     * */
    ~LRUCache()
    {
        clear();
    }

    /** @brief перегрузка оператора побитового сдвига для выводы std::ostream
     * */
    friend std::ostream& operator<<( std::ostream& os, const LRUCache& rhs )
    {
        for( const auto& [ key, value ] : rhs._cacheList )
            os << key << ": " << value << "\n";
        return os;
    }


    //// Iterators

    /** @class iterator - итератор
     * */
    class iterator
    {
        using listIt = std::list<std::pair<K, T>>::iterator;

        listIt _it;
    public:
        iterator( typename std::list<std::pair<K, T>>::iterator it ) : _it(it) {}

        bool operator!=(const iterator& other) const
        {
            return _it != other._it;
        }

        bool operator==(const iterator& other) const
        {
            return _it == other._it;
        }

        iterator& operator++()
        {
            ++_it;
            return *this;
        }

        iterator operator++(int)
        {
            iterator buffer = this;
            ++_it;
            return buffer;
        }

        std::pair<K, T&> operator*()
        {
            return { _it->first, (T&)_it->second };
        }
    };

    /** @brief begin - метод получения начала итератора
     * */
    iterator begin() noexcept
    {
        return iterator( _cacheList.begin() );
    }

    /** @brief end - метод получения конца итератора
     * */
    iterator end() noexcept
    {
        return iterator( _cacheList.end() );
    }

    // cbegin, cend не стал реализовывать, показалось лишним

    //// Capacity
    /** @brief empty - проверка не пуста ли наша таблица
     * */
    const bool empty() const noexcept
    {
        return _hashTable.empty();
    }
    
    /** @brief size - размер заполненого хеша
     * */
    const size_t size() const noexcept
    {
        return _hashTable.size();
    }

    //// Modifiers
    /** @brief clear - очистить все
     * */
    void clear() noexcept
    {
        _cacheList.clear();
        _hashTable.clear();
    }

    /** @brief insert - вставка пары
     * */
    void insert( const std::pair<K, T>& pr )
    {
        insert( pr.first, pr.second );
    }

    /** @brief insert - вставка ключ+значение
     * */
    void insert( const K& key, const T& value )
    {
        checkCapacityIsZero();
        if( _hashTable.count( key ) )
        {
            auto it = std::list<std::pair<K, T>>{std::pair<K, T>( key, value )};
            _hashTable.erase( key );
            _hashTable.insert( key, it.begin() );
            _cacheList.splice( _cacheList.begin(), _cacheList, _hashTable[key] );
        }
        else
        {
            if( _cacheList.size() == _capacity )
            {
                K lastKey = _cacheList.back().first;
                _hashTable.erase(lastKey);
                _cacheList.pop_back();
            }
            _cacheList.emplace_front( key, value );
            _hashTable.insert( key, _cacheList.begin() );
        }
    }

    /*
     * emplace не реализовал, по идеи в данной структуре его не должно быть,
     * так как он всегда бы возвращал true и от insert ни чем не отличался бы
     * */

    /** @brief erase - удаление по итератору
     * */
    void erase( const iterator& it )
    {
        checkCapacityIsZero();

        _cacheList.erase(*it);
        _hashTable.erase(*it.first);
    }

    /** @brief erase - удаление элемента по ключу
     * */
    void erase( const K& key )
    {
        checkCapacityIsZero();
        checkKeyIsFinding( key );

        _cacheList.erase( _hashTable[key] );
        _hashTable.erase( key );
    }

    //// Lookup
    /** @brief at - взятие элемета с тотальной проверкой, есть возможность после его редактировать
     * */
    T& at( const K& key )
    {
        checkCapacityIsZero();
        checkKeyIsFinding( key );

        return this->operator[](key);
    }

    /** @brief at - взятие элемета с тотальной проверкой
     * */
    const T& at( const K& key ) const
    {
        checkCapacityIsZero();
        checkKeyIsFinding( key );

        return this->operator[](key);
    }

    /** @brief operator[] - взятие элемета, есть возможность после его редактировать
     * */
    T& operator[]( const K& key )
    {
        _cacheList.splice( _cacheList.begin(), _cacheList, _hashTable[key] );
        return (T&)_hashTable[key]->second;
    }

    //// other
    /** @brief recapacity - изменить размер _capacity, очистить все данные что были до, мало ли
     *                      TODO: исправить, чтобы данные не удалялись
     * */
    void recapacity( const size_t& newCapacity ) noexcept
    {
        clear();
        _capacity = newCapacity;
    }

    /** @brief getCapacity - получить _capacity
     * */
    const size_t getCapacity() const noexcept
    {
        return _capacity;
    }
    
};


#endif  // CPP_COURSE_PROJECR_LRUCACHE_H_
