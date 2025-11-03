#include "Bee.h"
#include "Room.h"
#include "Player.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Bee::Bee(Room* rp, int r, int c)
{
    if (rp == nullptr)
    {
        cout << "***** A bee must be created in some Room!" << endl;
        exit(1);
    }
    if (r < 1 || r > rp->rows() || c < 1 || c > rp->cols())
    {
        cout << "***** Bee created with invalid coordinates (" << r << ","
            << c << ")!" << endl;
        exit(1);
    }
    m_room = rp;
    m_row = r;
    m_col = c;
    m_health = INITIAL_BEE_HEALTH;
}

int Bee::row() const
{
    return m_row;
}

int Bee::col() const
{
    return m_col;
}

void Bee::move()
{
    // Attempt to move in random direction; if bee can't move, don't move
    int dir = randInt(0, NUMDIRS - 1);  // dir is now UP, DOWN, LEFT, or RIGHT
    m_room->determineNewPosition(m_row, m_col, dir);
}

bool Bee::getSwatted(int dir)
{
    // return true if dies
    m_health--;
    if (m_health == 0)
    {
        int c = m_room->player()->col();
        int r = m_room->player()->row();
        m_room->history().record(r, c);
        return true;
    }
    if (!m_room->determineNewPosition(m_row, m_col, dir))
    {
        m_health = 0;
        int c1 = m_room->player()->col();
        int r1 = m_room->player()->row();
        m_room->history().record(r1, c1);
        return true;
    }
    return false;
}