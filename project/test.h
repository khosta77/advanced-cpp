#ifndef CPP_COURSE_TEST_H_
#define CPP_COURSE_TEST_H_

#include "lru.h"
#include "HashTable.h"
#include "List.h"

#include <unordered_map>
#include <list>
#include <cassert>

class TestException : std::exception 
{
private:
    std::string emsg;

public:
    explicit TestException( const std::string& msg ) : emsg(msg) {}
    const char *what() const noexcept override { return emsg.c_str(); }
};

namespace ListTestSpace
{
    class ListEmpty : public TestException
    {
    public:
        ListEmpty() : TestException("Список пустой!!!") {}
    };

    class ListNotEmpty : public TestException
    {
    public:
        ListNotEmpty() : TestException("Список не пустой!!!") {}
    };

    class ListSizeNotExpected : public TestException
    {
    public:
        ListSizeNotExpected( const size_t& size, const size_t& expected ) : TestException(
                ( "list.size() = " + std::to_string(size) + " not excepted: " + std::to_string(expected) )
            ) {}
    };

    class ListFrontNotLink : public TestException
    {
    public:
        ListFrontNotLink() : TestException("C front что-то не так!!!") {}
    };

    class ListBackNotLink : public TestException
    {
    public:
        ListBackNotLink() : TestException("C front что-то не так!!!") {}
    };

    class ListNotExceptedElement : public TestException
    {
    public:
        ListNotExceptedElement( const std::string& felem, const std::string& efelem,
                                const std::string& belem, const std::string& ebelem ) : TestException(
                ( "front( " + felem + " ) -> " + efelem + " | back( " + belem + " ) -> " + ebelem )
            ) {}
    };

    class ListNotCurrentValue : public TestException
    {
    public:
        ListNotCurrentValue( const int& a, const int& b ) : TestException(
                ( "Ожидаемые значения не равны!: " + std::to_string(a) + " != " + std::to_string(b) )
            ) {}
    };
    
    namespace check
    {
        std::vector<int> frame = { -3, -2, -1, 0, 1, 2, 3 };
        const size_t MIDDLE = 3;

        template<typename T>
        void front_and_back()
        {
            T lst;
            // Проверка вообще есть ли элементы уже
            if( !lst.empty() )
                throw ListNotEmpty();

            // Проверка размера
            if( lst.size() != 0 )
                throw ListSizeNotExpected( lst.size(), 0 );

            // Добавляем первый элемент
            lst.push_back(frame[MIDDLE]);
            if( ( ( lst.front() != frame[MIDDLE] ) or ( lst.back() != frame[MIDDLE] ) ) )
            {
                throw ListNotExceptedElement(
                        std::to_string( lst.front() ), std::to_string( frame[MIDDLE] ),
                        std::to_string( lst.back() ), std::to_string( frame[MIDDLE] )
                    );
            }

            // Если список все еще пустой -> ошибка
            if( lst.empty() )
                throw ListEmpty();

            // Размер должен быть 1
            if( lst.size() != 1 )
                throw ListSizeNotExpected( lst.size(), 1 );

            // Проверяем ссылочность front
            lst.front() = 1;
            if( ( ( lst.front() != 1 ) or ( lst.back() != 1 ) ) )
                throw ListFrontNotLink();

            // Проверяем ссылочность back
            lst.back() = -1;
            if( ( ( lst.front() != -1 ) or ( lst.back() != -1 ) ) )
                throw ListBackNotLink();
            lst.back() = frame[MIDDLE];

            // Проверяем заполнение вектора
            for( size_t i = ( MIDDLE - 1 ), j = ( MIDDLE + 1 ), count = 1; j < frame.size(); --i, ++j )
            {
                lst.push_front(frame[i]);
                lst.push_back(frame[j]);
                count += 2;

                // Проверка корректных значений
                if( ( ( lst.front() != frame[i] ) or ( lst.back() != frame[j] ) ) )
                    throw ListNotExceptedElement( std::to_string( lst.front() ), std::to_string( frame[i] ),
                                                  std::to_string( lst.back() ), std::to_string( frame[j] ) );

                // Проверка размера
                if( lst.size() != count )
                    throw ListSizeNotExpected( lst.size(), count );
            }

            // удаление
            for( size_t i = 1, j = ( lst.size() - 2 ), count = lst.size(); i <= j; ++i, --j )
            {
                lst.pop_front();
                lst.pop_back();
                count -= 2;

                // Проверка корректных значений
                if( ( ( lst.front() != frame[i] ) or ( lst.back() != frame[j] ) ) )
                    throw ListNotExceptedElement( std::to_string( lst.front() ), std::to_string( frame[i] ),
                                                  std::to_string( lst.back() ), std::to_string( frame[j] ) );

                // Проверка размера
                if( lst.size() != count )
                    throw ListSizeNotExpected( lst.size(), count );
            }

            lst.pop_front();
            if( lst.size() != 0 )
                throw ListSizeNotExpected( lst.size(), 0 );

            // Проверка очистки списка
            for( size_t i = ( MIDDLE - 1 ), j = ( MIDDLE + 1 ), count = 0; j < frame.size(); --i, ++j )
            {
                lst.push_front(frame[i]);
                lst.push_back(frame[j]);
            }

            if( lst.size() != ( frame.size() - 1 ) )
                throw ListSizeNotExpected( lst.size(), ( frame.size() - 1 ) );

            lst.clear();

            // Проверка вообще есть ли элементы уже
            if( !lst.empty() )
                throw ListNotEmpty();

            // Проверка размера
            if( lst.size() != 0 )
                throw ListSizeNotExpected( lst.size(), 0 );
        }

