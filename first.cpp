#include <iostream>
#include <variant>
#include <tuple>
#include <type_traits>

template<template <typename...> class T, typename Tuple>
struct ArgsSubstitutor;

template<template <typename...> class T, typename... Args>
struct ArgsSubstitutor<T, std::tuple<Args...>>
{
    using type = T<Args...>;
};

template <typename... Args>
struct tempalte_class;

int main()
{
    using TT = std::tuple<int, double, std::string>;

    using VariantType = ArgsSubstitutor<std::variant, TT>::type;
    using TemplateClassType = ArgsSubstitutor<tempalte_class, TT>::type;

    static_assert(std::is_same_v<VariantType, std::variant<int, double, std::string>>);
    static_assert(std::is_same_v<TemplateClassType, tempalte_class<int, double, std::string>>);
    return 0;
}


