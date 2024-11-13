#ifndef UNITS_PASSANGERS_HPP_
#define UNITS_PASSANGERS_HPP_

#include "unit.hpp"

class FirstClass : public Passenger
{
public:
    FirstClass( const size_t& _id, const std::array<Bag, 4>& _bags ) : Passenger( _id, _bags, 2, 60, 2, -1,
            "FIRST_CLASS" ) {}
};

class Business : public Passenger
{
public:
    Business( const size_t& _id, const std::array<Bag, 4>& _bags ) : Passenger( _id, _bags, 2, 24, 2, 40,
                "BUSINESS" ) {}
};

class Economy : public Passenger
{
public:
    Economy( const size_t& _id, const std::array<Bag, 4>& _bags ) : Passenger( _id, _bags, 1, 10, 1, 24,
            "ECONOMY" ) {}
};


#endif  // UNITS_PASSANGERS_HPP_
