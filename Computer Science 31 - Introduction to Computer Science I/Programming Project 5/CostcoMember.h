#include <string>
#ifndef COSTCOMEMBER_H
#define COSTCOMEMBER_H
class CostcoMember {
public:
	enum class Kind { BUSINESS, EXECUTIVE, GOLDSTAR};
	CostcoMember(std::string name, std::string number, CostcoMember::Kind kind);
	std::string getName();
	std::string getNumber();
	CostcoMember::Kind getKind();
	int getTripCount();
	void incrementTripCount();
	void clearTripCount();
	double getPurchaseTotal();
	void incrementPurchaseTotal(double amount);
	void decrementPurchaseTotal(double amount);
	void clearPurchaseTotal();
private:
	std::string mName;
	std::string mNumber;
	Kind mKind;
	int mTripCount = 0;
	double mPurchaseTotal = 0;

};
#endif