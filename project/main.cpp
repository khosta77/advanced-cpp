//#include "test.h"

#include "myHashTable.h"
#include <unordered_map>

int main( int argc, char* argv[] )
{
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
        std::cout << (*mit).first << '=' << (*sit).first << '|' << (*mit).second << '=' << (*sit).second << std::endl;
    std::cout << "Раскладка может отличатся" << std::endl;
    //test();
    return 0;
}


