#include <iostream>
#include <string>
#include <map>
#include <cassert>
#include <sstream>
#include <set>
#include <list>
#include <format>

struct Bag
{
	size_t weight;
	size_t passengerId;

	Bag( const size_t& w = 0, const size_t& id = 0 ) : weight(w), passengerId(id) {}
    Bag(const Bag& other) : weight(other.weight), passengerId(other.passengerId) {}

    // Определение оператора присваивания
    Bag& operator=( const Bag& other )
    {
        if( this != &other )
        {
            weight = other.weight;
            passengerId = other.passengerId;
        }
        return *this;
    }
};

std::ostream& operator<<( std::ostream& os, const Bag& bag )
{
    os << '(' << bag.weight << ", " << bag.passengerId << ')';
    return os;
}

std::ostream& operator<<( std::ostream& os, const std::array<Bag, 4>& bags )
{
    for( const auto& bag : bags )
        os << bag << ' ';
    return os;
}

class Passenger
{
private:
    bool check( const size_t& start, const size_t& end, const size_t& value )
    {
        for( size_t i = start; i < end; ++i )
        {
            if( bags[i].weight > value )
                return false;
        }
        return true;
    }

protected:
    size_t id;
    std::array<Bag, 4> bags;

    size_t HandBag;
    size_t HandBagValue;

    size_t _bag;
    size_t _bagValue;

    std::string name;

public:
    Passenger( const size_t& _id, const std::array<Bag, 4>& _bags, const size_t& mHB, const size_t& mHBV,
            const size_t& mB, const size_t& mBV, const std::string& _name = "" ) : id(_id), bags(_bags),
            HandBag(mHB), HandBagValue(mHBV), _bag(mB), _bagValue(mBV), name(_name) {} 
    ~Passenger()
    {
        name.clear();
    }

    bool WillWeBeAbleToRegisterIt()
    {
        if( !( this->check( 0, 2, HandBagValue ) and this->check( 2, 4, _bagValue ) ) )
            return false;

        if( ( ( HandBagValue == 0 ) and ( _bagValue == 0 ) ) )
            return true;

        if( HandBag == 1 )
            return !( ( bags[1].weight > 0 ) or ( bags[3].weight > 0 ) );
        return true;
    }
};

class Pilot : public Passenger
{
public:
    Pilot( const size_t& _id, const std::array<Bag, 4>& _bags ) : Passenger( _id, _bags, 2, 0, 2, 0,
            "PILOT" ) {}
};

class FlightAttendant : public Passenger
{
public:
    FlightAttendant( const size_t& _id, const std::array<Bag, 4>& _bags ) : Passenger( _id, _bags, 2, 0,
            2, 0, "FLIGHT_ATTENDANT" ) {}
};

class FirstClass : public Passenger
{
public:
    FirstClass( const size_t& _id, const std::array<Bag, 4>& _bags ) : Passenger( _id, _bags, 2, 60, 2,
            -1, "FIRST_CLASS" ) {}
};

class Business : public Passenger
{
public:
    Business( const size_t& _id, const std::array<Bag, 4>& _bags ) : Passenger( _id, _bags, 2, 24, 2,
            40, "BUSINESS" ) {}
};

class Economy : public Passenger
{
public:
    Economy( const size_t& _id, const std::array<Bag, 4>& _bags ) : Passenger( _id, _bags, 1, 10, 1,
            24, "ECONOMY" ) {}
};

class Plane
{
private:
    using BoolPair = std::pair<bool, bool>;

	struct Segment
	{
		size_t currentWeight;
		size_t maxWieght;
		std::vector<std::list<Bag>> bags_group;

        Segment() {}
		Segment( const size_t& max_Wieght ) : currentWeight(0), maxWieght(max_Wieght),
            bags_group(( maxWieght + 1 )) {}
		~Segment()
		{
			for( auto& bags : bags_group )
				bags.clear();
			bags_group.clear();
		}

        BoolPair append( const std::array<Bag, 4>& bags )
        {
            size_t bX1 = ( currentWeight + bags[2].weight );
            size_t bX2 = ( currentWeight + bags[3].weight );
            size_t bXs = ( currentWeight + bags[2].weight + bags[3].weight );

            if( ( bXs > maxWieght ) and ( ( bX1 > maxWieght ) and ( bX2 > maxWieght ) ) )
                return BoolPair{ false, false };

            if( bX1 > maxWieght and bX2 <= maxWieght )
            {
                currentWeight += bags[3].weight;
                bags_group[( bags[3].weight )].push_front(bags[3]);
                return BoolPair{ false, true };
            }

            if( bX2 > maxWieght and bX1 <= maxWieght )
            {
                currentWeight += bags[2].weight;
                bags_group[( bags[2].weight + 1 )].push_front(bags[2]);
                return BoolPair{ true, false };
            }

            currentWeight += ( bags[2].weight + bags[3].weight );
            bags_group[bags[2].weight].push_front(bags[2]);
            bags_group[bags[3].weight].push_front(bags[3]);
            return BoolPair{ true, true };
        }
	};

	std::unordered_map<std::string, Segment> class_segment;
    const std::string fcs = "FIRST_CLASS_SEGMENT";
    const std::string bcs = "BUSINESS_CLASS_SEGMENT";
    const std::string ecs = "ECONOMY_CLASS_SEGMENT";

