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
 *    - реализованы итераторы (обычный и const), +
 *    - empty(),                                 +
 *    - size(),                                  +
 *    - clear(),                                 +
 *    - front(),                                 +
 *    - back(),                                  +
 *    - push/pop_front(),                        +
 *    - push/pop_back(),                         +
 *    - insert(),                                +
 *    - splice() с семантикой, аналогичной STL.  +
 * */
template<typename T>
class List
{
public:
    class iterator;
    class const_iterator;

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

    friend class iterator;
    friend class const_iterator;

public:
    //// Member functions
    List() : _size(0), _head(nullptr), _tail(nullptr) {}

    ~List() { clear(); }

    //// Element access
    T& front() { return (T&)_head->_value; }

    T& back() { return (T&)_tail->_value; }

    //// Iterators
    class iterator
    {
    private:
        using It = Node*;

        It _it;
        bool _isEnd;

        friend class List;
    public:
        iterator( Node* node, const bool& isEnd = false ) : _it(node), _isEnd(isEnd) {}

        bool operator!=( const iterator& rhs ) const
        {
            if( ( _isEnd and ( rhs._isEnd == _isEnd ) ) )
                return false;

            int x = ( int( _it->_value != rhs._it->_value ) + int( _it->_prev != rhs._it->_prev )
                    + int( _it->_next != rhs._it->_next ) );

            if( ( ( x == 0 ) or ( ( x == 3 ) and ( _isEnd != rhs._isEnd ) ) ) )
                return true;
            return false;
        }
        
        bool operator==( const iterator& rhs ) const { return !( iterator( _it, _isEnd ) != rhs ); }

        iterator& operator++()
        {
            if( ( _isEnd and ( _it->_next != nullptr ) ) )
            {
                _isEnd = false;
                return *this;
            }

            if( _it->_next == nullptr )
                _isEnd = true;
            else
                _it = _it->_next;

            return *this;
        }

        iterator operator++(int)
        {
            iterator buffer( _it, _isEnd );

            if( ( _isEnd and ( _it->_next != nullptr ) ) )
            {
                _isEnd = false;
                return *this;
            }

            if( _it->_next == nullptr )
                _isEnd = true;
            else
                _it = _it->_next;

            return buffer;
        }

        iterator& operator--()
        {
            if( ( _isEnd and ( _it->_prev != nullptr ) ) )
            {
                _isEnd = false;
                return *this;
            }

            if( _it->_prev == nullptr )
                _isEnd = true;
            else
                _it = _it->_prev;

            return *this;
        }
        
        iterator operator--(int)
        {
            iterator buffer( _it, _isEnd );

            if( ( _isEnd and ( _it->_prev != nullptr ) ) )
            {
                _isEnd = false;
                return *this;
            }

            if( _it->_prev == nullptr )
                _isEnd = true;
            else
                _it = _it->_prev;

            return buffer;
        }
        
        T& operator*() { return (T&)_it->_value; }
    };

    class const_iterator
    {
    private:
        using It = Node*;

        It _it;
        bool _isEnd;
    public:
        const_iterator( Node* node, const bool& isEnd = false ) : _it(node), _isEnd(isEnd) {}

        bool operator!=( const const_iterator& rhs ) const
        {
            if( ( _isEnd and ( rhs._isEnd == _isEnd ) ) )
                return false;

            int x = ( int( _it->_value != rhs._it->_value ) + int( _it->_prev != rhs._it->_prev )
                    + int( _it->_next != rhs._it->_next ) );

            if( ( ( x == 0 ) or ( ( x == 3 ) and ( _isEnd != rhs._isEnd ) ) ) )
                return true;
            return false;
        }
        
        bool operator==( const const_iterator& rhs ) const { return !( const_iterator( _it, _isEnd ) != rhs ); }

        const_iterator& operator++()
        {
            if( ( _isEnd and ( _it->_next != nullptr ) ) )
            {
                _isEnd = false;
                return *this;
            }

            if( _it->_next == nullptr )
                _isEnd = true;
            else
                _it = _it->_next;

            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator buffer( _it, _isEnd );

            if( ( _isEnd and ( _it->_next != nullptr ) ) )
            {
                _isEnd = false;
                return *this;
            }

            if( _it->_next == nullptr )
                _isEnd = true;
            else
                _it = _it->_next;

            return buffer;
        }

        const_iterator& operator--()
        {
            if( ( _isEnd and ( _it->_prev != nullptr ) ) )
            {
                _isEnd = false;
                return *this;
            }

            if( _it->_prev == nullptr )
                _isEnd = true;
            else
                _it = _it->_prev;

            return *this;
        }
        
        const_iterator operator--(int)
        {
            const_iterator buffer( _it, _isEnd );

            if( ( _isEnd and ( _it->_prev != nullptr ) ) )
            {
                _isEnd = false;
                return *this;
            }

            if( _it->_prev == nullptr )
                _isEnd = true;
            else
                _it = _it->_prev;

            return buffer;
        }
        
        const T operator*() const { return _it->_value; }
    };

    /** @brief begin - метод получения начала итератора
     * */
    iterator begin() noexcept { return iterator(_head); }

    /** @brief cbegin - метод получения начала const итератора
     * */
    const_iterator cbegin() noexcept { return const_iterator(_head); }

    /** @brief end - метод получения конца итератора
     * */
    iterator end() noexcept { return iterator( _tail, true ); }

    /** @brief end - метод получения конца const итератора
     * */
    const_iterator cend() noexcept { return const_iterator( _tail, true ); }

    //// Capacity
    bool empty() const { return ( ( _head == nullptr ) and ( _tail == nullptr ) ); }

    size_t size() const { return _size; }

    //// Modifiers
    void clear()
    {
        if( ( _head == nullptr ) and ( _tail == nullptr ) )
            return;

        Node* node = _head->_next;
        while( node )
        {
            delete _head;
            _head = node;
            node = node->_next;
        }
        delete _head;
        _head = nullptr;
        _tail = nullptr;
        _size = 0;
    }

    iterator insert( iterator pos, const T& value )
    {
        if( pos._it == nullptr )
        {
            this->push_front(value);
            return this->begin();
        }
        
        Node* node = new Node( value, nullptr, nullptr );

        if( pos._it == _head )
        {
            node->_next = _head;
            _head->_prev = node;
            _head = node;
        }
        else
        {
            node->_prev = pos._it->_prev;
            node->_next = pos._it;

            if( pos._it->_prev )
                pos._it->_prev->_next = node;
            
            pos._it->_prev = node;
        }

        return iterator(node);
    }

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

    //// Operations
    void splice( iterator pos, List& other, iterator it )
    {
        if( ( it._it == nullptr ) || ( other.empty() ) )
            return;

        Node* nodeToMove = it._it;
        Node* prevNode = nodeToMove->_prev;
        Node* nextNode = nodeToMove->_next;

        if(prevNode)
            prevNode->_next = nextNode;
        if(nextNode)
            nextNode->_prev = prevNode;

        if( other._head == nodeToMove )
            other._head = nextNode;
        if( other._tail == nodeToMove )
            other._tail = prevNode;

        Node* posNode = pos._it;
        nodeToMove->_prev = posNode->_prev;
        nodeToMove->_next = posNode;

        if( posNode->_prev )
            posNode->_prev->_next = nodeToMove;
        posNode->_prev = nodeToMove;

        if( posNode == _head )
            _head = nodeToMove;

        --other._size;
        ++_size;
    }
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
    };

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


