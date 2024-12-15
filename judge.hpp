#ifndef _HW3_JUDGE_HPP_
#define _HW3_JUDGE_HPP_

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


#endif  // _HW3_JUDGE_HPP_
