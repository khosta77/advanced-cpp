#ifndef UNITS_PLANE_HPP_
#define UNITS_PLANE_HPP_

#include <iostream>
#include <unordered_map>
#include <sstream>
#include <list>
#include <vector>
#include <format>
#include <algorithm>

#include "unit.hpp"
#include "crew_members.hpp"
#include "passengers.hpp"

struct Segment
{
private:
    size_t currentWeight;
	size_t maxWieght;
	std::vector<std::list<Bag>> bags_group;

    bool checkWeight( const size_t& bXleft, const size_t& bXright, const Bag& bag )
    {
        if( bXleft > maxWieght and bXright <= maxWieght )
        {
            currentWeight += bag.weight;
            bags_group[( bag.weight )].push_front(bag);
            return true;
        }
        return false;
    }

    std::vector<std::pair<size_t, size_t>> fillAdjacentPairs( const size_t& neededFreeSpace )
    {
        std::vector<std::pair<size_t, size_t>> df;
        for( size_t i = 0, j = 1; i < 24; ++i, ++j )
        {
            if( bags_group[j].size() != 0 )
            {
                std::pair<size_t, size_t> x{ ( neededFreeSpace / j ), 0 };
                if( ( neededFreeSpace % j ) )
                    ++x.first;
                x.second = j;
                df.push_back(x);
            }
        }
        // Так как максимальное колличество элементов в векторе 24 - это
        // константая сложность
        std::sort( df.begin(), df.end() );
        return df;
    }
public:
    Segment() {}
    Segment( const size_t& max_Wieght ) : currentWeight(0), maxWieght(max_Wieght),
        bags_group(( maxWieght + 1 )) {}
	~Segment()
	{
	    for( auto& bags : bags_group )
			bags.clear();
		bags_group.clear();
	}

    std::pair<bool, bool> append( const std::array<Bag, 4>& bags )
    {
        size_t bX1 = ( currentWeight + bags[2].weight );
        size_t bX2 = ( currentWeight + bags[3].weight );
        size_t bXs = ( currentWeight + bags[2].weight + bags[3].weight );

        if( ( bXs > maxWieght ) and ( ( bX1 > maxWieght ) and ( bX2 > maxWieght ) ) )
            return std::pair<bool, bool>{ false, false };

        if( checkWeight( bX1, bX2, bags[3] ) )
            return std::pair<bool, bool>{ false, true };

        if( checkWeight( bX2, bX1, bags[2] ) )
            return std::pair<bool, bool>{ false, true };

        currentWeight += ( bags[2].weight + bags[3].weight );
        bags_group[bags[2].weight].push_front(bags[2]);
        bags_group[bags[3].weight].push_front(bags[3]);
        return std::pair<bool, bool>{ true, true };
    }

    void findFreeSpace( const Bag& bag, std::ostream& out )
    {
        const size_t w = bag.weight;
        size_t neededFreeSpace = ( currentWeight - (maxWieght - w) );

        auto df = fillAdjacentPairs(neededFreeSpace);
        for(const auto& [first, second] : df)
        {
            for( size_t i = 0; i < first; ++i )
            {
                if( bags_group[second].empty() )
                    break;

                if( neededFreeSpace <= maxWieght )
                {
                    neededFreeSpace -= second;
                    currentWeight -= second;
                    out << std::format( "!!PASSENGER’S LUGGAGE REMOVED FROM FLIGHT, ID = {}!!\n",
                                        bags_group[second].front().passengerId );
                    bags_group[second].pop_front();
                }
            }

            if( neededFreeSpace > maxWieght )
                break;
        }
        df.clear();
        currentWeight += w;
        bags_group[w].push_back(bag);
    }
};

class Plane
{
private:
	std::unordered_map<std::string, Segment> class_segment;
    const std::string fcs = "FIRST_CLASS_SEGMENT";
    const std::string bcs = "BUSINESS_CLASS_SEGMENT";
    const std::string ecs = "ECONOMY_CLASS_SEGMENT";

