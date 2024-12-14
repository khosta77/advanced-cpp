#include <functional>
#include <iostream>
#include <memory>
#include <random>
#include <tuple>
#include <sstream>
#include <cassert>

std::string f(int n1, int n2, int n3, const int& n4, int n5)
{
    std::stringstream ss;
    ss << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5;
    return ss.str();
}
 
int g(int n1)
{
    return n1;
}
 
struct Foo
{
    std::string print_sum(int n1, int n2)
    {
        std::stringstream ss;
        ss << n1 + n2;
        return ss.str();
    }
 
    int data = 10;
};

////
template<typename F, typename... Args>
class Bind
{
public:
    Bind( F f, Args... args ) : func(f), boundArgs(args...) {}

    template<typename... CallArgs>
    auto operator()( CallArgs... callArgs ) {
        return callImpl( std::index_sequence_for<Args...>(), callArgs... );
    }

private:
    F func;
    std::tuple<Args...> boundArgs;

    template<std::size_t... I, typename... CallArgs>
    auto callImpl( std::index_sequence<I...>, CallArgs... callArgs ) {
        // Механизм такой, мы берем изначальный Args..., из него получаем список
        // индексов подставляемых аргументов. Через get распаковываем его и дальше
        // подставлемя введенные аргументы.
        //
        // Только тут проблема:
        // 1) В такой реализации нельзя использовать std::placeholder
        // 2) Передача аргументов из методов тоже не работает.
        //
        // По факту данная реализация близка к std::format, где вместо строки функция
        return func( std::get<I>(boundArgs)..., callArgs... );
    }
};

template<typename F, typename... Args>
auto mbind(F f, Args... args) {
    return Bind<F, Args...>(f, args...);
}
////

int main()
{
    //using namespace std::placeholders;
    // c std::placeholders не получалось

    auto f1 = mbind(f, 2, 42, 1);
    assert("2 42 1 10 7" == f1(10, 7));
 

    int n = 7;
    auto lambda = [&ncref = n, n](auto a, auto b, auto /*unused*/) {
        return f(b, 42, a, ncref, n);
    };
    n = 10;
    assert("2 42 1 10 7" == lambda(1, 2, 1001));


    auto g1 = mbind(g, 10);
    auto f2 = mbind(f, 9, g1(), 11);
    assert("9 10 11 12 13" == f2(12, 13));

#if 0
    Foo foo;
    auto f3 = bind(&Foo::print_sum, &foo, 95, _1);
    assert("100" == f3(5));
 
    auto ptr_to_print_sum = std::mem_fn(&Foo::print_sum);
    auto f4 = bind(ptr_to_print_sum, &foo, 95, _1);
    assert("100" == f4(5));
 

    auto f5 = bind(&Foo::data, _1);
    assert(10 == f5(foo));
 

    auto ptr_to_data = std::mem_fn(&Foo::data);
    auto f6 = bind(ptr_to_data, _1);
    assert(10 == f6(foo));
 

    std::stringstream ss7;
    ss7 << f6(std::make_shared<Foo>(foo)) << ' ' << f6(std::make_unique<Foo>(foo));
    assert("10 10" == ss7.str());
#endif
    return 0;
}
