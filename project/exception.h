#ifndef CPP_COURSE_PROJECT_EXCEPTION_H_
#define CPP_COURSE_PROJECT_EXCEPTION_H_

#include <string>
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

#endif  // CPP_COURSE_PROJECT_EXCEPTION_H_
