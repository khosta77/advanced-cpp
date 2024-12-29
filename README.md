# Проект

[Задание](https://docs.google.com/document/d/1m5wexeH6hctr91Jm-uDB6BH0_154d0GiJdT_nyyrkS8/edit?tab=t.0)

### Комментарии

Надо скачать библиотеку

```cmd
git clone git@github.com:nlohmann/json.git
```

#### Надо сохранить!!! Очень долго до этого не мог догадатся

```C++
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/struct.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/type_index.hpp> 
#include <boost/mpl/vector.hpp>
#include <boost/type_index.hpp>

template<typename T>
void print(T data)
{
    boost::fusion::for_each(
        boost::mpl::range_c<unsigned, 0, boost::fusion::result_of::size<T>::value>(), 
        [&](auto index)
        {
            std::cout << "Name: "
                      << boost::fusion::extension::struct_member_name<T, index>::call()
                      << " Value: "
                      << boost::fusion::at_c<index>(data) << std::endl;
        }
    );
}
```
