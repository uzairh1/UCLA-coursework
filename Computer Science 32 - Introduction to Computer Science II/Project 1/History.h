#ifndef HISTORY
#define HISTORY
class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
	~History();
private:
	int mRoomRows;
	int mRoomColumns;
	int mRPlayerPosition;
	int mCPlayerPosition;
	int** table;
	char translatetoLetter(int a) const;
};
#endif