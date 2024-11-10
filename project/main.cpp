#include "test.h"

int main( int argc, char* argv[] )
{
    ListTestSpace::test();
    HashTestSpace::test();
    LRUTestSpace::test();
	LRUHWTestSpace::test();
	//loop( std::cin, std::cout );
    return 0;
}


