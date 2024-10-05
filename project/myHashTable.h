#ifndef CPP_COURSE_PROJECT_MYHASHTABLE_H_
#define CPP_COURSE_PROJECT_MYHASHTABLE_H_

#include <iostream>
#include <list>
#include <map>
#include <string>

#define CELL_EMPTY  0x00
#define CELL_DELETE 0x01
#define CELL_KEY    0x02

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
    HashTable() : _size(0)
    {
        _table.resize(8);
    }

    ~HashTable()
    {
        _table.clear();
    }

    bool find( const K& key )
    {
        return find_A_erase( key, false );
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
