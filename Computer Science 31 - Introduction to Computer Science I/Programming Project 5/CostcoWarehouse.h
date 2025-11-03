#include <string>
#include "CostcoMember.h"
#ifndef COSTCOWAREHOUSE_H
#define COSTCOWAREHOUSE_H
class CostcoWarehouse
{
public:
	CostcoWarehouse(std::string location, int number);
	std::string getLocation();
	int getNumber();
	void shopForStuff(CostcoMember &member, double amount);
	void returnStuff(CostcoMember& member, double amount);
	double calculateReward(CostcoMember member);
	double issueReward(CostcoMember& member);

private:
	std::string mLocation;
	int mNumber;

};
#endif