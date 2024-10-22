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
 *    - итераторы (обычный и const),           +
 *    - empty(),                               +
 *    - size(),                                +
 *    - clear(),                               +
 *    - reserve(),                             -
 *    - load_factor(),                         +
 *    - max_load_factor(),                     +
 *    - operator[](),                          +
 *    - find(),                                +
 *    - count(),                               +
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

        Cell() : _hash(0), _state( CELL_EMPTY ) {}
        Cell( const K& key, const T& item, const size_t& hash ) : _key(key), _item( item ), _hash(hash),
            _state( CELL_KEY ) {}
    };

    size_t _size;

    /* Коэффициент заполнения хеш-таблицы (load factor) вычисляется как отношение числа хранимых элементов
     * к размеру массива (числу возможных значений хеш-функции). Обозначается как alpha = N/M,
     * где N — количество элементов в хеш-таблице, M — размер хеш-таблицы.
     * - Этот коэффициент является важным параметром, от которого зависит среднее время выполнения операций.
     * - Значение коэффициента говорит о степени заполненности таблицы: 2
     * Пока коэффициент меньше единицы, в массиве есть свободные элементы, куда теоретически ещё можно
     * добавить новые ключи.
     * - Если alpha = 1, то массив заполнен полностью.
     * - Если же alpha > 1, то число ключей превышает размер хэш-таблицы.
     * */
    float _factor;
    std::vector<Cell> _table;
    Hash _hash;

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

