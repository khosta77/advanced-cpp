#include <iostream>
#include <list>
#include <map>
#include <string>
#include <cassert>
#include <exception>

template <typename T = std::string>
size_t hashStr( const T& key, const size_t& random_state, const size_t& maximum = 1e9 )
{
    size_t hash = 0, pow = 1;
    for ( auto element : key ) 
    {
        hash = ( hash + element * pow ) % maximum;
        pow = ( pow * random_state ) % maximum;
    }
    return hash;
}

template <typename T = std::string>
size_t getDoubleHash(const std::string& key )
{
    return ( ( hashStr( key, 41 ) << 32 ) | hashStr( key, 42 ) );
}

#define CELL_EMPTY  0x00
#define CELL_DELETE 0x01
#define CELL_KEY    0x02

template <typename T = std::string>
struct Cell
{
    T _item;
    size_t _hash;
    uint8_t _state;

    Cell() : _hash( 0 ), _state( CELL_EMPTY ) {}
    Cell( T item, const size_t& hash ) : _item( std::move( item ) ), _hash( hash ),
                                         _state( CELL_KEY ) {}
};


template <typename T = std::string>
class HashTable
{
    size_t _size;
    size_t ( *_hsh )( const T& );
    std::vector<Cell<T>> _table;

    void reBuild()
    {
        std::vector<Cell<T>> buffer( ( _table.size() * 2 ) ); 
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
    bool universalHashTableMethod( const T& item, const bool& isAdded, const bool& isDeleted )
    {
        // Так как у метода сложность O(1), то вызов его еще раз сделает тогда сложность O(2) -> O(1)
        if( isAdded && universalHashTableMethod( item, false, false ) )
            return false;

        if( isAdded && ( ( float )_size >= ( 0.75 * ( float )_table.size() ) ) )
            reBuild();

        const size_t hashValue = _hsh( item );
        size_t hash = ( hashValue % _table.size() ), cnt = 0;
        // удаленный элемент из ряда идущих элементов
        std::pair<bool, size_t> deletedItem = std::pair<bool, size_t>( true, 0 );

        while( ( _table[hash]._state != CELL_EMPTY ) && ( cnt < _table.size() ) )
        {
            if( ( _table[hash]._item == item ) && ( _table[hash]._state != CELL_DELETE) )
            {
                if( isDeleted )
                    _table[hash]._state = CELL_DELETE;
                return ( ( !isAdded ) ? true : false);
            }

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
        _table[hash] = Cell( std::move( item ), hashValue );
        ++_size;
        return true;
    }

public:
    HashTable( size_t ( *_hasher )( const T& ) = getDoubleHash ) : _hsh(_hasher), _size(0)
        { _table.resize(8); }

    ~HashTable() { _table.clear(); }

    bool find( const T& item ) { return universalHashTableMethod( item, false, false ); }

    bool add( const T& item ) { return universalHashTableMethod( item, true, false ); }    
    
    bool remove( const T& item ) { return universalHashTableMethod( item, false, true ); }

    void print()
    {
        std::cout << std::endl;
        for( auto cell : _table )
            std::cout << "h " << cell._hash << " e " << cell._item << " s " << ((int)cell._state) << std::endl;
    }

};

/** @class LRUCacheException - родительский класс ошибок для всего LRUcache, выводит сообщение с ошибкой
 * */
class LRUCacheException : public std::exception
{
public:
    explicit LRUCacheException( const std::string& msg ) : m_msg(msg) {}
    const char *what() const noexcept override
    {
        return m_msg.c_str();
    }
private:
    std::string m_msg;
};

/** @class LRUCCapacityEquallNull - класс ошибки, если capacity == 0
 * */
class LRUCCapacityEquallNull : public LRUCacheException
{
public:
    LRUCCapacityEquallNull() : LRUCacheException( "capacity is null" ) {}     
};

/** @class LRUCKeyNotFind класс ошибки если ключ не найден
 * */
class LRUCKeyNotFind : public LRUCacheException
{
public:
    LRUCKeyNotFind() : LRUCacheException( "key not find" ) {}
};

/** class LRUCache - Least Recently Used (LRU) Cache
 *                   Методы класса я старался по максимому и возможности взять из std::unordered_map
 * */
template<typename K, typename T>
class LRUCache
{
private:

    size_t _capacity;
    std::unordered_map<K, typename std::list<std::pair<K, T>>::iterator> _hashTable;
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
        if( _hashTable.find( key ) == _hashTable.end() )
            throw LRUCKeyNotFind();
    }
public:

    //// Member functions
    /** @brief Базовый конструктор
     * */
    LRUCache( const size_t& capacity = 0 ) : _capacity(capacity) {}
    
    /** @brief Конструктор копирования
     * */
    LRUCache( const LRUCache<K, T>& rhs ) : _capacity(rhs._capacity), _cacheList(rhs._cacheList),
        _hashTable(rhs._hashTable) {}

    /** @brief Конструктор перемещения
     * */
    LRUCache( LRUCache<K, T>&& rhs ) : _capacity(rhs._capacity), _cacheList(rhs._cacheList),
        _hashTable(rhs._hashTable) {}

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

        if( _hashTable.find( key ) != _hashTable.end() )
        {
            _hashTable[key]->second = value;
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
            _hashTable[key] = _cacheList.begin();
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

        _cacheList.erase(_hashTable[key]);
        _hashTable.erase(key);
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

struct TestObj {
    std::string _key;
    int _value;
    bool _put;
};

bool TEST_CACHE_PUTaGET( const int& capacity,  // Размер кэша
                         std::vector<TestObj>& testScript,  // Тестовый сценарий
                         [[maybe_unused]] const std::string& aboutT = "" )  // Имя теста
{
    LRUCache<std::string, int> cache(capacity);
    int value = 0;
    for( const TestObj& testObj : testScript )
    {
        if( testObj._put )
        {
            try
            {
                cache.insert( testObj._key, testObj._value );
            }
            catch( const LRUCCapacityEquallNull& capNull )
            {
                if( capacity == 0 )
                    continue;
                return false;
            }
            catch( const std::exception& emsg )
            {
                return false;
            };
        }
        else
        {
            try
            {
                value = cache.at( testObj._key );
            }
            catch( const LRUCCapacityEquallNull& capNull )  // Если размер изначальный равен 0
            {
                if( capacity == 0 )
                    continue;
                return false;
            }
            catch( const std::exception& emsg )
            {
                if( testObj._value != -1 )
                {
                    return false;
                }
                continue;
            };

            if( value != testObj._value )
            {
                return false;
            }
        }
    }
    return true;
}

std::vector<TestObj> BaseFromLeetCode = {
    { "1", 1, true },
    { "2", 2, true },
    { "1", 1, false },
    { "3", 3, true },
    { "2", -1, false },
    { "4", 4, true },
    { "1", -1, false },
    { "3", 3, false },
    { "4", 4, false }
};

std::vector<TestObj> DoubleKey = {
    { "A", 1, true },
    { "A", 1, false },
    { "A", 1, false },
    { "B", 2, true },
    { "A", 3, true },
    { "A", 3, false}
};

std::vector<TestObj> EmptyCache = {
    { "A", -1, false }
};

std::vector<TestObj> ManyDoubleElement = {
    { "A", 1, true },
    { "A", 1, true },
    { "A", 1, true },
    { "A", 1, true },
    { "A", 1, true },
    { "A", 1, true },
    { "A", 1, false }
};

int main( int argc, char* argv[] )
{
    assert( TEST_CACHE_PUTaGET( 2, BaseFromLeetCode ) );
    assert( TEST_CACHE_PUTaGET( 3, DoubleKey ) );
    assert( TEST_CACHE_PUTaGET( 2, EmptyCache ) );
    assert( TEST_CACHE_PUTaGET( 0, BaseFromLeetCode ) );
    assert( TEST_CACHE_PUTaGET( 2, ManyDoubleElement ) );
#if 0
    LRUCache<int, std::string> cache(3);
    cache.insert(1, "one");
    cache.insert(2, "two");
    cache.insert(3, "three");

    // Итерируемся по кэшу
    for (const auto& [key, value] : cache) {
        std::cout << key << ": " << value << std::endl;
        value = "X";
    }

    cache.insert(4, "four"); // Это удалит "one"
    
    std::cout << "After adding key 4:" << std::endl;
    std::cout << cache << std::endl;

    cache.erase(4);
    std::cout << "After del key 4:" << std::endl;
    std::cout << cache << std::endl;

#endif
    return 0;
}


