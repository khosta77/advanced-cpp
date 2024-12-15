#include <typeinfo>
#include <iostream>
#include <cassert>
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

BOOST_FUSION_DEFINE_STRUCT(
	(pkg), S1,
	(int, r0)
)

BOOST_FUSION_DEFINE_STRUCT(
	(pkg), S2,
	(float, val)
)

BOOST_FUSION_DEFINE_STRUCT(
	(pkg), S3,
	(int, r1)
    (float, r2)
	(std::string, some_str)
    (std::vector<int>, vals)
    (pkg::S2, s2_val)
    (std::vector<pkg::S1>, s1_vals)
)

template <typename T>
using is_fusion_struct = std::is_same<typename boost::fusion::traits::tag_of<T>::type, boost::fusion::struct_tag>;

template <typename T>
struct is_vector : std::false_type {};

template <typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>> : std::true_type {};

template<typename T>
struct is_valid_type
{
    static const bool value = (
        std::disjunction<
            std::is_same<T, int>, std::is_same<T, float>, std::is_same<T, std::string>, is_fusion_struct<T>
        >::value or
        is_vector<T>() );
};

template<typename FusionT>
std::string Serialize( const FusionT& fusion_obj )
{
    Json json_obj;

    boost::fusion::for_each(
        boost::mpl::range_c<unsigned, 0, boost::fusion::result_of::size<FusionT>::value>(), 
        [&](auto index)
        {
            using type = typename boost::fusion::result_of::value_at<FusionT, decltype(index)>::type;
            const auto name = boost::fusion::extension::struct_member_name<FusionT, index>::call();

            if constexpr( !is_valid_type<type>::value )
            {
                throw std::runtime_error(
                    std::format(
                        "\n\tНе допустимое значение {}",
                        boost::typeindex::type_id<type>().pretty_name()
                    )
                );
            }
            
            if constexpr( is_vector<type>() )
            {
                if constexpr( is_fusion_struct<typename type::value_type>() == 1 )
                {
                    const auto values = boost::fusion::at_c<index>(fusion_obj);
                    std::vector<std::string> results(values.size());
                    size_t i = 0;
                    for( const auto& value : values )
                    {
                        results[i++] = Serialize( value );
                    }
                    json_obj[name] = std::move(results);
                }
                else
                {
                    json_obj[name] = boost::fusion::at_c<index>(fusion_obj);
                }
            }
            else
            {
                if constexpr( is_fusion_struct<type>() == 1 )
                {
                    json_obj[name] = Serialize( boost::fusion::at_c<index>(fusion_obj) );
                }
                else
                {
                    json_obj[name] = boost::fusion::at_c<index>(fusion_obj);
                }
            }
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

void test01()
{
    pkg::Ranks ranks = {1, 2, 3, "example"};
    std::string json_str = Serialize(ranks);
    assert( json_str == "{\"r1\":1,\"r2\":2,\"r3\":3,\"some_str\":\"example\"}" );
    //std::cout << "JSON: " << json_str << std::endl;

    auto deserialized_ranks = Deserialize<pkg::Ranks>(json_str);
    assert( deserialized_ranks.r1 == 1 );
    assert( deserialized_ranks.r2 == 2 );
    assert( deserialized_ranks.r3 == 3 );
    assert( deserialized_ranks.some_str == "example" );
}

int main()
{
    //test01();

    pkg::S1 s1_0(1);
    pkg::S1 s1_1(2);
    pkg::S2 s2(1.22);
    pkg::S3 s3;
    s3.r1 = 123;
    s3.r2 = 1.23;
    s3.some_str = "abcde";
    s3.vals = std::vector<int>{1, 2, 3};
    s3.s2_val = s2;
    s3.s1_vals = std::vector<pkg::S1>{s1_0, s1_1};
    std::string json_str = Serialize(s3);
    std::cout << ( ( json_str == "null" ) ? "" : json_str ) << std::endl;
    return 0;
}
