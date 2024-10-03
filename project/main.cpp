#include <iostream>
#include <list>
#include <map>
#include <string>
#include <cassert>
#include <exception>

/** @brief Ошибка, с выводом сообщения
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

template<typename K, typename T>
class LRUCache
{
private:

    int _capacity;
    std::unordered_map<K, typename std::list<std::pair<K, T>>::iterator> mp;
    std::list<std::pair<K, T>> lst;

public:
    LRUCache( int capacity ) : _capacity(capacity) {}

    T get( K key )
    {
        if( mp.find(key) == mp.end() )
            throw LRUCacheException("Ключ не найден");
    
        lst.splice( lst.begin(), lst, mp[key] );
        return mp[key]->second;
    }
    
    void put( K key, T value )
    {
        if( mp.find(key) != mp.end() )
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
        std::cout << testObj._key << ' ' << testObj._value << std::endl;
        if( testObj._put )
        {
            try
            {
                cache.put( testObj._key, testObj._value );
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
                value = cache.get( testObj._key );
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

int main( int argc, char* argv[] )
{
    assert( TEST_CACHE_PUTaGET( 2, BaseFromLeetCode ) );
    return 0;
}


