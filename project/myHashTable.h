#ifndef CPP_COURSE_PROJECT_MYHASHTABLE_H_
#define CPP_COURSE_PROJECT_MYHASHTABLE_H_

#include <iostream>
#include <list>
#include <map>
#include <string>

#define CELL_EMPTY  0x00
#define CELL_DELETE 0x01
#define CELL_KEY    0x02

/*
 * Из ТЗ требование к map:
 * >> map должна быть шаблонной по ключу и значению. По желанию - возможность принять HashT, ComparatorT.
 * >> внутри мапы должны быть реализованы:
 *    - итераторы (обычный и const),           -
 *    - empty(),                               +
 *    - size(),                                +
 *    - clear(),                               +
 *    - reserve(),                             -
 *    - load_factor(),                         -
 *    - max_load_factor(),                     -
 *    - operator[](),                          +
 *    - find(),                                +
 *    - count(),                               -
 *    - insert(),                              +
 *    - erase() с семантикой, аналогичной STL. -
 * >> мапа должна верно реализовывать RAII
 * */

template <typename K, typename T, typename Hash = std::hash<K>>
class HashTable
{
    struct Cell
    {
        K _key;
        T _item;
        size_t _hash;
        uint8_t _state;

        Cell() : _hash( 0 ), _state( CELL_EMPTY ) {}
        Cell( const K& key, const T& item, const size_t& hash ) : _key(key), _item( item ),
            _hash( hash ), _state( CELL_KEY ) {}
    };

    size_t _size;
    size_t _capacity;
    Hash _hash;
    std::vector<Cell> _table;

    void reBuild()
    {
        std::vector<Cell> buffer( ( _table.size() * 2 ) ); 
        size_t hash = 0, cnt = 0;
        for( size_t i  = 0; i < _table.size(); ++i )
        {
            if( _table[i]._state == CELL_KEY )
            {
                hash = ( _table[i]._hash % buffer.size() );
                while( cnt = 0, buffer[hash]._state != CELL_EMPTY )
                    hash = ( ( hash + ++cnt ) % buffer.size() );
                buffer[hash] = _table[i];
            }
        }
        _table = std::move( buffer );
        buffer.clear();
    }

    /** @brief find_A_erase - концептуально метод схож с universalHashTableMethod, но просто осуществляет
     *                        поиск или удаление. 
     * */
    bool find_A_erase( const K& key, const bool& isDeleted )
    {
        const size_t hashValue = this->_hash( key );
        size_t hash = ( hashValue % _table.size() ), cnt = 0;

        while( ( _table[hash]._state != CELL_EMPTY ) && ( cnt < _table.size() ) )
        {
            if( ( _table[hash]._key == key ) && ( _table[hash]._state != CELL_DELETE) )
            {
                if( isDeleted )
                    _table[hash]._state = CELL_DELETE;
                return true;
            }

            hash = ( ( ++hash ) % _table.size() );
            ++cnt;
        }

        return false;
    }


    /** @brief universalHashTableMethod - операции добавления, удаления и поиска элемента +- одинаковые 
     *                                    по реализации. Решил что лучше будет объединить в один метод.
     *                                    Чтобы не было большого повторяющегося кода. появлется небольшое
     *                                    ветвление, но оно выполняется за О(1), так что им можно принебречь.
     *  @param item - вводимое значение
     *  @param isAdded - если мы хотим добавить элемент true
     *  @param isDeleted - если мы хотим удалить true
     *  +-------+---------+-----------+
     *  |isAdded|isDeleted| Результат |
     *  +-------+---------+-----------+
     *  | fasle | false   | Поиск     |
     *  +-------+---------+-----------+
     *  | true  | fasle   | Добавление|
     *  +-------+---------+-----------+
     *  | fasle | true    | Удаление  |
     *  +-------+---------+-----------+
     *  | true  | true    | Ошибка!!! |
     *  +-------+---------+-----------+
     * */
    bool universalHashTableMethod( const K& key, const T& item, const bool& isAdded, const bool& isDeleted )
    {
        // Так как у метода сложность O(1), то вызов его еще раз сделает тогда сложность O(2) -> O(1)
        if( isAdded && find_A_erase( key, false ) )
            return false;

        if( isAdded && ( ( float )_size >= ( 0.75 * ( float )_table.size() ) ) )
            reBuild();

        const size_t hashValue = this->_hash( key );
        size_t hash = ( hashValue % _table.size() ), cnt = 0;

        // удаленный элемент из ряда идущих элементов
        std::pair<bool, size_t> deletedItem = std::pair<bool, size_t>( true, 0 );

        while( ( _table[hash]._state != CELL_EMPTY ) && ( cnt < _table.size() ) )
        {
            if( ( _table[hash]._key == key ) && ( _table[hash]._state != CELL_DELETE) )
                return ( ( !isAdded ) ? true : false);

            if( isAdded && ( ( _table[hash]._state == CELL_DELETE ) && ( deletedItem.first ) ) )
            {
                deletedItem = std::pair<bool, size_t>( false, hash );
                break;  // Если бы попали на удаленную ячейку, сразу можно выйти
            }

            hash = ( ( ++hash ) % _table.size() );
            ++cnt;
        }

        if( !isAdded )  // Если мы не добавлеям новый элемент, то возвращаем false
            return false;

        hash = ( ( deletedItem.first ) ? hash : deletedItem.second );
        _table[hash] = Cell( key, item, hashValue );
        ++_size;
        return true;
    }

public:
    HashTable( const size_t& capacity ) : _size(0), _capacity(capacity), _table(capacity) {}

    ~HashTable()
    {
        _table.clear();
    }

    class iterator
    {
        using it = std::vector<std::pair<K, T>>::iterator;

        listIt _it;
    public:
        iterator( typename std::vector<Cell>::iterator it, const size_t& position )
        {
            for
        }

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

    bool find( const K& key )
    {
        const size_t hashValue = _hash( key );
        size_t hi = ( hashValue % _capacity ), cnt = 0;

        while( ( _table[hi]._state != CELL_EMPTY ) && ( cnt < _table.size() ) )
        {
            if( ( _table[hi]._key == key ) && ( _table[hi]._state != CELL_DELETE) )
            {
                return true;
            }

            hi = ( ( ++hi ) % _capacity );
            ++cnt;
        }

        return false;
    }

    bool insert( const K& key, const T& item )
    {
        return universalHashTableMethod( key, item, true, false );
    }    
    
    bool erase( const K& key )
    {
        return find_A_erase( key, true );
    }

    T& operator[]( const K& key )
    {
        const size_t hashValue = this->_hash( key );
        size_t hash = ( hashValue % _table.size() );
        return (T&)_table[hash]._item;
    }

    const bool empty() const noexcept
    {
        return ( _size == 0 );
    }

    const size_t size() const noexcept
    {
        return _size;
    }

    void clear()
    {
        _table.clear();
        _size = 0;
        _table.resize(8);
    }
};


#endif  // CPP_COURSE_PROJECT_MYHASHTABLE_H_
