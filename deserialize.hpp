#ifndef _HW3_DESERIALIZE_HPP_
#define _HW3_DESERIALIZE_HPP_

#include "judge.hpp"

template<typename FusionT>
FusionT Deserialize(std::string_view json_str)
{
    Json json_obj = Json::parse(json_str);
    FusionT fusion_obj{};

    // Надо проверять так же json, которые приходят на вход. Просто из условия возможна такая ситуация,
    // что на вход придет либо не полный json. Либо с лишним элементом...
    std::unordered_map<std::string, bool> content( json_obj.size() );
    for( const auto& it : json_obj.items() )
        content[it.key()] = false;

    boost::fusion::for_each(
        boost::mpl::range_c<unsigned, 0, boost::fusion::result_of::size<FusionT>::value>(),
        [&]( auto index )
        {
            using type = typename boost::fusion::result_of::value_at<FusionT, decltype(index)>::type;
			static_assert( is_valid<type>::value );  // Выполняется на этапе компиляции

            const auto name = boost::fusion::extension::struct_member_name<FusionT,index>::call();

            // Тут проверка на то, есть ли имя переменной, что хотим найти в json
            if( content.count(name) == 0 )
                throw std::runtime_error( std::format( "\n\tВ json отсутствует ключ {}", name ) );
            content[name] = true;
 
            if constexpr( is_valid_vector_t<type>::value )
            {
                if constexpr( is_fusion_struct<typename type::value_type>() == 1 )
                {
                    const std::vector<Json> values = json_obj[name].template get<std::vector<Json>>();
                    type results(values.size());
                    size_t i = 0;
                    for( const auto& value : values )
                    {
                        const auto obj = Deserialize<typename type::value_type>( value.dump() );
                        results[i++] = obj;
                    }
                    boost::fusion::at_c<index>(fusion_obj) = results;
                }
                else
                {
                    const type value = json_obj[name].template get<type>();
                    boost::fusion::at_c<index>(fusion_obj) = value;
                }
            }
            else
            {
                if constexpr( is_fusion_struct<type>() == 1 )
                {
                    const type value = Deserialize<type>( json_obj[name].dump() );
                    boost::fusion::at_c<index>(fusion_obj) = value;
                }
                else
                {
                    const type value = json_obj[name].template get<type>();
                    boost::fusion::at_c<index>(fusion_obj) = value;
                }
            }
        }
    );

    // Если какой-то переменной не найдем, вернет ошибку
    for( const auto& [key, value] : content )
    {
        if( !value )
        {
            throw std::runtime_error( std::format( "\n\tВ json лишний элемент {}", key ) );
        }
    }

    return fusion_obj;
}

#endif  // _HW3_DESERIALIZE_HPP_