        template<typename T>
        void inter()
        {
            T lst;
            lst.push_back(frame[MIDDLE]);
            for( size_t i = ( MIDDLE - 1 ), j = ( MIDDLE + 1 ); j < frame.size(); --i, ++j )
            {
                lst.push_front(frame[i]);
                lst.push_back(frame[j]);
            }

            //// Тест обычного итератора ++, -- ...
            auto vectorIt = frame.begin();
            auto listIt = lst.begin();
            for( size_t i = 0, I = frame.size(); i < I; ++i, ++listIt, ++vectorIt )
            {
                if( *listIt != *vectorIt )
                    throw ListNotCurrentValue( *listIt, *vectorIt );
            }

            vectorIt = frame.begin();
            listIt = lst.begin();
            for( size_t i = 0, I = frame.size(); i < I; ++i, listIt++, vectorIt++ )
            {
                if( *listIt != *vectorIt )
                    throw ListNotCurrentValue( *listIt, *vectorIt );
            }

            auto vectorItEnd = frame.end();
            auto listItEnd = lst.end();
            for( size_t i = 0, I = frame.size(); i < I; ++i )
            {
                --listItEnd;
                --vectorItEnd;
                if( *listItEnd != *vectorItEnd )
                    throw ListNotCurrentValue( *listItEnd, *vectorItEnd );
            }

            vectorItEnd = frame.end();
            listItEnd = lst.end();
            for( size_t i = 0, I = frame.size(); i < I; ++i )
            {
                listItEnd--;
                vectorItEnd--;
                if( *listItEnd != *vectorItEnd )
                    throw ListNotCurrentValue( *listItEnd, *vectorItEnd );
            }

            //// Тест обычного const итератора ++, -- ...
            auto vectorItConst = frame.cbegin();
            auto listItConst = lst.cbegin();
            for( size_t i = 0, I = frame.size(); i < I; ++i, ++listItConst, ++vectorItConst )
            {
                if( *listItConst != *vectorItConst )
                    throw ListNotCurrentValue( *listItConst, *vectorItConst );
            }

            vectorItConst = frame.cbegin();
            listItConst = lst.cbegin();
            for( size_t i = 0, I = frame.size(); i < I; ++i, listItConst++, vectorItConst++ )
            {
                if( *listItConst != *vectorItConst )
                    throw ListNotCurrentValue( *listItConst, *vectorItConst );
            }

            auto vectorItEndConst = frame.cend();
            auto listItEndConst = lst.cend();
            for( size_t i = 0, I = frame.size(); i < I; ++i )
            {
                --listItEndConst;
                --vectorItEndConst;
                if( *listItEndConst != *vectorItEndConst )
                    throw ListNotCurrentValue( *listItEndConst, *vectorItEndConst );
            }

            vectorItEndConst = frame.cend();
            listItEndConst = lst.cend();
            for( size_t i = 0, I = frame.size(); i < I; ++i )
            {
                listItEndConst--;
                vectorItEndConst--;
                if( *listItEndConst != *vectorItEndConst )
                    throw ListNotCurrentValue( *listItEndConst, *vectorItEndConst );
            }

            auto it = lst.begin();
            auto revers = --(frame.cend());
            for( const auto END = lst.end(); it != END; ++it, --revers )
            {
                *it = *revers;
            }
            --it;
            for( const auto value : frame )
            {
                int x = *(it--);
                if( value != x )
                    throw ListNotCurrentValue( value, x );
            }
        }

