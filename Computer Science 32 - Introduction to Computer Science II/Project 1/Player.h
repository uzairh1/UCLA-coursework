#ifndef PLAYER
#define PLAYER
class Room;
class Player
{
public:
    // Constructor
    Player(Room* rp, int r, int c);

    // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;

    // Mutators
    void stand();
    void moveOrSwat(int dir);
    void setDead();

private:
    Room* m_room;
    int   m_row;
    int   m_col;
    int   m_age;
    bool  m_dead;
};
#endif