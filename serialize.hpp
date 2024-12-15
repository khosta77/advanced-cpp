#ifndef _HW3_SERIALIZE_HPP_
#define _HW3_SERIALIZE_HPP_

#include "judge.hpp"

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
                    std::vector<Json> results(values.size());
                    size_t i = 0;
                    for( const auto& value : values )
                    {
                        std::string obj = Serialize( value ); 
                        results[i++] = Json::parse( std::move(obj) );
                    }
                    json_obj[name] = results;
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
                    const std::string obj = Serialize( boost::fusion::at_c<index>(fusion_obj) ); 
                    json_obj[name] = Json::parse(obj);
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

#endif  // _HW3_SERIALIZE_HPP_
