#include <iostream>
#include <string>

#define S_NAME field

#define NAME(N) #N
#define IS(name) ( name == NAME(s.S_NAME) )

#define CAT(x, y) x##y

struct S 
{
    int S_NAME = 0;
};

S Deserialize(const std::string& name, int value)
{
    // if name == "field" => return S{value};
    // else => return S{0};
    S s;

    CAT(s,S_NAME);

    if( IS(name) )
    {
        s.field = value;
        return s;
    }
    return s;
}

std::ostream& operator<<(std::ostream& os, const S& rhs)
{
    os << rhs.field;
    return os;
}

int main()
{
    std::cout << Deserialize("field", 4) << std::endl;
    std::cout << Deserialize("field1", 4) << std::endl;
    return 0;
}


