#include <cassert>
#include <iostream>
#include "plane.hpp"

void test()
{
	Plane plane;
	std::stringstream input, output, true_output;
    input << "FIRST_CLASS_SEGMENT 400" << std::endl;
    input << "BUSINESS_CLASS_SEGMENT 800" << std::endl;
    input << "ECONOMY_CLASS_SEGMENT 1000" << std::endl;
    input << "PILOT 0 0" << std::endl;
    input << "FLIGHT_ATTENDANT 0 0" << std::endl;
    input << "ECONOMY 5 20" << std::endl;
    input << "ECONOMY 5 4" << std::endl;
    input << "ECONOMY 5 10" << std::endl;
    input << "ECONOMY 5 15" << std::endl;
    input << "ECONOMY 5 16" << std::endl;
    input << "ECONOMY 5 10" << std::endl;
    input << "ECONOMY 5 20" << std::endl;
    input << "ECONOMY 5 21" << std::endl;
    input << "ECONOMY 5 20" << std::endl;
    input << "ECONOMY 5 15" << std::endl;
    input << "ECONOMY 5 6" << std::endl;
    input << "ECONOMY 5 8" << std::endl;
    input << "ECONOMY 5 10" << std::endl;
    input << "BUSINESS 0 10 34" << std::endl;
    input << "FIRST_CLASS 30 30 200 200" << std::endl;
    input << "FIRST_CLASS 0 0 850 100" << std::endl;
	plane.load( input, output );
	true_output << "!!PASSENGER’S LUGGAGE REMOVED FROM FLIGHT, ID = 12!!\n";
	true_output << "!!PASSENGER’S LUGGAGE REMOVED FROM FLIGHT, ID = 6!!\n";
	true_output << "!!PASSENGER’S LUGGAGE REMOVED FROM FLIGHT, ID = 11!!\n";
	true_output << "!!PASSENGER’S LUGGAGE REMOVED FROM FLIGHT, ID = 9!!\n";
	true_output << "!!PASSENGER’S LUGGAGE REMOVED FROM FLIGHT, ID = 3!!\n";
	true_output << "!!PASSENGER’S LUGGAGE REMOVED FROM FLIGHT, ID = 10!!\n";
	true_output << "!!PASSENGER’S LUGGAGE REMOVED FROM FLIGHT, ID = 7!!\n";

	assert(output.str() == true_output.str());

}

int main()
{
	test();
	return 0;
}

