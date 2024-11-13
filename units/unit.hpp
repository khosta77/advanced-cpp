#ifndef UNITS_UNIT_HPP_
#define UNITS_UNIT_HPP_

#include <string>
#include <array>

struct Bag
{
	size_t weight;
	size_t passengerId;

	Bag( const size_t& w = 0, const size_t& id = 0 ) : weight(w), passengerId(id) {}
    Bag(const Bag& other) : weight(other.weight), passengerId(other.passengerId) {}
    ~Bag() = default;
    Bag& operator=( const Bag& other )
    {
        if( this != &other )
        {
            weight = other.weight;
            passengerId = other.passengerId;
        }
        return *this;
    }

    Bag( Bag&& ) = delete;
    Bag& operator=( const Bag&& other ) = delete;
};

class Passenger
{
private:

    size_t _id;
    std::array<Bag, 4> _bags;

    size_t _handBag;
    size_t _handBagValue;

    size_t _bag;
    size_t _bagValue;

    bool check( const size_t& start, const size_t& end, const size_t& value )
    {
        for( size_t i = start; i < end; ++i )
        {
            if( _bags[i].weight > value )
                return false;
        }
        return true;
    }

public:
    const std::string name;

    Passenger() = delete;
    Passenger( const Passenger& ) = delete;
    Passenger( Passenger&& ) = delete;

    Passenger( const size_t& id, const std::array<Bag, 4>& bags, const size_t& handBag, const size_t& handBagValue,
            const size_t& bag, const size_t& bagValue, const std::string& _name = "" ) : _id(id), _bags(bags),
            _handBag(handBag), _handBagValue(handBagValue), _bag(bag), _bagValue(bagValue), name(_name) {}
    ~Passenger() {}

    bool WillWeBeAbleToRegisterIt()
    {
        if( !( this->check( 0, 2, _handBagValue ) and this->check( 2, 4, _bagValue ) ) )
            return false;

        if( ( ( _handBagValue == 0 ) and ( _bagValue == 0 ) ) )
            return true;

        if( ( ( _handBag == 1 ) and ( _handBag == _bag ) ) )
            return !( ( _bags[1].weight > 0 ) or ( _bags[3].weight > 0 ) );
        return true;
    }
};

#endif  // UNITS_UNIT_HPP_