        template<typename T>
        void insert()
        {
            T lst;
            auto it = lst.begin();
            for( size_t i = 0; i < frame.size(); ++i )
            {
                it = lst.insert( it, frame[i] );
                if( *it != frame[i] )
                    throw ListNotCurrentValue( *it, frame[i] );
            }
        }
 
        void splice()
        {
            List<int> lst1;
            List<int> lst2;
            lst2.push_back(10);
            lst2.push_back(20);
            lst2.push_back(30);

            lst1.push_back(frame[MIDDLE]);
            for( size_t i = ( MIDDLE - 1 ), j = ( MIDDLE + 1 ); j < frame.size(); --i, ++j )
            {
                lst1.push_front(frame[i]);
                lst1.push_back(frame[j]);
            }

            auto it = ++(lst1.begin());
            lst1.splice( it, lst2, lst2.begin() );

            std::list<int> list1{ -3, -2, -1, 0, 1, 2, 3};
            std::list<int> list2{ 10, 20, 30 };

            auto it_ = ++(list1.begin());
            list1.splice( it_, list2, list2.begin() );

            auto stdIt1 = list1.begin();
            auto myIt1 = lst1.begin();
            for( size_t i = 0; i < lst1.size(); ++i, ++stdIt1, ++myIt1 )
            {
                if( *stdIt1 != *myIt1 )
                    throw ListNotCurrentValue( *stdIt1, *myIt1 );
            }

            auto stdIt2 = list2.begin();
            auto myIt2 = lst2.begin();
            for( size_t i = 0; i < lst2.size(); ++i, ++stdIt2, ++myIt2 )
            {
                if( *stdIt2 != *myIt2 )
                    throw ListNotCurrentValue( *stdIt2, *myIt2 );
            }
        }

    };  // check

    void test()
    {
        using mylist = List<int>;
        using stdlist = std::list<int>;

        try
        {
            check::front_and_back<mylist>();
            check::front_and_back<stdlist>();
            check::inter<mylist>();
            check::inter<stdlist>();
            check::insert<mylist>();
            check::insert<stdlist>();
            check::splice();
        }
        catch( const TestException& emsg )
        {
            std::cout << emsg.what() << std::endl;
        };

    }

};  // ListTestSpace

namespace HashTestSpace
{
    class TableEmptyButNotMustbeEmpty : public TestException
    {
    public:
        TableEmptyButNotMustbeEmpty() : TestException( "Таблица не должна быть пустой!!!" ) {}
    };

    class NoSizeTest : public TestException
    {
    public:
        NoSizeTest( const size_t& a, const size_t& b ) : TestException(
                ( "Размеры не равны: " + std::to_string(a) + " != " + std::to_string(b) )
            ) {}
    };

    class NoElementInTable : public TestException
    {
    public:
        NoElementInTable( const std::string& key, const int& value ) : TestException(
                    ( "Нету элемента: { " + key + " :" + std::to_string(value) )
                ) {}
    };

    class CountNotWork : public NoElementInTable
    {
    public:
        CountNotWork( const std::string& key, const int& value ) : NoElementInTable( key, value ) {}
    };

    class FindNotWork : public CountNotWork
    {
    public:
        FindNotWork( const std::string& key, const int& value ) : CountNotWork( key, value ) {}
    };

    class ClearMethodDontWork : public TestException
    {
    public:
        ClearMethodDontWork() : TestException("Метод clear не работает") {}
    };

    class LoadFactorNotCool : public TestException
    {
    public:
        LoadFactorNotCool( const float& lf, const float& cool ) : TestException(
                    ( "Проблема с load_factor: lf=" + std::to_string(lf)
                        + " ! cool=" + std::to_string(cool) )
                ) {}
    };

    class MaxLoadFactorNotCool : public TestException
    {
    public:
        MaxLoadFactorNotCool( const float& mlf, const float& cool ) : TestException(
                    ( "Проблема с max_load_factor: lf=" + std::to_string(mlf)
                                                        + " ! cool=" + std::to_string(cool) )
                ) {}
    };

