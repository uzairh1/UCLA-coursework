#include<iostream>
#include<string>
using namespace std;


int main()
// Initialize variables
{
	string size = "";
	string topping = "";
	string extra_topping = "";
	string method = "";
	int amount = 0;
	int cost = 0;

	// Input/output and error handling
	cout << "How many pizzas do you want to order? ";
	cin >> amount;
	if (amount <= 0)
	{
		cout << "How many value must be greater than 0!";
		return -1;
	}
	cin.ignore(10000, '\n'); // to be able to read in a string after a number correctly
	cout << "What size? ";
	getline(cin, size);
	if ((size != "Small") && (size != "Medium") && (size != "Large"))
	{
		cout << "Size value must be either Small, Medium or Large!";
		return -2;
	}
	cout << "Topping? ";
	getline(cin, topping);
	if ((topping != "Pepperoni") && (topping != "Mushroom") && (topping != "Cheese"))
	{
		cout << "Topping value must be either Cheese, Pepperoni or Mushroom!";
		return -3;
	}
	cout << "Extra topping? ";
	getline(cin, extra_topping);
	if ((extra_topping != "Yes") && (extra_topping != "No"))
	{
		cout << "Extra topping value must be either Yes or No!";
		return -4;
	}
	cout << "Takeout or Delivery? ";
	getline(cin, method);
	if ((method != "Takeout") && (method != "Delivery"))
	{
		cout << "Where value must be either Takeout or Delivery!";
		return -5;
	}

	// Cost calculations

	// size

	if (size == "Small")
	{
		cost = 4 * amount;
	}
	else if (size == "Medium")
	{
		cost = 6 * amount;
	}
	else if (size == "Large")
	{
		cost = 8 * amount;
	}

	// topping

	if ((size == "Small") && (topping == "Pepperoni") && (extra_topping == "No"))
	{
		cost = cost + (2 * amount);
	}
	if ((size == "Small") && (topping == "Mushroom") && (extra_topping == "No"))
	{
		cost = cost + (1 * amount);
	}
	if ((size == "Medium") && (topping == "Pepperoni") && (extra_topping == "No"))
	{
		cost = cost + (4 * amount);
	}
	if ((size == "Medium") && (topping == "Mushroom") && (extra_topping == "No"))
	{
		cost = cost + (2 * amount);
	}
	if ((size == "Large") && (topping == "Pepperoni") && (extra_topping == "No"))
	{
		cost = cost + (6 * amount);
	}
	if ((size == "Large") && (topping == "Mushroom") && (extra_topping == "No"))
	{
		cost = cost + (3 * amount);
	}

	// extra topping


	if ((size == "Small") && (topping == "Pepperoni") && (extra_topping == "Yes"))
	{
		cost = cost + (4 * amount);
	}
	if ((size == "Medium") && (topping == "Pepperoni") && (extra_topping == "Yes"))
	{
		cost = cost + (8 * amount);
	}
	if ((size == "Large") && (topping == "Pepperoni") && (extra_topping == "Yes"))
	{
		cost = cost + (12 * amount);
	}

	if ((size == "Small") && (topping == "Mushroom") && (extra_topping == "Yes"))
	{
		cost = cost + (2 * amount);
	}
	if ((size == "Medium") && (topping == "Mushroom") && (extra_topping == "Yes"))
	{
		cost = cost + (4 * amount);
	}
	if ((size == "Large") && (topping == "Mushroom") && (extra_topping == "Yes"))
	{
		cost = cost + (6 * amount);
	}

	// takeout vs. delivery
	if ((cost < 30) && method == "Delivery")
	{
		cost = cost + 4;
	}


	// Final output of cost

	cout << "Your Pizza order totals $" << cost << ". Enjoy!";
	return 0;

}
