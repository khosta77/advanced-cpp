#ifndef CPP_COURSE_PROJECR_LRUCACHE_H_
#define CPP_COURSE_PROJECR_LRUCACHE_H_

#include <iostream>
#include <list>
#include <map>

#include "exception.h"
#include "HashTable.h"
#include "List.h"

/** class LRUCache - Least Recently Used (LRU) Cache
 *                   Методы класса я старался по максимому и возможности взять из std::unordered_map
 * */
template<typename K, typename T>
class LRUCache
{
private:

    size_t _capacity;

    using LRU_pair = std::pair<K, T>;
    using LRU_list = std::list<LRU_pair>;
    using LRU_list_iterator = typename LRU_list::iterator;
    using LRU_table = HashTable<K, LRU_list_iterator>;
    LRU_table _hashTable;
    LRU_list _cacheList;

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
    ~LRUCache() { clear(); }

    /** @brief перегрузка оператора побитового сдвига для выводы std::ostream
     * */
    friend std::ostream& operator<<( std::ostream& os, const LRUCache& rhs )
    {
        for( const auto& [ key, value ] : rhs._cacheList )
            os << key << ": " << value << "\n";
        return os;
    }

    //// Capacity
    /** @brief empty - проверка не пуста ли наша таблица
     * */
    const bool empty() const noexcept { return _hashTable.empty(); }
    
    /** @brief size - размер заполненого хеша
     * */
    const size_t size() const noexcept { return _hashTable.size(); }

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
    void insert( const LRU_pair& pr ) { insert( pr.first, pr.second ); }

    /** @brief insert - вставка ключ+значение
     * */
    void insert( const K& key, const T& value )
    {
        checkCapacityIsZero();
        if( _hashTable.count( key ) )
        {
            LRU_list buffer;
            buffer.push_front( LRU_pair( key, value ) );
            _hashTable.erase( key );
            _hashTable.insert( key, buffer.begin() );
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
            _cacheList.push_front( LRU_pair( key, value ) );
            _hashTable.insert( key, _cacheList.begin() );
        }
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
        return (T&)(*_hashTable[key]).second;
    }

    //// other
    /** @brief recapacity - изменить размер _capacity, очистить все данные что были до, мало ли
     * */
    void recapacity( const size_t& newCapacity ) noexcept
    {
        clear();
        _capacity = newCapacity;
    }

    /** @brief getCapacity - получить _capacity
     * */
    const size_t getCapacity() const noexcept { return _capacity; }
};

#endif  // CPP_COURSE_PROJECR_LRUCACHE_H_
