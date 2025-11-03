//
//  Code for Project 1
//  Social Media!

#include <iostream>
#include <string>
using namespace std;

int main()
{

    int totalHours = 0;
    int facebookHours = 0;
    int tiktokHours = 0;
    int instagramHours = 0;



    // prompt for social media time
    cout << "How much time did you spend on social media today? ";
    cin >> totalHours;
    cout << "How much time did you spend on Facebook? ";
    cin >> facebookHours;
    cout << "How much time did you spend on Tiktok today? ";
    cin >> tiktokHours;
    cout << "How much time did you spend on Instagram today? ";
    cin >> instagramHours;

    // is it alot??    
    if (totalHours > 4)
    {
        cout << "Wow!  That's alot!" << endl;
    }
    if (totalHours == 1)
    {
        cout << "Wow!  None!" << endl;
    }
    if (totalHours != facebookHours + tiktokHours + instagramHours)
    {
        cout << "Something went awry..." << endl; 
    }

    return(0);
}