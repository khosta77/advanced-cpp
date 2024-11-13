#include <cassert>
#include "passengers.hpp"

void test()
{
    std::array<Bag, 4> bags_first = { Bag(60, 20), Bag(60, 20), Bag(200, 20), Bag(100, 20) };
    std::array<Bag, 4> bags_business = { Bag(24, 12), Bag(24, 12), Bag(40, 12), Bag(40, 12) };
    std::array<Bag, 4> bags_econom = { Bag(10, 13), Bag(0, 12), Bag(20, 13), Bag(0, 13) };

    Passenger *first_class01 = new FirstClass( 20, bags_first );
    Passenger *first_class02 = new FirstClass( 12, bags_business );
    Passenger *first_class03 = new FirstClass( 13, bags_econom );

    assert( first_class01->name == "FIRST_CLASS" );
    assert( first_class02->name == "FIRST_CLASS" );
    assert( first_class03->name == "FIRST_CLASS" );

    assert( first_class01->WillWeBeAbleToRegisterIt() );
    assert( first_class02->WillWeBeAbleToRegisterIt() );
    assert( first_class03->WillWeBeAbleToRegisterIt() );
	
	delete first_class01;
	delete first_class02;
	delete first_class03;
	// У остальных так же
}

int main()
{
	test();
    return 0;
}