	void init_class_segment( std::istream& in )
	{
		std::string line;
        for( size_t i = 0, max_Wieght = 0; i < 3; ++i, max_Wieght = 0 )
        {
		    in >> line >> max_Wieght;
            class_segment.insert( std::pair<std::string, Segment>{ line, Segment(max_Wieght) } );
        }
	}

    std::vector<std::string> split(const std::string &content, const char del) {
        std::vector<std::string> arrayString;
        std::string buffer = "";
        for (size_t i = 0, I = content.size(); i < I; ++i) {
            if (content[i] != del)
                buffer += content[i];
            else {
                arrayString.push_back(buffer);
                buffer = "";
            }
        }
        arrayString.push_back(buffer);
        buffer.clear();
        return arrayString;
    }

    std::array<Bag, 4> unpackBagsFromLine( const size_t& id, const std::vector<std::string>& words )
    {
        std::array<Bag, 4> bags = { Bag( 0, id ), Bag( 0, id ), Bag( 0, id ), Bag( 0, id ) };
        size_t size = words.size();
        if( size == 3 )
        {
            bags[0].weight = std::stoi(words[1]);
            bags[2].weight = std::stoi(words[2]);
        }
        else if( size == 4 )
        {
            bags[0].weight = std::stoi(words[1]);
            bags[1].weight = std::stoi(words[2]);
            bags[2].weight = std::stoi(words[3]);            
        }
        else
        {
            bags[0].weight = std::stoi(words[1]);
            bags[1].weight = std::stoi(words[2]);
            bags[2].weight = std::stoi(words[3]);
            bags[3].weight = std::stoi(words[4]);            
        }
        return bags;
    }

    const size_t MAX_PASSANGER = 222;
    std::vector<Passenger*> thePlane;

    const size_t MAX_PILOT_PASSANGER = 2;
    size_t pilot_passanger = 0;

    const size_t MAX_FLIGHT_ATTENDANT_PASSANGER = 6;
    size_t flight_attendant_passanger = 0;

    const size_t MAX_FIRST_CLASS_PASSANGER = 4;
    size_t first_class_passanger = 0;

    const size_t MAX_BUSINESS_PASSANGER = 10;
    size_t business_passanger = 0;

    const size_t MAX_ECONOMY_PASSANGER = 200;
    size_t economy_passanger = 0;

    size_t lastId;

    template<typename T>
    bool toRegister( std::ostream& out, const std::string& type, const size_t& id, const std::array<Bag, 4>& bags,
            const std::string& cs )
    {
        Passenger* passenger = new T( id, bags );
        if( !passenger->WillWeBeAbleToRegisterIt() )
        {
            delete passenger;
            out << std::format( "!!CANT REGISTER {} PASSENGER, ID = {}!!\n", type, id );
            return false;
        }
        BoolPair bool_p = class_segment[cs].append( bags );
        if( bool_p.first and bool_p.second )
            return true;

        if( bool_p.first and !bool_p.second and passenger.name == "ECONOMY" )
            return true;

        return true;
    }

    void boarding( std::istream& in, std::ostream& out )
    {
        std::string line;
        in.ignore();
        while( std::getline( in, line ) )
        {
            ++lastId;
            std::vector<std::string> passenger = split( line, ' ' );

            if( thePlane.size() > MAX_PASSANGER )
            {
                out << std::format( "!!CANT REGISTER {} PASSENGER, ID = {}!!\n", passenger[0], lastId );
                continue;
            }

            auto bags = unpackBagsFromLine( lastId, passenger );

            if( ( ( passenger[0] == "PILOT" ) and ( pilot_passanger <= MAX_PILOT_PASSANGER ) ) )
            {
                if( toRegister<Pilot>( out, passenger[0], lastId, bags, ecs ) )
                    ++pilot_passanger;
                continue;
            }
            
            if( ( ( passenger[0] == "FLIGHT_ATTENDANT" ) and
                        ( flight_attendant_passanger <= MAX_FLIGHT_ATTENDANT_PASSANGER ) ) )
            {
                if( toRegister<FlightAttendant>( out, passenger[0], lastId, bags, ecs ) )
                    ++flight_attendant_passanger;
                continue;
            }
            
            if( ( ( passenger[0] == "ECONOMY" ) and ( economy_passanger <= MAX_ECONOMY_PASSANGER ) ) )
            {
                if( toRegister<Economy>( out, passenger[0], lastId, bags, ecs ) )
                    ++economy_passanger;
                continue;
            }
            
            if( ( ( passenger[0] == "BUSINESS" ) and ( business_passanger <= MAX_BUSINESS_PASSANGER ) ) )
            {
                if( toRegister<Business>( out, passenger[0], lastId, bags, bcs ) )
                    ++business_passanger;
                continue;
            }
            
            if( ( ( passenger[0] == "FIRST_CLASS" ) and ( first_class_passanger <= MAX_FIRST_CLASS_PASSANGER ) ) )
            {
                if( toRegister<FirstClass>( out, passenger[0], lastId, bags, fcs ) )
                    ++first_class_passanger;
                continue;
            }
            out << std::format( "!!CANT REGISTER {} PASSENGER, ID = {}!!\n", passenger[0], lastId );
        }
    }

public:
    Plane() : lastId(0) {}
    ~Plane() {}

    int load( std::istream& in, std::ostream& out )
    {
		init_class_segment(in);
        boarding( in, out );
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
