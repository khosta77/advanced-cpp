//#include "test.h"

#include "myHashTable.h"
#include <unordered_map>

class TestException : std::exception 
{
private:
    std::string emsg;

public:
    explicit TestException( const std::string& msg ) : emsg(msg) {}
    const char *what() const noexcept override { return emsg.c_str(); }
};

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


namespace HashTestSpace
{
    namespace check
    {
        template<typename T>
        void insert()
        {
            std::vector<std::pair<std::string, int>> frame = {
                { "a", 10 },
                { "b", 20 },
                { "c", 30 }
            };

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
            }


        }
    };

    void test()
    {
        using HT = HashTable<std::string, int>;
        using UM = std::unordered_map<std::string, int>;

        check::insert<HT>();
        check::insert<UM>();
#if 0
        HashTable<std::string, int> mt(1);
        std::cout << "Размер моей таблица: " << mt.size() << std::endl;
        mt.max_load_factor(0.5);
        mt.insert(std::pair<std::string, int>{"a", 1});
        mt.insert(std::pair<std::string, int>{"b", 3});
        mt.insert(std::pair<std::string, int>{"c", 2});
        mt.insert(std::pair<std::string, int>{"d", 2});
        mt.insert(std::pair<std::string, int>{"e", 2});
        mt.insert(std::pair<std::string, int>{"g", 2});
        mt.insert(std::pair<std::string, int>{"f", 2});
        mt.insert(std::pair<std::string, int>{"v", 2});
        mt.insert(std::pair<std::string, int>{"q", 2});
        mt.insert(std::pair<std::string, int>{"x", 2});
        mt.max_load_factor(0.05);

        std::cout << "Мои факторы: " << mt.load_factor() << ' ' << mt.max_load_factor() << std::endl;
        std::cout << mt.size() << std::endl;

        std::unordered_map<std::string, int> st(1);
        std::cout << "Размер STL таблица: " << st.size() << std::endl;
        st.max_load_factor(0.5);
        st.insert(std::pair<std::string, int>{"a", 1});
        st.insert(std::pair<std::string, int>{"b", 3});
        st.insert(std::pair<std::string, int>{"a", 7});
        st.insert(std::pair<std::string, int>{"c", 2});
        st.insert(std::pair<std::string, int>{"d", 2});
        st.insert(std::pair<std::string, int>{"e", 2});
        st.insert(std::pair<std::string, int>{"g", 2});
        st.insert(std::pair<std::string, int>{"f", 2});
        st.insert(std::pair<std::string, int>{"v", 2});
        st.insert(std::pair<std::string, int>{"q", 2});
        st.insert(std::pair<std::string, int>{"x", 2});
        st.max_load_factor(0.05);

        std::cout << "STL факторы: " << st.load_factor() << ' ' << st.max_load_factor() << std::endl;
        std::cout << st.size() << std::endl;
        std::cout << st.count("a") << std::endl;
    
        for( auto it = mt.begin(); it != mt.end(); ++it )
            (*it).second += 1;

        auto mit = mt.begin();
        auto sit = st.begin();
        for( ; ( ( mit != mt.end() ) and ( sit != st.end() ) ); ++mit, ++sit )
            std::cout << (*mit).first << '=' << (*sit).first << '|'
                      << (*mit).second << '=' << (*sit).second << std::endl;
        std::cout << "Раскладка может отличатся!!!" << std::endl;

        for( const auto [ key, value ] : st )
            std::cout << key << ' ' << value << std::endl;
        std::cout << "Сокращаем" << std::endl;
        st.reserve(2);
        for( const auto [ key, value ] : st )
            std::cout << key << ' ' << value << std::endl;

        for( const auto [ key, value ] : mt )
            std::cout << key << ' ' << value << std::endl;
        std::cout << "Сокращаем" << std::endl;
        mt.reserve(2);
        for( const auto [ key, value ] : mt )
            std::cout << key << ' ' << value << std::endl;
#endif
    }
};

int main( int argc, char* argv[] )
{

    //test();
    return 0;
}


