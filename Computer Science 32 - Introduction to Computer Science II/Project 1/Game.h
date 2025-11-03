#ifndef GAME
#define GAME

class Room;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nBees);
    ~Game();

    // Mutators
    void play();

private:
    Room* m_room;
};


#endif