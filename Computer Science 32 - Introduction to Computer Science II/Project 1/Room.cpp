#include "Room.h"
#include "Bee.h"
#include "Player.h"
#include <iostream>
#include <cstdlib>
using namespace std;

History& Room::history()
{
    History* ptr = &mRoomHistory;
    return *ptr; 
}

Room::Room(int nRows, int nCols) : mRoomHistory(nRows, nCols) 
{
    if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
    {
        cout << "***** Room created with invalid size " << nRows << " by "
            << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nBees = 0;
}

Room::~Room()
{
    for (int k = 0; k < m_nBees; k++)
        delete m_bees[k];
    delete m_player;
}

int Room::rows() const
{
    return m_rows;
}

int Room::cols() const
{
    return m_cols;
}

Player* Room::player() const
{
    return m_player;
}

int Room::beeCount() const
{
    return m_nBees;
}

int Room::numBeesAt(int r, int c) const
{
    int count = 0;
    for (int k = 0; k < m_nBees; k++)
    {
        const Bee* bp = m_bees[k];
        if (bp->row() == r && bp->col() == c)
            count++;
    }
    return count;
}

bool Room::determineNewPosition(int& r, int& c, int dir) const
{
    switch (dir)
    {
    case UP:     if (r <= 1)      return false; else r--; break;
    case DOWN:   if (r >= rows()) return false; else r++; break;
    case LEFT:   if (c <= 1)      return false; else c--; break;
    case RIGHT:  if (c >= cols()) return false; else c++; break;
    default:     return false;
    }
    return true;
}

void Room::display() const
{
    // Position (row,col) of room coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;

    // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';

    // Indicate each bee's position
    for (int k = 0; k < m_nBees; k++)
    {
        const Bee* bp = m_bees[k];
        char& gridChar = grid[bp->row() - 1][bp->col() - 1];
        switch (gridChar)
        {
        case '.':  gridChar = 'B'; break;
        case 'B':  gridChar = '2'; break;
        case '9':  break;
        default:   gridChar++; break;  // '2' through '8'
        }
    }

    // Indicate player's position
    if (m_player != nullptr)
    {
        // Set the char to '@', unless there's also a bee there,
        // in which case set it to '*'.
        char& gridChar = grid[m_player->row() - 1][m_player->col() - 1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }

    // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

    // Write message, bee, and player info
    cout << endl;
    cout << "There are " << beeCount() << " bees remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Room::addBee(int r, int c)
{
    // Dynamically allocate a new Bee and add it to the room
    if (m_nBees == MAXBEES)
        return false;
    m_bees[m_nBees] = new Bee(this, r, c);
    m_nBees++;
    return true;
}

bool Room::addPlayer(int r, int c)
{
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

    // Dynamically allocate a new Player and add it to the room
    m_player = new Player(this, r, c);
    return true;
}

bool Room::swatBeeAt(int r, int c, int dir)
{
    // Swat one bee.  Returns true if a bee was swatted and destroyed,
    // false otherwise (no bee there, or the swat did not destroy the
    // bee).
    int k = 0;
    for (; k < m_nBees; k++)
    {
        if (m_bees[k]->row() == r && m_bees[k]->col() == c)
            break;
    }
    if (k < m_nBees && m_bees[k]->getSwatted(dir))  // bee dies
    {
        delete m_bees[k];
        m_bees[k] = m_bees[m_nBees - 1];
        m_nBees--;
        return true;
    }
    return false;
}

bool Room::moveBees()
{
    for (int k = 0; k < m_nBees; k++)
    {
        Bee* bp = m_bees[k];
        bp->move();
        if (bp->row() == m_player->row() && bp->col() == m_player->col())
            m_player->setDead();
    }

    // return true if the player is still alive, false otherwise
    return !m_player->isDead();
}