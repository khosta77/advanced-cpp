#include <typeinfo>
#include <iostream>
#include <string>

#include "json/single_include/nlohmann/json.hpp"
using Json = nlohmann::json;

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

template<typename FusionT>
std::string Serialize( const FusionT& fusion_obj )
{
    Json json_obj;

    boost::fusion::for_each(
        boost::mpl::range_c<unsigned, 0, boost::fusion::result_of::size<pkg::Ranks>::value>(), 
        [&](auto index)
        {
            json_obj[boost::fusion::extension::struct_member_name<pkg::Ranks, index>::call()] = 
                boost::fusion::at_c<index>(fusion_obj);
        }
    );

    return json_obj.dump();
}

template<typename FusionT>
FusionT Deserialize(std::string_view json_str)
{
    Json json_obj = Json::parse(json_str);
    FusionT fusion_obj{};

    boost::fusion::for_each(
        boost::mpl::range_c<unsigned, 0, boost::fusion::result_of::size<FusionT>::value>(),
        [&]( auto index )
        {
            using type = typename boost::fusion::result_of::value_at<FusionT, decltype(index)>::type;

            const auto name = boost::fusion::extension::struct_member_name<FusionT,index>::call();
            const auto value = json_obj[name].template get<type>();

            boost::fusion::at_c<index>(fusion_obj) = value;
        }
    );

    return fusion_obj;
}

int main()
{
    pkg::Ranks ranks = {1, 2, 3, "example"};
    std::string json_str = Serialize(ranks);
    std::cout << "Serialized JSON: " << json_str << std::endl;

    auto deserialized_ranks = Deserialize<pkg::Ranks>(json_str);
    std::cout << "Deserialized Ranks: "
              << deserialized_ranks.r1 << ", "
              << deserialized_ranks.r2 << ", "
              << deserialized_ranks.r3 << ", "
              << deserialized_ranks.some_str << std::endl;

    return 0;
}
