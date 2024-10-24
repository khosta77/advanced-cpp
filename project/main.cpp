//#include "test.h"

#include "myHashTable.h"
//#include <unordered_map>

#include <list>

/*
 * Из ТЗ требование к list:
 * >> список, очевидно, будет шаблонным. Можно вдохновляться
 *    https://www.boost.org/doc/libs/1_67_0/doc/html/intrusive/list.html ,
 *    который "using the public member hook...".
 * >> внутри списка должны быть:
 *    - реализованы итераторы (обычный и const), -
 *    - empty(),                                 -
 *    - size(),                                  -
 *    - clear(),                                 -
 *    - front(),                                 -
 *    - back(),                                  -
 *    - push/pop_front(),                        -
 *    - push/pop_back(),                         -
 *    - insert(),                                -
 *    - splice() с семантикой, аналогичной STL.  -
 * */
template<typename T>
class List
{
private:
    
public:
    //// Member functions
    List() : {}
    ~List()

    //// Element access
    T& front() {}

    T& back() {}

    //// Iterators
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

    /** @brief begin - метод получения начала итератора
     * */
    iterator begin() noexcept { return iterator( _table.begin(), _table.end() ); }

    /** @brief cbegin - метод получения начала const итератора
     * */
    const_iterator cbegin() noexcept { return const_iterator( _table.cbegin(), _table.cend() ); }

    /** @brief end - метод получения конца итератора
     * */
    iterator end() noexcept { return iterator( _table.end(), _table.end() ); }

    /** @brief end - метод получения конца const итератора
     * */
    const_iterator cend() noexcept { return const_iterator( _table.cend(), _table.cend() ); }

    //// Capacity
    bool empty() {}

    size_t size() {}

    //// Modifiers
    void clear() {}

    iterator insert() {}

    void push_front() {}

    void push_back() {}

    void pop_front() {}

    void pop_back() {}
    
    //// Operations
    void splice() {}
};

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
    
    void test()
    {
        
    }
};

int main( int argc, char* argv[] )
{
    //HashTestSpace::test();
    ListTestSpace::test();
    return 0;
}

#if 0
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
    };

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
};
#endif


