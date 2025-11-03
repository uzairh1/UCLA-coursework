#include <string>
#include <iostream>
#include "CostcoMember.h"
using namespace std;

CostcoMember::CostcoMember(string name, string number, CostcoMember::Kind kind)
{
	mName = name;
	mNumber = number;
	mKind = kind;
}
string CostcoMember::getName()
{
	return mName;

}
string CostcoMember::getNumber()
{
	return mNumber;
}
CostcoMember::Kind CostcoMember::getKind()
{
	return mKind;
}
int CostcoMember::getTripCount()
{
	return mTripCount;
}
void CostcoMember::incrementTripCount()
{
	mTripCount++;
}
void CostcoMember::clearTripCount()
{
	mTripCount = 0;
}
double CostcoMember::getPurchaseTotal()
{
	return mPurchaseTotal;
}
void CostcoMember::clearPurchaseTotal()
{
	mPurchaseTotal = 0;
}
void CostcoMember::incrementPurchaseTotal(double amount) 
{
	mPurchaseTotal += amount;

}
void CostcoMember::decrementPurchaseTotal(double amount)
{
	mPurchaseTotal -= amount;
}
