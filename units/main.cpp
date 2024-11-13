#include "plane.hpp"

int main()
{
    Plane plane;
    std::stringstream input;
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
    return plane.load( input, std::cout );
}