public:
    HashTable( const size_t& sizeTable = 4, const float& factor = 0.75 ) : _size(0), 
        _factor(factor), _table(sizeTable) {}

    ~HashTable()
    {
        clear();
    }

    class iterator
    {
    private:
        using It = std::vector<Cell>::iterator;

        It _it;
        It _end;
    public:
        iterator( It it, It end ) : _it(it), _end(end) {}

        bool operator!=(const iterator& other) const { return _it != other._it; }
        bool operator==(const iterator& other) const { return _it == other._it; }

        iterator& operator++()
        {
            do
            {
                ++_it;
            }
            while( ( ( (*_it)._state != CELL_KEY ) and ( _it != _end ) ) );
            return *this;
        }

        iterator operator++(int)
        {
            iterator buffer;
            do
            {
                buffer = this;
                ++_it;
            }
            while( ( ( (*_it)._state != CELL_KEY ) and ( _it != _end ) ) );
            return buffer;
        }

        iterator& operator--()
        {
            do
            {
                --_it;
            }
            while( ( ( (*_it)._state != CELL_KEY ) and ( _it != _end ) ) );
            return *this;
        }

        iterator operator--(int)
        {
            iterator buffer;
            do
            {
                buffer = this;
                --_it;
            }
            while( ( ( (*_it)._state != CELL_KEY ) and ( _it != _end ) ) );
            return buffer;
        }

        std::pair<K, T&> operator*()
        { 
            while( ( ( (*_it)._state != CELL_KEY ) and ( _it != _end ) ) ) { ++_it; }
            return { (*_it)._key, (T&)(*_it)._item };
        }
    };

    /** @brief begin - метод получения начала итератора
     * */
    iterator begin() noexcept { return iterator( _table.begin(), _table.end() ); }

    /** @brief end - метод получения конца итератора
     * */
    iterator end() noexcept { return iterator( _table.end(), _table.end() ); }

    class const_iterator
    {
    private:
        using const_It = std::vector<Cell>::iterator;

        const_It _it;
        const_It _end;
    public:
        const_iterator( const_It it, const_It end ) : _it(it), _end(end) {}

        bool operator!=(const const_iterator& other) const { return _it != other._it; }
        bool operator==(const const_iterator& other) const { return _it == other._it; }

        const_iterator& operator++()
        {
            do
            {
                ++_it;
            }
            while( ( ( (*_it)._state != CELL_KEY ) and ( _it != _end ) ) );
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator buffer;
            do
            {
                buffer = this;
                ++_it;
            }
            while( ( ( (*_it)._state != CELL_KEY ) and ( _it != _end ) ) );
            return buffer;
        }

        const_iterator& operator--()
        {
            do
            {
                --_it;
            }
            while( ( ( (*_it)._state != CELL_KEY ) and ( _it != _end ) ) );
            return *this;
        }

        const_iterator operator--(int)
        {
            const_iterator buffer;
            do
            {
                buffer = this;
                --_it;
            }
            while( ( ( (*_it)._state != CELL_KEY ) and ( _it != _end ) ) );
            return buffer;
        }

        const std::pair<K, T> operator*()
        {
            while( ( ( (*_it)._state != CELL_KEY ) and ( _it != _end ) ) ) { ++_it; }
            return { (*_it)._key, (*_it)._item };
        }
    };

    /** @brief cbegin - метод получения начала const итератора
     * */
    const_iterator cbegin() noexcept { return const_iterator( _table.cbegin(), _table.cend() ); }

    /** @brief end - метод получения конца const итератора
     * */
    const_iterator cend() noexcept { return const_iterator( _table.cend(), _table.cend() ); }

    std::pair<iterator, bool> find( const K& key )
    {
        const size_t hashValue = _hash( key );
        size_t hi = ( hashValue % _table.size() ), cnt = 0;

        while( ( _table[hi]._state != CELL_EMPTY ) && ( cnt < _table.size() ) )
        {
            if( ( _table[hi]._key == key ) && ( _table[hi]._state != CELL_DELETE) )
                return std::pair<iterator, bool>( iterator( ( _table.begin() + hi ), _table.end() ), false );

            hi = ( ( ++hi ) % _table.size() );
            ++cnt;
        }

        return std::pair<iterator, bool>( iterator( _table.end(), _table.end() ), false );
    }

    std::pair<iterator, bool> insert( const K& key, const T& item )
    {
        if( ( _factor <= this->load_factor() ) or ( 1.0f == this->load_factor() ) )
            reBuild();

        const size_t hashValue = this->_hash( key );
        size_t hash = ( hashValue % _table.size() ), cnt = 0;
 
        while( ( _table[hash]._state != CELL_EMPTY ) && ( cnt < _table.size() ) )
        {
            if( _table[hash]._state == CELL_DELETE )
                break;  // Если бы попали на удаленную ячейку, сразу можно выйти

            hash = ( ( ++hash ) % _table.size() );
            ++cnt;
        }

        _table[hash] = Cell( key, item, hashValue );
        ++_size;
        return std::pair<iterator, bool>( iterator( ( _table.begin() + hash ), _table.end() ), true ); 
    } 
    
    std::pair<iterator, bool> insert( const std::pair<K, T>& pair )
    {
        return this->insert( pair.first, pair.second );
    }

    size_t erase( const K& key )
    {
        const size_t hashValue = this->_hash( key );
        size_t hash = ( hashValue % _table.size() ), cnt = 0;

        while( ( _table[hash]._state != CELL_EMPTY ) && ( cnt < _table.size() ) )
        {
            if( ( _table[hash]._key == key ) && ( _table[hash]._state != CELL_DELETE) )
            {
                _table[hash]._state = CELL_DELETE;
                return 1;
            }

            hash = ( ( ++hash ) % _table.size() );
            ++cnt;
        }

        return 0;
    }

    T& operator[]( const K& key ) { return (T&) (*(this->find(key).first)).second; }

    const bool empty() const noexcept { return ( _size == 0 ); }
    const size_t size() const noexcept { return _size; }

    void clear()
    {
        _table.clear();
        _size = 0;
    }

    size_t count( const K& key )
    {
        const size_t hashValue = _hash( key );
        size_t hi = ( hashValue % _table.size() ), cnt = 0;

        while( ( _table[hi]._state != CELL_EMPTY ) && ( cnt < _table.size() ) )
        {
            if( ( _table[hi]._key == key ) && ( _table[hi]._state != CELL_DELETE) )
            {
                return 1;
            }

            hi = ( ( ++hi ) % _table.size() );
            ++cnt;
        }

        return 0;
    }

    float load_factor() const { return ( static_cast<float>(_size) / static_cast<float>( _table.size() ) ); }
    float max_load_factor() const { return std::max( _factor, this->load_factor() ); }
    void max_load_factor( float ml ) { _factor = ml; }
};


#endif  // CPP_COURSE_PROJECT_MYHASHTABLE_H_
