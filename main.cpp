#include <type_traits>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <limits>
#include <iomanip>

template <typename T, typename = void>
struct HasOutputOperator : std::false_type {};

template <typename T>
struct HasOutputOperator<T, std::void_t<decltype(std::cout << std::declval<T>())>> : std::true_type {};

template <typename T>
concept MustHavingOutPutOperator = HasOutputOperator<T>::value;

template <typename T>
requires MustHavingOutPutOperator<T>
std::string our_to_string(T value)
{
    std::stringstream ss;
    if constexpr(std::is_same_v<T, bool>)
        ss << std::boolalpha << value;
    else if constexpr(std::is_same_v<T, float>)
        ss << std::fixed << std::setprecision(7) << value;
    else if constexpr(std::is_same_v<T, double>)
        ss << std::fixed << std::setprecision(15) << value;
    else
        ss << value;

    std::string str = ss.str();
    if constexpr(std::floating_point<T>)  // Обрезаем нули в конце float|double
    {
        const size_t end_zeros = (str.find_last_not_of('0') + 1);
        return str.erase( ( (str[end_zeros - 1] == '.') ? (end_zeros + 1) : end_zeros ) );
    }
    else
        return str;
}

struct A
{
    int a;
};

struct B
{
    int b;
};

std::ostream& operator<<(std::ostream& os, const B& rhs)
{
    os << rhs.b;
    return os;
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
    A a_;
    a_.a = 10;
    //std::cout << our_to_string(a_) << std::endl;  // будет ошибка компиляции с выводом почему неверно
    B b_;
    b_.b = 20;
    std::cout << our_to_string(b_) << std::endl;
    return 0;
}