	void init_class_segment( std::istream& in )
	{
		std::string line;
        for( size_t i = 0, j = 0; ( i < 3 ) and (in >> line >> j); ++i )
            class_segment.emplace( line, j );
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

    size_t MAX_PASSANGER;
    std::vector<Passenger*> thePlane;

    std::unordered_map<std::string, std::pair<size_t, size_t>> passenger_size;
#if 0
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
#endif
    size_t lastId;

    template<typename T>
    bool toRegister( std::ostream& out, const std::string& type, const size_t& id,
                     const std::array<Bag, 4>& bags, const std::string& cs )
    {
        Passenger* passenger = new T( id, bags );
        if( !passenger->WillWeBeAbleToRegisterIt() )
        {
            delete passenger;
            out << std::format( "!!CANT REGISTER {} PASSENGER, ID = {}!!\n", type, id );
            return false;
        }
        std::pair<bool, bool> bool_p = class_segment[cs].append( bags );
        if( bool_p.first and bool_p.second )
            return true;

        if( bool_p.first and !bool_p.second and passenger->name == "ECONOMY" )
            return true;

        if( !bool_p.first )
            class_segment[ecs].findFreeSpace(bags[2], out);

        if( !bool_p.second )
            class_segment[ecs].findFreeSpace(bags[3], out);

        return true;
    }

    bool isRange(const std::string& type)
    {
        return ( passenger_size[type].first <= passenger_size[type].second );
    }

    template<typename T>
    void put( std::ostream& out, const std::string& type, const std::string& true_type,
              const std::array<Bag, 4>& bags, const std::string& cs )
    {
        if( ( ( type == true_type ) and isRange(true_type) ) )
        {
            if( toRegister<T>( out, type, lastId, bags, cs ) )
                ++passenger_size[true_type].first;
        }
    }

    void passengerUpBoard( std::ostream& out, const std::vector<std::string>& passenger )
    {
        if( thePlane.size() > MAX_PASSANGER )
        {
            out << std::format( "!!CANT REGISTER {} PASSENGER, ID = {}!!\n", passenger[0], lastId );
            return;
        }
        auto bags = unpackBagsFromLine( lastId, passenger );
        put<Pilot>( out, passenger[0], "PILOT", bags, ecs );
        put<FlightAttendant>( out, passenger[0], "FLIGHT_ATTENDANT", bags, ecs );
        put<Economy>( out, passenger[0], "ECONOMY", bags, ecs );
        put<Business>( out, passenger[0], "BUSINESS", bags, bcs );
        put<FirstClass>( out, passenger[0], "FIRST_CLASS", bags, fcs );
    }

    void boarding( std::istream& in, std::ostream& out )
    {
        std::string line;
        in.ignore();
        while( std::getline( in, line ) )
        {
            ++lastId;
            std::vector<std::string> passenger = split( line, ' ' );
            passengerUpBoard( out, passenger );
        }
    }

    void calcMAX_PASSENGER()
    {
        MAX_PASSANGER = 0;
        for( const auto& [key, value] : passenger_size )
            MAX_PASSANGER += value.second;
    }

public:
    Plane() : passenger_size({
            { "PILOT",            { 0, 2 } },
            { "FLIGHT_ATTENDANT", { 0, 6 } },
            { "ECONOMY",          { 0, 200 } },
            { "BUSINESS",         { 0, 10 } },
            { "FIRST_CLASS",      { 0, 4 } }, }), lastId(0) { calcMAX_PASSENGER(); }

    ~Plane()
    {
        for( auto& passenger : thePlane )
            delete passenger;
        thePlane.clear();
    }

    int load( std::istream& in, std::ostream& out )
    {
		init_class_segment(in);
        boarding( in, out );
		return 0;
    }
};

#endif  // UNITS_PLANE_HPP_
