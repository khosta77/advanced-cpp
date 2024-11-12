#include <iostream>
#include <format>
#include <chrono>

template<typename T>
void myprintln( std::deque<std::string>& deq, T value ) {
    std::cout << deq.front() << value;
    deq.pop_front();
    if( deq.empty() )
    {
        std::cout << std::endl;
        return;
    }
    std::cout << deq.front() << std::endl;
    deq.pop_front();
}

template<typename T, typename... Args>
void myprintln( std::deque<std::string>& deq, T first, Args... args) {
    std::cout << deq.front() << first;
    deq.pop_front();
    myprintln(deq, args...);
}

template<typename... Args>
void myprintln( const std::string& message, Args... args )
{
    std::deque<std::string> deq;
    for( size_t i = message.find("{}", 0), j = 0, J = message.size(); j != J; j = (++i + 1), i = message.find("{}", i) )
    {
        if( i == std::string::npos )  // Если у нас есть сообщение после последних {} 
        {
            deq.push_back( message.substr( j, ( message.size() - j ) ) );;
            break; 
        }
        deq.push_back( message.substr( j, ( i - j ) ) );;
    }
    myprintln(deq, args...);
}

template <class T>
void println( std::string_view fmt, T&& value )
{
    size_t i = fmt.find("{}");
    std::cout << fmt.substr(0, i) << value;
    if( i != std::string::npos )
        std::cout << fmt.substr( ( i + 2 ) );
    std::cout << std::endl;
}

template <class T, class... Args>
void println( std::string_view fmt, T&& value, Args&&... args )
{
    size_t i = fmt.find("{}");
    std::cout << fmt.substr(0, i) << value;
    println( fmt.substr( ( i + 2 ) ), args...);
}

int main()
{
    auto res = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();

    println( "{}, {}! unix timestamp right now: {}", "hello", "world", res);
    println( "today: {}:{}:{}, is {}!", 12, 11, 2024, "tuesday" );

    // Моя версия, чутка другое объявление
    myprintln( "{}, {}! unix timestamp right now: {}", "hello", "world", res);
    myprintln( "today: {}:{}:{}, is {}!", 12, 11, 2024, "tuesday" );
    return 0;
}