    class OperatorNotWork : public TestException
    {
    public:
        OperatorNotWork() : TestException( " operator[] не работает :(" ) {}
    };

    class ReserveProblem : public TestException
    {
    public:
        ReserveProblem() : TestException( "Резервирование отрабатывает не корректно" ) {}
    };

    namespace check
    {
        std::vector<std::pair<std::string, int>> frame = {
            { "a", 10 },
            { "b", 20 },
            { "c", 30 }
        };

        template<typename T>
        void insert()
        {
            // Корректно вставляем
            T table;
            if( !table.empty() )
                throw TableEmptyButNotMustbeEmpty();

            for( const auto& [ key, value ] : frame )
                table.insert( std::pair<std::string, int>{ key, value } );

            if( table.empty() )
                throw TableEmptyButNotMustbeEmpty();

            // Проверка размера
            if( table.size() != frame.size() )
                throw NoSizeTest( table.size(), frame.size() );

            // Проверка итератора
            for( const auto [ key, value ] : frame )
            {
                bool noElement = true;

                // Проверка итератора
                for( const auto [ table_key, table_value ] : table )
                {
                    if( ( ( table_key == key ) and ( table_value == value ) ) )
                        noElement = false;
                }

                if( noElement )
                    throw NoElementInTable( key, value );

                // Проверка count
                if( table.count(key) != 1 )
                    throw CountNotWork( key, value );

                // Проверка find
                auto it_finded = table.find(key);
                if( ( ( (*it_finded).first != key ) and ( (*it_finded).second != value ) ) )
                    throw FindNotWork( key, value );

                if( table[key] != value )
                    throw OperatorNotWork();
            }
        }

        template<typename T>
        void clear()
        {
            T table(10);
            float lf = 0.0f;
            float mlf = 0.0f;
            table.max_load_factor( 1.0f );

            for( const auto& [ key, value ] : frame )
                table.insert( std::pair<std::string, int>{ key, value } );

            // Тестирование load_factor
            lf = table.load_factor();
            if( lf > 0.3f )
                throw LoadFactorNotCool( lf, 0.3f );

            mlf = table.max_load_factor();
            if( mlf != 1.0f )
                throw MaxLoadFactorNotCool( mlf, 1.0f );

            table.max_load_factor( 0.01f );
            mlf = table.max_load_factor();
            if( mlf != lf )
                throw MaxLoadFactorNotCool( mlf, lf );

            // Тестирование clear
            table.clear();

            if( !table.empty() )
                throw ClearMethodDontWork();

            // Тестирование load_factor
            lf = table.load_factor();
            if( lf != 0.0f )
                throw LoadFactorNotCool( lf, 0.0f );
        }

        template<typename T>
        void reserve()
        {
            T table(10);
            if( table.load_factor() != 0.0f )
                throw ReserveProblem();

            for( const auto& [ key, value ] : frame )
                table.insert( std::pair<std::string, int>{ key, value } );

            table.reserve(0);
            if( table.load_factor() != 1.0f )
                throw ReserveProblem();

            table.reserve(100);
            if( table.load_factor() > 0.03f )
                throw ReserveProblem();
        }
    
    };  // check

    void test()
    {
        using HT = HashTable<std::string, int>;
        using UM = std::unordered_map<std::string, int>;

        try
        {
            check::insert<HT>();
            check::clear<HT>();
            check::reserve<HT>();
            check::insert<UM>();
            check::clear<UM>();
            check::reserve<UM>();
        }
        catch( const TestException& emsg )
        {
            std::cout << emsg.what() << std::endl;
        };
    }

};  // HashTestSpace

namespace LRUTestSpace
{
    struct TestObj
    {
        std::string _key;
        int _value;
        bool _put;
    };

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

    bool TEST_CACHE_PUTaGET( const int& capacity, std::vector<TestObj>& testScript )
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
                    return false;
            }
        }
        return true;
    }

    void test()
    {
        assert( TEST_CACHE_PUTaGET( 2, BaseFromLeetCode ) );
        assert( TEST_CACHE_PUTaGET( 3, DoubleKey ) );
        assert( TEST_CACHE_PUTaGET( 2, EmptyCache ) );
        assert( TEST_CACHE_PUTaGET( 0, BaseFromLeetCode ) );
        assert( TEST_CACHE_PUTaGET( 2, ManyDoubleElement ) );
    }
};  // LRUTestSpace

#endif  // CPP_COURSE_TEST_H_
