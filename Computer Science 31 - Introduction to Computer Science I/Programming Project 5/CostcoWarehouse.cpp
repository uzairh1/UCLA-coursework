#include <string>
#include <iostream>
#include "CostcoMember.h"
#include "CostcoWarehouse.h"
using namespace std;

CostcoWarehouse::CostcoWarehouse(std::string location, int number)
{
	mLocation = location;
	mNumber = number;
}
int CostcoWarehouse::getNumber()
{
	return mNumber;
}
string CostcoWarehouse::getLocation()
{
	return mLocation;
}
void CostcoWarehouse::shopForStuff(CostcoMember &member, double amount)
{
	member.incrementPurchaseTotal(amount);
	member.incrementTripCount();
}
void CostcoWarehouse::returnStuff(CostcoMember& member, double amount)
{
	member.decrementPurchaseTotal(amount);
}
double CostcoWarehouse::calculateReward(CostcoMember member)
{
	
	if (member.getKind() == CostcoMember::Kind::BUSINESS || (member.getKind() == CostcoMember::Kind::GOLDSTAR))
	{
		return 0;
	}
	else
	{
		return 0.02 * member.getPurchaseTotal();
	}
}

double CostcoWarehouse::issueReward(CostcoMember& member)
{
	
	if (member.getKind() == CostcoMember::Kind::BUSINESS || (member.getKind() == CostcoMember::Kind::GOLDSTAR))
	{
		return 0;
	}
	else
	{
		double reward = 0;
		reward = 0.02 * member.getPurchaseTotal();
		if (reward > 0)
		{
			member.clearPurchaseTotal();
		}
		return reward;
		
	}

}

