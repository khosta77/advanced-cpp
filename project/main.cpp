//#include "test.h"

#include "myHashTable.h"
//#include <unordered_map>

#include <list>
#include <stack>

/*
 * Из ТЗ требование к list:
 * >> список, очевидно, будет шаблонным. Можно вдохновляться
 *    https://www.boost.org/doc/libs/1_67_0/doc/html/intrusive/list.html ,
 *    который "using the public member hook...".
 * >> внутри списка должны быть:
 *    - реализованы итераторы (обычный и const), -
 *    - empty(),                                 +
 *    - size(),                                  +
 *    - clear(),                                 +
 *    - front(),                                 +
 *    - back(),                                  +
 *    - push/pop_front(),                        +
 *    - push/pop_back(),                         +
 *    - insert(),                                -
 *    - splice() с семантикой, аналогичной STL.  -
 * */
template<typename T>
class List
{
private:
    size_t _size;

    struct Node
    {
        T _value;
        Node* _next;
        Node* _prev;

        Node( const T& value, Node* next, Node* prev ) : _value(value), _next(next), _prev(prev) {}
    };

    Node* _head;
    Node* _tail;
    
public:
    //// Member functions
    List() : _size(0), _head(nullptr), _tail(nullptr) {}

    ~List()
    {
        clear();
    }

    //// Element access
    T& front() { return (T&)_head->_value; }

    T& back() { return (T&)_tail->_value; }

    //// Iterators
    class iterator
    {
    private:
        using It = Node*;

        It _it;
    public:
        iterator( Node* node ) : _it(node)
        {
        }

        bool operator!=( const iterator& rhs ) const
        {
            if( _it == nullptr )
                return ( rhs._it != nullptr );
            
            if( rhs._it == nullptr )
                return true;

            return ( _it->_value != rhs._it->_value );
        }
        
        bool operator==( const iterator& rhs ) const
        {
            return !( this != rhs );
        }

        iterator& operator++()
        {
            _it = _it->_next;
            return *this;
        }

        iterator operator++(int)
        {
            iterator buffer = this;
            _it = _it->_next;
            return buffer;
        }

        iterator& operator--()
        {
            _it = _it->_prev;
            return *this;
        }
        
        iterator operator--(int)
        {
            iterator buffer = this;
            _it = _it->_prev;
            return buffer;
        }
        
        T& operator*()
        {
            return (T&)_it->_value;
        }
    };

#if 0
    class const_iterator
    {
    private:
        using const_It = std::vector<Cell>::iterator;

    public:
        const_iterator() : {}

        bool operator!=(const const_iterator& other) const {}
        bool operator==(const const_iterator& other) const {}

        const_iterator& operator++() {}
        const_iterator operator++(int) {}
        const_iterator& operator--() {}
        const_iterator operator--(int) {}
        const T operator*() {}
    };
#endif

    /** @brief begin - метод получения начала итератора
     * */
    iterator begin() noexcept
    {
        return iterator(_head);
    }

#if 0
    /** @brief cbegin - метод получения начала const итератора
     * */
    const_iterator cbegin() noexcept {}
#endif

    /** @brief end - метод получения конца итератора
     * */
    iterator end() noexcept
    {
        return iterator(nullptr);
    }

#if 0
    /** @brief end - метод получения конца const итератора
     * */
    const_iterator cend() noexcept {}
#endif

    //// Capacity
    bool empty() const
    {
        return ( ( _head == nullptr ) and ( _tail == nullptr ) ); }

    size_t size() const { return _size; }

    //// Modifiers
    void clear()
    {
        if( ( _head == nullptr ) and ( _tail == nullptr ) )
            return;

        Node* node = _head->_next;
        while( node )
        {
            --_size;
            delete _head;
            _head = node;
            node = node->_next;
        }
        delete _head;
        _head = nullptr;
        _tail = nullptr;
        _size = 0;
    }

    //iterator insert() {}

private:
    void push_first_element( const T& value )
    {
        Node* node = new Node( value, nullptr, nullptr );
        _head = _tail = node;
        ++_size;
    }

public:
    void push_front( const T& value )
    {
        if( _size == 0 )
            return push_first_element( value );
        
        Node* node = new Node( value, _head, nullptr );
        _head->_prev = node;
        _head = node;
        ++_size;
    }

    void push_back( const T& value )
    {
        if( _size == 0 )
            return push_first_element( value );
        
        Node* node = new Node( value, nullptr, _tail );
        _tail->_next = node;
        _tail = node;
        ++_size;
    }

private:
    void pop_last_element()
    {
        Node* node = _head;
        _head = nullptr;
        _tail = nullptr;
        delete node;
        --_size;
    }

public:
    void pop_front()
    {
        if( _size == 0 )
            return;
        
        if( _size == 1 )
            return pop_last_element();

        Node* node = _head;
        _head = _head->_next;
        _head->_prev = nullptr;
        --_size;
        delete node;
    }

    void pop_back()
    {
        if( _size == 0 )
            return;
        
        if( _size == 1 )
            return pop_last_element();

        Node* node = _tail;
        _tail = _tail->_prev;
        _tail->_next = nullptr;
        --_size;
        delete node;
    }

#if 1
    void print()
    {
        Node* node = _head;
        while( node )
        {
            std::cout << node->_value << '\t';
            node = node->_next;
        }
        std::cout << std::endl;
        node = _tail;
        while( node )
        {
            std::cout << node->_value << '\t';
            node = node->_prev;
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
#endif
    //// Operations
    //void splice() {}
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
    
    namespace check
    {
        std::vector<int> frame = { -3, -2, -1, 0, 1, 2, 3 };

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
            const size_t MIDDLE = 3;
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

        }

    };

    void test()
    {
        using mylist = List<int>;
        using stdlist = std::list<int>;

        try
        {
            check::front_and_back<mylist>();
            check::front_and_back<stdlist>();
        }
        catch( const TestException& emsg )
        {
            std::cout << emsg.what() << std::endl;
        };

#if 0
        List<int> ml;
        ml.push_back(1);
        std::cout << ml.front() << ' ' << ml.back() << std::endl;
        ml.push_back(2);
        std::cout << ml.front() << ' ' << ml.back() << std::endl;
        ml.push_back(3);
        std::cout << ml.front() << ' ' << ml.back() << std::endl;
#endif
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


