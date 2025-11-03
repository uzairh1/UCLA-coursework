#include <string>
#include <iostream>
using namespace std;
#include "CostcoMember.h"
#include "CostcoWarehouse.h"
#include <cassert>

int main()
{
    // sample test code working with CostcoMember and CostcoWarehouse
    CostcoMember howard("Howard", "123456", CostcoMember::Kind::EXECUTIVE);
    CostcoWarehouse warehouse("Culver City", 210);

    assert(howard.getName() == "Howard");
    assert(howard.getNumber() == "123456");
    assert(howard.getKind() == CostcoMember::Kind::EXECUTIVE);
    assert(howard.getTripCount() == 0);

    // since doubles are inexact, you need to stringify them to assert their value
    // by doing so, you will always get six digits after the decimal point,
    // leaving off the rounding error that might be present
    assert(std::to_string(howard.getPurchaseTotal()) == "0.000000");

    assert(warehouse.getLocation() == "Culver City");
    assert(warehouse.getNumber() == 210);
    assert(std::to_string(warehouse.calculateReward(howard)) == "0.000000");
    warehouse.shopForStuff(howard, 100);
    assert(std::to_string(howard.getPurchaseTotal()) == "100.000000"); 
    assert(howard.getTripCount() == 1);
    warehouse.shopForStuff(howard, 1000);
    warehouse.returnStuff(howard, 100);
    assert(std::to_string(howard.getPurchaseTotal()) == "1000.000000");
    assert(howard.getTripCount() == 2);
    assert(std::to_string(warehouse.calculateReward(howard)) == "20.000000");
    assert(std::to_string(warehouse.issueReward(howard)) == "20.000000"); 
    assert(std::to_string(howard.getPurchaseTotal()) == "0.000000");
    assert(howard.getTripCount() == 2);

    cout << "all tests passed!" << endl;
    return(0);
}

