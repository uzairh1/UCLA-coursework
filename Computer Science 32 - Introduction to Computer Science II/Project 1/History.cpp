#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
	mRoomColumns = nCols;
	mRoomRows = nRows;
	mRPlayerPosition = -1;
	mCPlayerPosition = -1;
	table = new int* [mRoomRows];
	for (int i = 0; i < mRoomRows; i++)
	{
		table[i] = new int[mRoomColumns];
		for (int j = 0; j < mRoomColumns; j++)
		{
			table[i][j] = 0;
		}
	}

}
bool History::record(int r, int c)
{

	mRPlayerPosition = r;
	mCPlayerPosition = c;

	if ((r > mRoomRows) || (c > mRoomColumns) || (r < 1) || ( c < 1))
	{
		return false;
	}
	table[r-1][c-1]++;
	cerr << "Recorded at (" << r << ", " << c << "): " << table[r-1][c-1] << endl; // Debugging statement

	return true;
}

void History::display() const
{
    clearScreen();

    for (int i = 0; i < mRoomRows; i++)
    {
        for (int j = 0; j < mRoomColumns; j++)
        {
            cout << translatetoLetter(table[i][j]); 
        }
        cout << endl; 
    }
    cout << endl;
}

History::~History()
{
	for (int i = 0; i < mRoomRows; i++)
	{
		delete [] table[i];
	}
	delete [] table;
}

char History::translatetoLetter(int a) const
{
	switch (a)
	{
	case 0:
		return '.';
		break;
	case 1:
		return 'A';
		break;
	case 2:
		return 'B';
		break;
	case 3:
		return 'C';
		break;
	case 4:
		return 'D';
		break;
	case 5:
		return 'E';
		break;
	case 6:
		return 'F';
		break;
	case 7:
		return 'G';
		break;
	case 8:
		return 'H';
		break;
	case 9:
		return 'I';
		break;
	case 10:
		return 'J';
		break;
	case 11: 
		return 'K';
		break;
	case 12:
		return 'L';
		break;
	case 13:
		return 'M';
		break;
	case 14:
		return 'N';
		break;
	case 15:
		return 'O';
		break;
	case 16:
		return 'P';
		break;
	case 17:
		return 'Q';
		break;
	case 18:
		return 'R';
		break;
	case 19:
		return 'S';
		break;
	case 20:
		return 'T';
		break;
	case 21:
		return 'U';
		break;
	case 22:
		return 'V';
		break;
	case 23:
		return 'W';
		break;
	case 24:
		return 'X';
		break;
	case 25:
		return 'Y';
		break;
	default:
		return 'Z';
		break;


	}
}

