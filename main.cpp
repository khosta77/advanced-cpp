#include "serialize.hpp"
#include "deserialize.hpp"

BOOST_FUSION_DEFINE_STRUCT(
    (pkg), Ranks,
    (int, r1)
    (int, r2)
    (int, r3)
    (std::string, some_str)
)

void test01()
{
    pkg::Ranks ranks = {1, 2, 3, "example"};

    std::string json_str = Serialize(ranks);
    assert( json_str == "{\"r1\":1,\"r2\":2,\"r3\":3,\"some_str\":\"example\"}" );

    auto deserialized_ranks = Deserialize<pkg::Ranks>(json_str);
    assert( deserialized_ranks.r1 == 1 );
    assert( deserialized_ranks.r2 == 2 );
    assert( deserialized_ranks.r3 == 3 );
    assert( deserialized_ranks.some_str == "example" );
}

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

void test02()
{
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
    assert( json_str == "{\"r1\":123,\"r2\":1.2300000190734863,\"s1_vals\":[{\"r0\":1},{\"r0\":2}],\"s2_val\":{\"val\":1.2200000286102295},\"some_str\":\"abcde\",\"vals\":[1,2,3]}" );

    auto deserialized_s3 = Deserialize<pkg::S3>(json_str);
    assert( deserialized_s3.r1 == 123 );
    assert( deserialized_s3.r2 == 1.23f );
    assert( deserialized_s3.some_str == "abcde" );
    assert( deserialized_s3.vals.size() == 3 );
    assert( deserialized_s3.vals[0] == 1 );
    assert( deserialized_s3.vals[1] == 2 );
    assert( deserialized_s3.vals[2] == 3 );
    assert( deserialized_s3.s2_val.val == 1.22f );
    assert( deserialized_s3.s1_vals.size() == 2 );
    assert( deserialized_s3.s1_vals[0].r0 == 1 );
    assert( deserialized_s3.s1_vals[1].r0 == 2 );
}

BOOST_FUSION_DEFINE_STRUCT(
	(pkg), S4,
	( bool, val )
)

void test03()
{
	// Закомментировал, тк будет ошибка компи
	//pkg::S4 s4(true);
    //std::string json_str = Serialize(s4);
}

void test04()
{
    pkg::Ranks ranks = {1, 2, 3, "example"};

    std::string json_str = "{\"r1\":1,\"r2\":2,\"r3\":3,\"r4\":4,\"some_str\":\"example\"}";

	try
	{
		auto deserialized_ranks = Deserialize<pkg::Ranks>(json_str);
	}
	catch( std::exception& emsg )
	{
		const std::string error_msg = emsg.what();
        assert( error_msg == "\n\tВ json лишний элемент r4" );
	};
}

void test05()
{
    pkg::Ranks ranks = {1, 2, 3, "example"};

    std::string json_str = "{\"r1\":1,\"r2\":2,\"some_str\":\"example\"}";

	try
	{
		auto deserialized_ranks = Deserialize<pkg::Ranks>(json_str);
	}
	catch( std::exception& emsg )
	{
		const std::string error_msg = emsg.what();
        assert( error_msg == "\n\tВ json отсутствует ключ r3" );
	};
}


int main()
{
    test01();
    test02();
	test03();
	test04();
    test05();
    return 0;
}
