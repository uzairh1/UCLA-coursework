#ifndef ROOM
#define ROOM
#include "globals.h"
#include "History.h"
class Bee;
class Player;
class Room
{
public:
    // Constructor/destructor
    Room(int nRows, int nCols);
    ~Room();

    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     beeCount() const;
    int     numBeesAt(int r, int c) const;
    bool    determineNewPosition(int& r, int& c, int dir) const;
    void    display() const;
    History& history();

    // Mutators
    bool addBee(int r, int c);
    bool addPlayer(int r, int c);
    bool swatBeeAt(int r, int c, int dir);
    bool moveBees();

private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Bee* m_bees[MAXBEES];
    int     m_nBees;
    History mRoomHistory;
};
#endif