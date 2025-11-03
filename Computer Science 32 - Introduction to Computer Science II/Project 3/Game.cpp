// Game.cpp

#include "provided.h"
#include <utility>
#include <iostream>
using namespace std;

class GameImpl
{
  public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black); 
    bool completed(int& winner) const; 
    bool takeTurn(); 
    void play(); 
    int checkerAt(int c, int r) const;
private:
    Scaffold mScaffold;
    Player* mRed;
    Player* mBlack;
    int mN;
    int mTurn;
};

GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black) : mScaffold(nColumns, nLevels) 
{
    if (N < 0)
    {
        cerr << "N cannot be negative.";
        exit(1);
    }

    mRed = red;
    mBlack = black;
    mN = N;
    mTurn = RED;
}

bool GameImpl::completed(int& winner) const
{
    for (int i = 1; i <= mScaffold.levels(); i++) // row = i, col = j
    {
        for (int j = 1; j <= mScaffold.cols(); j++)
        {
            if (mScaffold.checkerAt(j, i) == VACANT)
                continue;

            int color = mScaffold.checkerAt(j, i);

            // Checking for horizontal matches
            if (j + mN - 1 <= mScaffold.cols())
            {
                bool allMatch = true;
                for (int k = 1; k <= mN - 1; k++)
                {
                    if (mScaffold.checkerAt(j + k, i) != color)
                    {
                        allMatch = false;
                        break;
                    }
                }
                if (allMatch)
                {
                    winner = color;
                    return true;
                }
            }

            // Checking for vertical matches
            if (i + mN - 1 <= mScaffold.levels())
            {
                bool allMatch = true;
                for (int m = 1; m <= mN - 1; m++)
                {
                    if (mScaffold.checkerAt(j, i + m) != color)
                    {
                        allMatch = false;
                        break;
                    }
                }
                if (allMatch)
                {
                    winner = color;
                    return true;
                }
            }
            // Checking for diagonal matches in the "SE" direction
            if (j + mN - 1 <= mScaffold.cols() && i + mN - 1 <= mScaffold.levels())
            {
                bool allMatch = true;
                for (int h = 1; h <= mN - 1; h++)
                {
                    if (mScaffold.checkerAt(j + h, i + h) != color)
                    {
                        allMatch = false;
                        break;
                    }
                }
                if (allMatch)
                {
                    winner = color;
                    return true;
                }
            }
            //  Checking for diagonal matches in the "SW" direction
            if (j -  (mN - 1) >= 1 && i + mN - 1 <= mScaffold.levels())
            {
                bool allMatch = true;
                for (int g = 1; g <= mN - 1; g++)
                {
                    if (mScaffold.checkerAt(j -g, i + g) != color)
                    {
                        allMatch = false;
                        break;
                    }
                }
                if (allMatch)
                {
                    winner = color;
                    return true;
                }
            }

        }
    }

    if (mScaffold.numberEmpty() == 0)
    {
        winner = TIE_GAME;
        return true;
    }

    return false;
}

bool GameImpl::takeTurn()
{
    int result;
    if (completed(result))
    {
        return false;
    }

    Player* current;
    if (mTurn == RED)
    {
        current = mRed;
    }

    else
    {
        current = mBlack;
    }
        
    int desiredMove = current->chooseMove(mScaffold, mN, mTurn);
    mScaffold.makeMove(desiredMove, mTurn);

    if (mTurn == RED)
    {
        mTurn = BLACK;
    }
    else
    {
        mTurn = RED;
    }


    return true;
       
}

void GameImpl::play()
{
    int result;
    while (!completed(result))
    {
        mScaffold.display();
        takeTurn();
    }

    mScaffold.display();
    completed(result);

    if (result == RED)
    {
        cout << mRed->name() << " wins." << endl;
    }
    else if (result == BLACK)
    {
        cout << mBlack->name() << " wins." << endl;
    }
    else
    {
        cout << "Draw." << endl;

    }
}

int GameImpl::checkerAt(int c, int r) const
{
    return mScaffold.checkerAt(c,r);
} 

//******************** Game functions **************************************

//  These functions simply delegate to GameImpl's functions.  You should not
//  change any of the code from here to the end of this file.

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}
 
Game::~Game()
{
    delete m_impl;
}
 
bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}
 
int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}
