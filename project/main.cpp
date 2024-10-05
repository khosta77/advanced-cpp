#include <iostream>
#include <list>
#include <map>
#include <string>
#include <cassert>
#include <exception>

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
    std::unordered_map<K, typename std::list<std::pair<K, T>>::iterator> mp;
    std::list<std::pair<K, T>> lst;

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
        if( mp.find( key ) == mp.end() )
            throw LRUCKeyNotFind();
    }
public:

    //// Member functions
    /** @brief Базовый конструктор
     * */
    LRUCache( const size_t& capacity = 0 ) : _capacity(capacity) {}
    
    /** @brief Конструктор копирования
     * */
    LRUCache( const LRUCache<K, T>& rhs ) : _capacity(rhs._capacity), lst(rhs.lst), mp(rhs.mp) {}

    /** @brief Конструктор перемещения
     * */
    LRUCache( LRUCache<K, T>&& rhs ) : _capacity(rhs._capacity), lst(rhs.lst), mp(rhs.mp) {}

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
        for( const auto& [ key, value ] : rhs.lst )
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
        return iterator( lst.begin() );
    }

    /** @brief end - метод получения конца итератора
     * */
    iterator end() noexcept
    {
        return iterator( lst.end() );
    }

    // cbegin, cend не стал реализовывать, показалось лишним

    //// Capacity
    /** @brief empty - проверка не пуста ли наша таблица
     * */
    const bool empty() const noexcept
    {
        return mp.empty();
    }
    
    /** @brief size - размер заполненого хеша
     * */
    const size_t size() const noexcept
    {
        return mp.size();
    }

    //// Modifiers
    /** @brief clear - очистить все
     * */
    void clear() noexcept
    {
        lst.clear();
        mp.clear();
    }

    /** @brief insert - вставка пары
     * */
    void insert( const std::pair<K, T>& pr )
    {
        checkCapacityIsZero();
        
        if( mp.find( pr.first ) != mp.end() )
        {
            mp[pr.first]->second = pr.second;
            lst.splice( lst.begin(), lst, mp[pr.first] );
        }
        else
        {
            if( lst.size() == _capacity )
            {
                K lastKey = lst.back().first;
                mp.erase( lastKey );
                lst.pop_back();
            }
            lst.emplace_front( pr );
            mp[pr.fisrt] = lst.begin();
        }
    }

    /** @brief insert - вставка ключ+значение
     * */
    void insert( const K& key, const T& value )
    {
        checkCapacityIsZero();

        if( mp.find( key ) != mp.end() )
        {
            mp[key]->second = value;
            lst.splice( lst.begin(), lst, mp[key] );
        }
        else
        {
            if( lst.size() == _capacity )
            {
                K lastKey = lst.back().first;
                mp.erase(lastKey);
                lst.pop_back();
            }
            lst.emplace_front( key, value );
            mp[key] = lst.begin();
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

        lst.erase(*it);
        mp.erase(*it.first);
    }

    /** @brief erase - удаление элемента по ключу
     * */
    void erase( const K& key )
    {
        checkCapacityIsZero();
        checkKeyIsFinding( key );

        lst.erase(mp[key]);
        mp.erase(key);
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
        lst.splice( lst.begin(), lst, mp[key] );
        return (T&)mp[key]->second;
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


