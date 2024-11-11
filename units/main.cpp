#include <iostream>
#include <string>
#include <map>
#include <cassert>
#include <sstream>
#include <set>

#if 0
class Pilot
{
private:

public:
    Pilot() {}
    ~Pilot() {}
};

class FlightAttendant
{
private:

public:
    FlightAttendant() {}
    ~FlightAttendant() {}
};

class Passenger
{
private:

public:
    Passenger() {}
    ~Passenger() {}
};

class FirstClass : public Passenger
{

};

class Business : public Passenger
{

};

class Economy : public Passenger
{

};
#endif

struct Bag
{
	size_t weight;
	size_t passengerId;

	Bag( const size_t& w = 0, const size_t& id = 0 ) : weight(w), passengerId(id) {}
	bool operator<( const Bag& other ) const { return ( weight < other.weight ); }
};

class Plane
{
private:
	struct Segment
	{
		std::unordered_map<size_t, std::list<Bag>> bags_group;
		size_t currentWeight;
		size_t maxWieght;

		Segment() = delete;
		Segment( const Segment& ) = delete;
		Segment( Segment&& ) = delete;
		Segment( const size_t& max_Wieght ) : currentWeight(0), maxWieght(max_Wieght)
		{
			
		}
		~Segment()
		{
			for( auto& bags : bags_group )
				bags.clear();
			bags_group.clear();
		}
	}

	std::unordered_map<std::string, int> class_segment;

	void init_class_segment( std::istream& is )
	{
		std::string line;
		in >> line >> class_segment[line];
		in >> line >> class_segment[line];
		in >> line >> class_segment[line];
	}

	std::unordered_map<std::string, >

public:
    Plane() : class_segment({ { "FIRST_CLASS_SEGMENT", 0 }, { "BUSINESS_CLASS_SEGMENT", 0 },
            { "ECONOMY_CLASS_SEGMENT", 0 } }) {}
    ~Plane() {}

    int load( std::istream& in, std::ostream& out )
    {
		init_class_segment(in);

		return 0;
    }
};

int main()
{
    Plane plane;
    std::stringstream input;
    input << "FIRST_CLASS_SEGMENT 1000" << std::endl;
    input << "BUSINESS_CLASS_SEGMENT 2000" << std::endl;
    input << "ECONOMY_CLASS_SEGMENT 3000" << std::endl;
    input << "PILOT 0 0" << std::endl;
    input << "FLIGHT_ATTENDANT 0 0" << std::endl;
    input << "ECONOMY 5 20" << std::endl;
    input << "BUSINESS 0 10 34" << std::endl;
    input << "FIRST_CLASS 30 30 40 20" << std::endl;
    input << "FIRST_CLASS 0 0 50 40" << std::endl;
    return plane.load( input, std::cout );
}
