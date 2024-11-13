#include "crew_members.hpp"
#include <cassert>

void test() {
  std::array<Bag, 4> bags_not = {Bag(10, 20), Bag(20, 20), Bag(20, 20),
                                 Bag(10, 20)};
  std::array<Bag, 4> bags_yes = {Bag(0, 12), Bag(0, 12), Bag(0, 12),
                                 Bag(0, 12)}; // По условию

  Passenger *pilot01 = new Pilot(20, bags_not);
  Passenger *pilot02 = new Pilot(12, bags_yes);

  assert(pilot01->name == "PILOT");
  assert(pilot02->name == "PILOT");

  assert(!pilot01->WillWeBeAbleToRegisterIt());
  assert(pilot02->WillWeBeAbleToRegisterIt());

  delete pilot01;
  delete pilot02;

  Passenger *flight_attendant01 = new FlightAttendant(20, bags_not);
  Passenger *flight_attendant02 = new FlightAttendant(12, bags_yes);

  assert(flight_attendant01->name == "FLIGHT_ATTENDANT");
  assert(flight_attendant02->name == "FLIGHT_ATTENDANT");

  assert(!flight_attendant01->WillWeBeAbleToRegisterIt());
  assert(flight_attendant02->WillWeBeAbleToRegisterIt());

  delete flight_attendant01;
  delete flight_attendant02;
}

int main() {
  test();
  return 0;
}
