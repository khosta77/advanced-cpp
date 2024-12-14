#include <typeinfo>
#include <iostream>
#include <string>

#include "json/single_include/nlohmann/json.hpp"

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

BOOST_FUSION_DEFINE_STRUCT(
    (pkg), Ranks,
    (int, r1)
    (int, r2)
    (int, r3)
    (std::string, some_str)
)

int main()
{
    std::cout << boost::fusion::extension::struct_size<pkg::Ranks>::value << std::endl;
    pkg::Ranks ranks = {1, 2, 3, "example"};
    boost::fusion::for_each(
        boost::mpl::range_c<unsigned, 0, boost::fusion::result_of::size<pkg::Ranks>::value>(), 
        [&](auto index)
        {
            std::cout << "Name: "
                      << boost::fusion::extension::struct_member_name<pkg::Ranks, index>::call()
                      << " Value: "
                      << boost::fusion::at_c<index>(ranks) << std::endl;
        }
    );

    return 0;
}
