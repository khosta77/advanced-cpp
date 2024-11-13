#include "passengers.hpp"
#include <cassert>

void test() {
  std::array<Bag, 4> bags_first = {Bag(60, 20), Bag(60, 20), Bag(200, 20),
                                   Bag(100, 20)};
  std::array<Bag, 4> bags_business = {Bag(24, 12), Bag(24, 12), Bag(40, 12),
                                      Bag(40, 12)};
  std::array<Bag, 4> bags_econom = {Bag(10, 13), Bag(0, 12), Bag(20, 13),
                                    Bag(0, 13)};

  Passenger *first_class01 = new FirstClass(20, bags_first);
  Passenger *first_class02 = new FirstClass(12, bags_business);
  Passenger *first_class03 = new FirstClass(13, bags_econom);

  assert(first_class01->name == "FIRST_CLASS");
  assert(first_class02->name == "FIRST_CLASS");
  assert(first_class03->name == "FIRST_CLASS");

  assert(first_class01->WillWeBeAbleToRegisterIt());
  assert(first_class02->WillWeBeAbleToRegisterIt());
  assert(first_class03->WillWeBeAbleToRegisterIt());

  delete first_class01;
  delete first_class02;
  delete first_class03;

  Passenger *business_class01 = new Business(20, bags_first);
  Passenger *business_class02 = new Business(12, bags_business);
  Passenger *business_class03 = new Business(13, bags_econom);

  assert(business_class01->name == "BUSINESS");
  assert(business_class02->name == "BUSINESS");
  assert(business_class03->name == "BUSINESS");

  assert(!business_class01->WillWeBeAbleToRegisterIt());
  assert(business_class02->WillWeBeAbleToRegisterIt());
  assert(business_class03->WillWeBeAbleToRegisterIt());

  delete business_class01;
  delete business_class02;
  delete business_class03;

  Passenger *economy_class01 = new Economy(20, bags_first);
  Passenger *economy_class02 = new Economy(12, bags_business);
  Passenger *economy_class03 = new Economy(13, bags_econom);

  assert(economy_class01->name == "ECONOMY");
  assert(economy_class02->name == "ECONOMY");
  assert(economy_class03->name == "ECONOMY");

  assert(!economy_class01->WillWeBeAbleToRegisterIt());
  assert(!economy_class02->WillWeBeAbleToRegisterIt());
  assert(economy_class03->WillWeBeAbleToRegisterIt());

  delete economy_class01;
  delete economy_class02;
  delete economy_class03;
}

int main() {
  test();
  return 0;
}
