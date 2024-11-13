#ifndef UNITS_CREW_MEMBERS_HPP_
#define UNITS_CREW_MEMBERS_HPP_

#include "unit.hpp"

class Pilot : public Passenger
{
public:
    Pilot( const size_t& _id, const std::array<Bag, 4>& _bags ) : Passenger( _id, _bags, 2, 0, 2, 0,
            "PILOT" ) {}
};

class FlightAttendant : public Passenger
{
public:
    FlightAttendant( const size_t& _id, const std::array<Bag, 4>& _bags ) : Passenger( _id, _bags, 2, 0, 2,
            0, "FLIGHT_ATTENDANT" ) {}
};

#endif  // UNITS_CREW_MEMBERS_HPP_
