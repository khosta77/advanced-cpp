#include <type_traits>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <limits>
#include <iomanip>
#include <typeinfo>

template<typename T>
struct is_string_literal
{
    static constexpr bool value =
        std::is_same<typename std::decay<T>::type, const char*>::value or
        std::is_same<typename std::decay<T>::type, char const*>::value;
};

template<typename T>
struct is_can_be_to_string
{
    static constexpr bool value = std::is_arithmetic<T>::value or is_string_literal<T>::value;
};

template<typename T>
using IsCanBeToString = std::enable_if_t<is_can_be_to_string<T>::value>;

template<typename T, typename = IsCanBeToString<T>>
std::string our_to_string(T value)
{
    if constexpr(std::is_same_v<T, bool>)
        return std::string( (value) ? "true" : "false" );
    else if constexpr(is_string_literal<T>::value)
        return std::string(value);
    else
        return std::to_string(value);
}

int main() {
    std::cout << our_to_string(true) << std::endl;
    std::cout << our_to_string(false) << std::endl;
    std::cout << our_to_string(42) << std::endl;
    std::cout << our_to_string(std::numeric_limits<uint64_t>::max()) << std::endl;
    std::cout << our_to_string(1.0f) << std::endl;
    std::cout << our_to_string(1.1f) << std::endl;
    std::cout << our_to_string(std::numeric_limits<double>::max()) << std::endl;
    std::cout << our_to_string("Lorem ipsum dolor sit amet, consectetur adipiscing elit") << std::endl;
    // будет ошибка компиляции с выводом почему неверно
    //std::cout << our_to_string(std::string("Hello, World!")) << std::endl;
    return 0;
}


