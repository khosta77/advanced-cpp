#include <iostream>
#include <tuple>
#include <utility>

template<typename Tuple, typename Predicate, std::size_t... Is>
// https://en.cppreference.com/w/cpp/utility/integer_sequence
void for_each_element_impl(Tuple& tuple, const Predicate& p, std::index_sequence<Is...>)
{
    (p(std::get<Is>(tuple)), ...);
}

template<typename... Args, typename Predicate>
void for_each_element(std::tuple<Args...>& tt, const Predicate& p)
{
    for_each_element_impl(tt, p, std::index_sequence_for<Args...>{});
}

int main()
{
    std::tuple<int, double, int, size_t, std::string> buffer(1, 2.5, 10, 20, "X");
    for_each_element(buffer, [](const auto& value){ std::cout << value << std::endl; });
    return 0;
}


