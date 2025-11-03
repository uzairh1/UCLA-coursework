#include "Game.h"
#include "Room.h"
#include "Player.h"
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

int decodeDirection(char dir);

Game::Game(int rows, int cols, int nBees)
{
    if (nBees < 0)
    {
        cout << "***** Cannot create Game with negative number of bees!" << endl;
        exit(1);
    }
    if (nBees > MAXBEES)
    {
        cout << "***** Trying to create Game with " << nBees
            << " bees; only " << MAXBEES << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1 && cols == 1 && nBees > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the bees!" << endl;
        exit(1);
    }

    // Create room
    m_room = new Room(rows, cols);

    // Add player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_room->addPlayer(rPlayer, cPlayer);

    // Populate with bees
    while (nBees > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        // Don't put a bee where the player is
        if (r == rPlayer && c == cPlayer)
            continue;
        m_room->addBee(r, c);
        nBees--;
    }
}

Game::~Game()
{
    delete m_room;
}

void Game::play()
{
    m_room->display();
    Player* player = m_room->player();
    if (player == nullptr)
        return;
    while (!player->isDead() && m_room->beeCount() > 0)
    {
        cout << endl;
        cout << "Move (u/d/l/r/q/h or nothing): ";
        string action;
        getline(cin, action);
        if (action.size() == 0)  // player stands
            player->stand();
        else
        {
            switch (action[0])
            {
            default:   // if bad move, nobody moves
                cout << '\a' << endl;  // beep
                continue;
            case 'h':
                m_room->history().display();
                cout << "Press enter to continue.";
                cin.ignore(10000, '\n');
                break;
            case 'q':
                return;
            case 'u':
            case 'd':
            case 'l':
            case 'r': 
                player->moveOrSwat(decodeDirection(action[0]));
                break;
            }
        }
        m_room->display();
        if (action != "h")
        { 
        m_room->moveBees();
        }
    }
}

int decodeDirection(char dir)
{
    switch (dir)
    {
    case 'u':  return UP;
    case 'd':  return DOWN;
    case 'l':  return LEFT;
    case 'r':  return RIGHT;
    }
    return -1;  // bad argument passed in!
}
